#ifndef DEVINETTE_HPP
#define DEVINETTE_HPP

#include "Chatbot.hpp"
#include <string>

/**
 * @brief États possibles du jeu de devinettes.
 */
enum class GameState {
    INIT,       ///< État initial, affichage du menu
    PLAYER_GUESSING, ///< Mode où le joueur devine le mot de l'IA
    AI_GUESSING,     ///< Mode où l'IA devine le mot du joueur
    GAME_OVER        ///< Fin du jeu
};

/**
 * @class Devinette
 * @brief Gère la logique du jeu de devinettes avec l'IA.
 * 
 * Cette classe implémente un jeu de devinettes avec deux modes :
 * 1. Le joueur devine le mot choisi par l'IA
 * 2. L'IA devine le mot choisi par le joueur
 * 
 * Le jeu propose trois niveaux de difficulté qui déterminent
 * le nombre de questions disponibles pour chaque mode.
 */
class Devinette {
private:
    Chatbot chatbot;           ///< Instance de Chatbot pour l'interaction avec l'IA
    GameState game_state;      ///< État actuel du jeu
    int difficulty_level;      ///< Niveau de difficulté (1: Facile, 2: Normal, 3: Difficile)
    int score_player;          ///< Score du joueur
    int score_ai;              ///< Score de l'IA
    int max_attempts_player;   ///< Nombre maximum de questions pour le joueur
    int max_attempts_ai;       ///< Nombre maximum de questions pour l'IA
    bool debug_mode;           ///< Mode débogage activé/désactivé

    /**
     * @brief Affiche l'écran de bienvenue
     */
    void displayWelcome();
    
    /**
     * @brief Affiche le menu principal
     */
    void showMenu();
    
    /**
     * @brief Gère une partie où le joueur devine le mot de l'IA
     */
    void playerGuessingRound();
    
    /**
     * @brief Gère une partie où l'IA devine le mot du joueur
     */
    void aiGuessingRound();
    
    /**
     * @brief Prépare une partie où le joueur devine le mot
     */
    void preparePlayerGuessingRound();
    
    /**
     * @brief Prépare une partie où l'IA devine le mot
     */
    void prepareAIGuessingRound();
    
    /**
     * @brief Configure le niveau de difficulté
     */
    void setDifficulty();
    
    /**
     * @brief Affiche les scores
     */
    void displayScore();
    
    /**
     * @brief Affiche l'historique des parties
     */
    void displayHistory();
    
    /**
     * @brief Demande au joueur s'il veut rejouer
     * @return true si le joueur veut rejouer, false sinon
     */
    bool askPlayAgain();
    
    /**
     * @brief Affiche le message de fin de jeu
     */
    void displayGoodbye();
    
    /**
     * @brief Récupère une entrée entière dans une plage donnée
     * @param min Valeur minimale acceptée
     * @param max Valeur maximale acceptée
     * @return L'entier saisi par l'utilisateur
     */
    int getIntInput(int min, int max);
    
    /**
     * @brief Vérifie si une question est valide
     * @param question La question à vérifier
     * @return true si la question est valide, false sinon
     */
    bool isValidQuestion(const std::string& question);
    
    /**
     * @brief Vérifie si deux questions sont similaires
     * @param q1 Première question
     * @param q2 Seconde question
     * @return true si les questions sont similaires, false sinon
     */
    bool similarQuestions(const std::string& q1, const std::string& q2);
    
    /**
     * @brief Affiche l'aide du jeu
     */
    void displayHelp();
    
    /**
     * @brief Active ou désactive le mode débogage
     */
    void toggleDebugMode();

public:
    /**
     * @brief Constructeur
     * @param api_key Clé API pour le modèle d'IA
     * @param model_name Nom du modèle d'IA (par défaut: gpt-3.5-turbo)
     * @param server_url URL du serveur API (par défaut: http://llmcode.math.unistra.fr:8090/v1/chat/completions)
     */
    Devinette(const std::string& api_key, 
              const std::string& model_name = "gpt-3.5-turbo",
              const std::string& server_url = "http://llmcode.math.unistra.fr:8090/v1/chat/completions");
    
    /**
     * @brief Démarre le jeu
     */
    void start();

    /**
     * @brief Obtient l'état actuel du jeu
     * @return L'état actuel du jeu
     */
    GameState getGameState() const;
    
    /**
     * @brief Obtient le score du joueur
     * @return Le score du joueur
     */
    int getScorePlayer() const;
    
    /**
     * @brief Obtient le score de l'IA
     * @return Le score de l'IA
     */
    int getScoreAI() const;
    
    /**
     * @brief Vérifie si la partie actuelle est gagnée
     * @return true si la partie est gagnée, false sinon
     */
    bool isGameWon() const;
};

#endif // DEVINETTE_HPP