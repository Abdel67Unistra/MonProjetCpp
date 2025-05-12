#include "ApiClient.hpp"
#include "JsonUtils.hpp"
#include <iostream>
#include <chrono>
#include <thread>

ApiClient::ApiClient(const std::string& api_key, const std::string& base_url)
    : api_key(api_key),
      base_url(base_url),
      last_error(""),
      logging_enabled(false),
      request_history(nlohmann::json::array()),
      progress_callback(nullptr) {
}

bool ApiClient::testConnection(const RequestOptions& options) {
    try {
        // Construire une requête simple pour tester la connexion
        nlohmann::json payload = {
            {"model", "test-model"},
            {"messages", nlohmann::json::array({
                {{"role", "user"}, {"content", "Hello"}}
            })},
            {"max_tokens", 5}
        };
        
        // URL de test (utilisant l'endpoint de chat)
        std::string test_url = base_url;
        
        // Effectuer la requête avec un timeout court
        RequestOptions test_options = options;
        test_options.timeout_ms = 5000;  // 5 secondes seulement pour le test
        test_options.max_retries = 1;    // Une seule tentative
        
        cpr::Response response = makeRequest(test_url, payload, test_options);
        
        // La connexion est considérée comme réussie si le code est entre 200 et 499
        // (même une erreur 400 signifie que le serveur est joignable)
        return response.status_code >= 200 && response.status_code < 500;
    } catch (const std::exception& e) {
        last_error = "Erreur lors du test de connexion: " + std::string(e.what());
        return false;
    }
}

std::string ApiClient::chatCompletion(
    const std::vector<nlohmann::json>& messages,
    const ModelParams& params,
    const RequestOptions& options) {
    try {
        // Construire le payload
        nlohmann::json payload = {
            {"model", params.model},
            {"messages", nlohmann::json::array()},
            {"temperature", params.temperature},
            {"max_tokens", params.max_tokens},
            {"top_p", params.top_p},
            {"frequency_penalty", params.frequency_penalty},
            {"presence_penalty", params.presence_penalty}
        };
        
        // Ajouter les messages
        for (const auto& message : messages) {
            payload["messages"].push_back(message);
        }
        
        // Effectuer la requête
        cpr::Response response = makeRequest(base_url, payload, options);
        
        // Traiter la réponse
        return processResponse(response);
    } catch (const std::exception& e) {
        last_error = "Erreur lors de la requête de chat: " + std::string(e.what());
        return "";
    }
}

std::string ApiClient::completion(
    const std::string& prompt,
    const ModelParams& params,
    const RequestOptions& options) {
    try {
        // Convertir le prompt en format de messages pour l'API de chat
        std::vector<nlohmann::json> messages = {
            {{"role", "user"}, {"content", prompt}}
        };
        
        // Utiliser la méthode chatCompletion
        return chatCompletion(messages, params, options);
    } catch (const std::exception& e) {
        last_error = "Erreur lors de la requête de complétion: " + std::string(e.what());
        return "";
    }
}

void ApiClient::setProgressCallback(ProgressCallback callback) {
    progress_callback = callback;
}

std::string ApiClient::getLastError() const {
    return last_error;
}

void ApiClient::setLogging(bool enable) {
    logging_enabled = enable;
}

nlohmann::json ApiClient::getRequestHistory() const {
    std::lock_guard<std::mutex> lock(history_mutex);
    return request_history;
}

void ApiClient::clearRequestHistory() {
    std::lock_guard<std::mutex> lock(history_mutex);
    request_history = nlohmann::json::array();
}

cpr::Response ApiClient::makeRequest(
    const std::string& url,
    const nlohmann::json& payload,
    const RequestOptions& options) {
    // Préparer les headers
    cpr::Header headers = {
        {"Content-Type", options.content_type}
    };
    
    // Ajouter l'Authorization header si une clé API est fournie
    if (!api_key.empty()) {
        headers["Authorization"] = "Bearer " + api_key;
    }
    
    // Convertir le payload en string
    std::string payload_str = JsonUtils::stringify(payload);
    
    // Afficher les détails de la requête en mode verbeux
    if (options.verbose) {
        std::cout << "URL: " << url << std::endl;
        std::cout << "Headers: " << JsonUtils::stringify(headers) << std::endl;
        std::cout << "Payload: " << payload_str << std::endl;
    }
    
    // Préparer la session
    cpr::Session session;
    session.SetUrl(cpr::Url{url});
    
    // Ajouter chaque header individuellement
    for (const auto& header : headers) {
        session.SetHeader(cpr::Header{{header.first, header.second}});
    }
    
    session.SetBody(cpr::Body{payload_str});
    session.SetTimeout(cpr::Timeout{options.timeout_ms});
    
    // Fonction pour configurer le callback de progression si nécessaire
    if (progress_callback) {
        // Dans cette version simplifiée, on n'utilise pas le callback de progression
        // car la signature a changé dans les versions de CPR
    }
    
    // Effectuer la requête avec retry
    cpr::Response response;
    for (int attempt = 0; attempt < options.max_retries; ++attempt) {
        if (attempt > 0 && options.verbose) {
            std::cout << "Tentative " << (attempt + 1) << "/" << options.max_retries << std::endl;
        }
        
        response = session.Post();
        
        // Si la requête a réussi ou si c'est une erreur client (4xx), pas besoin de retry
        if (response.status_code >= 200 && response.status_code < 500) {
            break;
        }
        
        // En cas d'erreur serveur (5xx), attendre avant de réessayer
        if (attempt < options.max_retries - 1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(options.retry_delay_ms));
        }
    }
    
    // Enregistrer la requête et la réponse si l'enregistrement est activé
    if (logging_enabled) {
        nlohmann::json response_json;
        try {
            response_json = JsonUtils::parse(response.text);
        } catch (...) {
            response_json = {
                {"text", response.text},
                {"status_code", response.status_code}
            };
        }
        
        logRequest(payload, response_json);
    }
    
    // Afficher les détails de la réponse en mode verbeux
    if (options.verbose) {
        std::cout << "Status: " << response.status_code << std::endl;
        std::cout << "Response: " << response.text << std::endl;
    }
    
    return response;
}

std::string ApiClient::processResponse(const cpr::Response& response) {
    // Vérifier le code de statut
    if (response.status_code != 200) {
        last_error = "Erreur HTTP " + std::to_string(response.status_code) + ": " + response.text;
        return "";
    }
    
    try {
        // Parser la réponse JSON
        nlohmann::json json_response = JsonUtils::parse(response.text);
        
        // Format de réponse standard pour l'API OpenAI
        if (json_response.contains("choices") && !json_response["choices"].empty()) {
            const auto& first_choice = json_response["choices"][0];
            
            // Pour les modèles de chat (contenu dans message)
            if (first_choice.contains("message") && first_choice["message"].contains("content")) {
                return JsonUtils::getString(first_choice["message"], "content");
            }
            
            // Pour les modèles de complétion (contenu direct dans text)
            if (first_choice.contains("text")) {
                return JsonUtils::getString(first_choice, "text");
            }
        }
        
        // Si le format n'est pas reconnu, retourner la réponse brute
        last_error = "Format de réponse non reconnu";
        return response.text;
    } catch (const std::exception& e) {
        last_error = "Erreur lors du traitement de la réponse: " + std::string(e.what());
        return "";
    }
}

void ApiClient::logRequest(const nlohmann::json& request, const nlohmann::json& response) {
    std::lock_guard<std::mutex> lock(history_mutex);
    
    // Créer une entrée pour cette requête
    nlohmann::json entry = {
        {"timestamp", std::chrono::system_clock::now().time_since_epoch().count()},
        {"request", request},
        {"response", response}
    };
    
    // Ajouter à l'historique
    request_history.push_back(entry);
} 