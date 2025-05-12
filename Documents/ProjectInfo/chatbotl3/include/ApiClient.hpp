#ifndef API_CLIENT_HPP
#define API_CLIENT_HPP

#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

/**
 * @class ApiClient
 * @brief Gère les communications avec l'API LLM
 * 
 * Cette classe encapsule toutes les communications HTTP avec l'API
 * du serveur LLM, avec gestion des erreurs, retry, timeouts, etc.
 */
class ApiClient {
public:
    /**
     * @brief Structure pour les options de requête
     */
    struct RequestOptions {
        int timeout_ms;           ///< Timeout en millisecondes
        int max_retries;          ///< Nombre maximum de tentatives
        int retry_delay_ms;       ///< Délai entre les tentatives
        bool verbose;             ///< Mode verbeux
        std::string content_type; ///< Type de contenu
        
        // Constructeur avec valeurs par défaut
        RequestOptions() : 
            timeout_ms(30000),
            max_retries(3),
            retry_delay_ms(1000),
            verbose(false),
            content_type("application/json") {}
    };
    
    /**
     * @brief Structure pour les paramètres du modèle LLM
     */
    struct ModelParams {
        std::string model;        ///< Nom du modèle
        float temperature;        ///< Température (aléatoire)
        int max_tokens;           ///< Nombre maximum de tokens
        float top_p;              ///< Top-p sampling
        float frequency_penalty;  ///< Pénalité de fréquence
        float presence_penalty;   ///< Pénalité de présence
        
        // Constructeur avec valeurs par défaut
        ModelParams() :
            model(""),
            temperature(0.7f),
            max_tokens(500),
            top_p(1.0f),
            frequency_penalty(0.0f),
            presence_penalty(0.0f) {}
    };
    
    /**
     * @brief Fonction callback pour le progrès des requêtes
     */
    using ProgressCallback = std::function<void(int, int, int, int)>;
    
    /**
     * @brief Constructeur
     * @param api_key Clé API (peut être vide pour certains serveurs)
     * @param base_url URL de base du serveur
     */
    ApiClient(const std::string& api_key, const std::string& base_url);
    
    /**
     * @brief Destructeur
     */
    ~ApiClient() = default;
    
    /**
     * @brief Teste la connexion au serveur
     * @param options Options de requête
     * @return true si la connexion est établie, false sinon
     */
    bool testConnection(const RequestOptions& options = RequestOptions());
    
    /**
     * @brief Envoie une requête de chat à l'API
     * @param messages Liste des messages de la conversation
     * @param params Paramètres du modèle
     * @param options Options de requête
     * @return La réponse du modèle
     */
    std::string chatCompletion(
        const std::vector<nlohmann::json>& messages,
        const ModelParams& params = ModelParams(),
        const RequestOptions& options = RequestOptions()
    );
    
    /**
     * @brief Envoie une requête de complétion à l'API
     * @param prompt Le prompt à compléter
     * @param params Paramètres du modèle
     * @param options Options de requête
     * @return La réponse du modèle
     */
    std::string completion(
        const std::string& prompt,
        const ModelParams& params = ModelParams(),
        const RequestOptions& options = RequestOptions()
    );
    
    /**
     * @brief Définit la fonction de callback pour le progrès
     * @param callback La fonction de callback
     */
    void setProgressCallback(ProgressCallback callback);
    
    /**
     * @brief Obtient la dernière erreur
     * @return Message de la dernière erreur
     */
    std::string getLastError() const;
    
    /**
     * @brief Définit si le client doit enregistrer les requêtes et réponses
     * @param enable true pour activer l'enregistrement
     */
    void setLogging(bool enable);
    
    /**
     * @brief Obtient l'historique des requêtes et réponses
     * @return L'historique au format JSON
     */
    nlohmann::json getRequestHistory() const;
    
    /**
     * @brief Efface l'historique des requêtes
     */
    void clearRequestHistory();

private:
    std::string api_key;             ///< Clé API
    std::string base_url;            ///< URL de base du serveur
    std::string last_error;          ///< Dernière erreur rencontrée
    bool logging_enabled;            ///< Enregistrement activé/désactivé
    nlohmann::json request_history;  ///< Historique des requêtes
    mutable std::mutex history_mutex;///< Mutex pour l'historique
    ProgressCallback progress_callback; ///< Fonction de callback pour le progrès
    
    /**
     * @brief Effectue une requête HTTP avec gestion des erreurs et retry
     * @param url URL de la requête
     * @param payload Payload JSON
     * @param options Options de requête
     * @return La réponse HTTP
     */
    cpr::Response makeRequest(
        const std::string& url,
        const nlohmann::json& payload,
        const RequestOptions& options
    );
    
    /**
     * @brief Traite une réponse HTTP
     * @param response Réponse HTTP
     * @return Le contenu extrait
     */
    std::string processResponse(const cpr::Response& response);
    
    /**
     * @brief Ajoute une entrée à l'historique des requêtes
     * @param request Requête
     * @param response Réponse
     */
    void logRequest(const nlohmann::json& request, const nlohmann::json& response);
};

#endif // API_CLIENT_HPP 