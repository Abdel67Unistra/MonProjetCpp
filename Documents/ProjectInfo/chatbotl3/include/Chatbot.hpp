#pragma once

#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <map>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

/**
 * @class Chatbot
 * @brief Classe gérant l'interaction avec l'IA pour le jeu de devinettes.
 * 
 * Cette classe permet de :
 * 1. Générer un mot aléatoire pour que le joueur le devine
 * 2. Répondre aux questions du joueur par oui/non
 * 3. Générer des questions stratégiques pour deviner le mot du joueur
 * 4. Analyser les réponses du joueur pour affiner sa stratégie
 */
class Chatbot {
private:
    // Configuration
    std::string api_key;       ///< Clé API pour le modèle d'IA
    std::string model;         ///< Nom du modèle d'IA utilisé
    std::string server_url;    ///< URL du serveur API
    bool debug_mode;           ///< Mode débogage activé/désactivé

    // État du jeu
    bool gameEnded;            ///< Indique si la partie est terminée
    bool first_display;        ///< Indique si c'est le premier affichage
    bool is_thinking;          ///< Indique si l'IA est en train de réfléchir
    bool is_connected;         ///< État de la connexion API
    bool connection_tested;    ///< Indique si la connexion a déjà été testée
    std::string chosen_word;   ///< Mot choisi par l'IA
    std::string chosen_category; ///< Catégorie du mot choisi
    std::string session_id;    ///< Identifiant de la session actuelle
    std::string context;       ///< Contexte pour les requêtes à l'IA
    std::string current_context; ///< Contexte actuel
    std::string initial_context; ///< Contexte initial
    std::string last_error;    ///< Dernière erreur rencontrée
    std::vector<std::string> game_history; ///< Historique complet du jeu
    std::vector<std::string> message_history; ///< Historique des messages
    std::vector<std::string> history;     ///< Historique des questions
    std::vector<std::string> reponses;    ///< Historique des réponses
    std::string current_display; ///< Affichage actuel

    // Statistiques
    int score;                 ///< Score actuel
    int total_games;           ///< Nombre total de parties jouées
    int games_won;             ///< Nombre de parties gagnées

    // Générateur de nombres aléatoires
    std::mt19937 rng;          ///< Générateur de nombres aléatoires

    /**
     * @brief Normalise un mot (minuscules, sans ponctuation)
     * @param word Le mot à normaliser
     * @return Le mot normalisé
     */
    std::string normalizeWord(const std::string& word) const;
    
    /**
     * @brief Vérifie si deux questions sont similaires
     * @param q1 Première question
     * @param q2 Seconde question
     * @return true si les questions sont similaires, false sinon
     */
    bool similarQuestions(const std::string& q1, const std::string& q2);

    /**
     * @brief Initialise le générateur de nombres aléatoires
     */
    void initializeRNG();
    
    /**
     * @brief Réinitialise l'état du jeu
     */
    void reset();
    
    /**
     * @brief Sélectionne aléatoirement une catégorie et un mot
     */
    void selectRandomCategoryAndWord();
    
    /**
     * @brief Initialise ou complète la base de connaissances pour le mot choisi
     */
    void initializeWordKnowledge();
    
    /**
     * @brief Affiche l'interface du jeu
     */
    void displayGameInterface();
    
    /**
     * @brief Configure le contexte du jeu
     */
    void setupGameContext();
    
    /**
     * @brief Traite une question du joueur
     * @param question La question posée
     * @return La réponse à la question
     */
    std::string ask(const std::string& question);
    
    /**
     * @brief Vérifie si la question est une commande spéciale
     * @param question La question à vérifier
     * @return true si c'est une commande spéciale, false sinon
     */
    bool isSpecialCommand(const std::string& question);
    
    /**
     * @brief Traite une commande spéciale
     * @param command La commande à traiter
     * @return Le résultat de la commande
     */
    std::string handleSpecialCommand(const std::string& command);
    
    /**
     * @brief Affiche les statistiques du jeu
     */
    void displayGameStats();
    
    /**
     * @brief Nettoie une question entrée par l'utilisateur
     * @param question La question à nettoyer
     * @return La question nettoyée
     */
    std::string cleanInputQuestion(const std::string& question);
    
    /**
     * @brief Construit le contexte d'historique pour l'IA
     * @return Le contexte d'historique
     */
    std::string buildHistoryContext();
    
    /**
     * @brief Traite une question et génère une réponse
     * @param question La question à traiter
     * @param history_context Le contexte d'historique
     * @return La réponse à la question
     */
    std::string processQuestion(const std::string& question, const std::string& history_context);
    
    /**
     * @brief Envoie une requête à l'API d'IA
     * @param prompt Le prompt à envoyer
     * @return La réponse de l'IA
     */
    std::string sendRequest(const std::string& prompt);
    
    /**
     * @brief Construit le payload JSON pour la requête API
     * @param prompt Le prompt à inclure dans le payload
     * @return Le payload JSON
     */
    nlohmann::json buildPayload(const std::string& prompt);
    
    /**
     * @brief Vérifie si le modèle est un modèle Llama
     * @return true si c'est un modèle Llama, false sinon
     */
    bool isLlamaModel();
    
    /**
     * @brief Ajoute des options spécifiques pour les modèles Llama
     * @param payload Le payload à modifier
     */
    void addLlamaSpecificOptions(nlohmann::json& payload);
    
    /**
     * @brief Effectue la requête API
     * @param payload Le payload de la requête
     * @return La réponse de l'API
     */
    cpr::Response makeApiRequest(const nlohmann::json& payload);
    
    /**
     * @brief Traite une réponse API réussie
     * @param r La réponse API
     * @return Le contenu extrait de la réponse
     */
    std::string handleSuccessfulResponse(const cpr::Response& r);
    
    /**
     * @brief Extrait le contenu d'une réponse JSON
     * @param response La réponse JSON
     * @return Le contenu extrait
     */
    std::string extractContent(const nlohmann::json& response);
    
    /**
     * @brief Traite une erreur de réponse API
     * @param r La réponse API en erreur
     * @return Un message d'erreur
     */
    std::string handleErrorResponse(const cpr::Response& r);
    
    /**
     * @brief Teste la connexion à l'API
     * @return true si la connexion est établie, false sinon
     */
    bool testConnection();
    
    /**
     * @brief Initialise le mode où l'IA devine le mot du joueur
     */
    void setUserThinking();
    
    /**
     * @brief Envoie une requête directe à l'API avec un contexte temporaire
     * @param prompt Le prompt à envoyer
     * @param temp_context Le contexte temporaire
     * @return La réponse de l'IA
     */
    std::string sendDirectRequest(const std::string& prompt, const std::string& temp_context);
    
    /**
     * @brief Gère la victoire du joueur
     * @return Un message de victoire
     */
    std::string handleVictory();
    
    /**
     * @brief Affiche les statistiques finales
     */
    void displayFinalStats() const;
    
    /**
     * @brief Demande au joueur s'il veut jouer une nouvelle partie
     * @return true si le joueur veut rejouer, false sinon
     */
    bool askNewGame() const;

public:
    /**
     * @brief Constructeur
     * @param api_key Clé API pour le modèle d'IA
     * @param model Nom du modèle d'IA
     * @param server_url URL du serveur API
     * @param debug_mode Mode débogage activé/désactivé
     */
    Chatbot(const std::string& api_key, const std::string& model, const std::string& server_url, bool debug_mode = false);
    
    /**
     * @brief Fonction virtuelle pour démarrer le jeu (implémentation vide par défaut)
     */
    virtual void start() {}
    
    /**
     * @brief Fait choisir un mot à l'IA
     */
    void thinkOfWord();
    
    /**
     * @brief Obtient le mot choisi par l'IA
     * @return Le mot choisi
     */
    std::string getChosenWord() const { return chosen_word; }
    
    /**
     * @brief Obtient la catégorie du mot choisi
     * @return La catégorie du mot
     */
    std::string getChosenCategory() const { return chosen_category; }
    
    /**
     * @brief Obtient la dernière erreur
     * @return La dernière erreur
     */
    std::string getLastError() const { return last_error; }
    
    /**
     * @brief Obtient la clé API
     * @return La clé API
     */
    std::string getApiKey() const { return api_key; }
    
    /**
     * @brief Obtient le nom du modèle
     * @return Le nom du modèle
     */
    std::string getModel() const { return model; }
    
    /**
     * @brief Obtient l'historique des questions
     * @return L'historique des questions
     */
    const std::vector<std::string>& getHistory() const { return history; }
    
    /**
     * @brief Réinitialise le jeu
     */
    void resetGame() { reset(); }
    
    /**
     * @brief Pose une question à l'IA
     * @param question La question à poser
     * @return La réponse de l'IA
     */
    std::string askQuestion(const std::string& question) { return ask(question); }
    
    /**
     * @brief Initialise le mode où l'IA devine
     */
    void setThinking() { setUserThinking(); }
    
    /**
     * @brief Vérifie la connexion à l'API
     * @return true si la connexion est établie, false sinon
     */
    bool checkConnection() { return testConnection(); }
    
    /**
     * @brief Vérifie si l'API est connectée (utilise un cache)
     * @return true si connecté, false sinon
     */
    bool isConnected();
    
    /**
     * @brief Force un test de connexion et met à jour le statut
     */
    void checkConnectionStatus();
    
    /**
     * @brief Génère une question pour deviner le mot du joueur
     * @param last_answer La dernière réponse du joueur
     * @return La question générée par l'IA
     */
    std::string guess(const std::string& last_answer);
}; 