#ifndef DEVINETTE_HPP
#define DEVINETTE_HPP

#include "Chatbot.hpp"
#include <string>

/**
 * @brief États possibles du jeu de devinettes.
 */
enum class GameState {
    INIT,
    PLAYER_GUESSING,
    AI_GUESSING,
    GAME_OVER
};

/**
 * @class Devinette
 * @brief Gère la logique du jeu de devinettes avec l'IA.
 */
class Devinette {
private:
    Chatbot chatbot;
    GameState game_state;
    int difficulty_level;
    int score_player;
    int score_ai;
    int max_attempts_player;
    int max_attempts_ai;
    bool debug_mode;

    void displayWelcome();
    void showMenu();
    void playerGuessingRound();
    void aiGuessingRound();
    void preparePlayerGuessingRound();
    void prepareAIGuessingRound();
    void setDifficulty();
    void displayScore();
    void displayHistory();
    bool askPlayAgain();
    void displayGoodbye();
    int getIntInput(int min, int max);
    bool isValidQuestion(const std::string& question);
    bool similarQuestions(const std::string& q1, const std::string& q2);
    void displayHelp();
    void toggleDebugMode();

public:
    Devinette(const std::string& api_key, 
              const std::string& model_name = "gpt-3.5-turbo",
              const std::string& server_url = "http://llmcode.math.unistra.fr:8090/v1/chat/completions");
    void start();

    GameState getGameState() const;
    int getScorePlayer() const;
    int getScoreAI() const;
    bool isGameWon() const;
};

#endif // DEVINETTE_HPP 