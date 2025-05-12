#include <gtest/gtest.h>
#include "Devinette.hpp"
#include "Chatbot.hpp"
#include <string>
#include <sstream>

// Mock pour simuler l'entrée utilisateur
class MockDevinette : public Devinette {
public:
    MockDevinette() : Devinette("fake_api_key", "fake_model") {}
    
    void setMockInput(const std::vector<std::string>& inputs) {
        mock_inputs = inputs;
        input_index = 0;
    }
    
    // Override les méthodes qui lisent l'entrée utilisateur
    bool isValidQuestion(const std::string& question) {
        return Devinette::isValidQuestion(question);
    }
    
    int getMockIntInput(int min, int max) {
        if (input_index < mock_inputs.size()) {
            try {
                int val = std::stoi(mock_inputs[input_index++]);
                if (val >= min && val <= max) return val;
            } catch (...) {}
        }
        return min; // Valeur par défaut
    }

private:
    std::vector<std::string> mock_inputs;
    size_t input_index = 0;
};

// Suite de tests pour Devinette
TEST(DevinetteTest, InitialisationTest) {
    Devinette jeu("test_key", "test_model");
    EXPECT_EQ(jeu.getGameState(), GameState::INIT);
    EXPECT_EQ(jeu.getDifficultyLevel(), 1);
    EXPECT_EQ(jeu.getScorePlayer(), 0);
    EXPECT_EQ(jeu.getScoreAI(), 0);
}

TEST(DevinetteTest, IsValidQuestionTest) {
    MockDevinette jeu;
    
    // Questions valides
    EXPECT_TRUE(jeu.isValidQuestion("Est-ce un animal ?"));
    EXPECT_TRUE(jeu.isValidQuestion("A-t-il quatre pattes ?"));
    EXPECT_TRUE(jeu.isValidQuestion("Peut-on le manger ?"));
    
    // Questions invalides
    EXPECT_FALSE(jeu.isValidQuestion("un animal"));
    EXPECT_FALSE(jeu.isValidQuestion("chat"));
    
    // Cas limites
    EXPECT_TRUE(jeu.isValidQuestion("animal ?"));  // Contient "?" mais pas de formulation standard
    EXPECT_FALSE(jeu.isValidQuestion("Est-ce un animal"));  // Pas de point d'interrogation
}

TEST(DevinetteTest, SetDifficultyTest) {
    MockDevinette jeu;
    
    // Difficulté initiale
    EXPECT_EQ(jeu.getDifficultyLevel(), 1);
    EXPECT_EQ(jeu.getMaxAttemptsPlayer(), 16);
    EXPECT_EQ(jeu.getMaxAttemptsAI(), 15);
    
    // Changer la difficulté à 2 (Normal)
    jeu.setDifficulty(2);
    EXPECT_EQ(jeu.getDifficultyLevel(), 2);
    EXPECT_EQ(jeu.getMaxAttemptsPlayer(), 12);
    EXPECT_EQ(jeu.getMaxAttemptsAI(), 12);
    
    // Changer la difficulté à 3 (Difficile)
    jeu.setDifficulty(3);
    EXPECT_EQ(jeu.getDifficultyLevel(), 3);
    EXPECT_EQ(jeu.getMaxAttemptsPlayer(), 8);
    EXPECT_EQ(jeu.getMaxAttemptsAI(), 8);
}

TEST(DevinetteTest, ScoreManagementTest) {
    MockDevinette jeu;
    
    // Scores initiaux
    EXPECT_EQ(jeu.getScorePlayer(), 0);
    EXPECT_EQ(jeu.getScoreAI(), 0);
    
    // Incrémenter le score du joueur
    jeu.incrementPlayerScore();
    EXPECT_EQ(jeu.getScorePlayer(), 1);
    EXPECT_EQ(jeu.getScoreAI(), 0);
    
    // Incrémenter le score de l'IA
    jeu.incrementAIScore();
    EXPECT_EQ(jeu.getScorePlayer(), 1);
    EXPECT_EQ(jeu.getScoreAI(), 1);
    
    // Réinitialiser les scores
    jeu.resetScores();
    EXPECT_EQ(jeu.getScorePlayer(), 0);
    EXPECT_EQ(jeu.getScoreAI(), 0);
}

TEST(DevinetteTest, GameStateTransitionsTest) {
    MockDevinette jeu;
    
    // État initial
    EXPECT_EQ(jeu.getGameState(), GameState::INIT);
    
    // Transition vers PLAYER_GUESSING
    jeu.setGameState(GameState::PLAYER_GUESSING);
    EXPECT_EQ(jeu.getGameState(), GameState::PLAYER_GUESSING);
    
    // Transition vers AI_GUESSING
    jeu.setGameState(GameState::AI_GUESSING);
    EXPECT_EQ(jeu.getGameState(), GameState::AI_GUESSING);
    
    // Transition vers GAME_OVER
    jeu.setGameState(GameState::GAME_OVER);
    EXPECT_EQ(jeu.getGameState(), GameState::GAME_OVER);
    
    // Retour à INIT
    jeu.setGameState(GameState::INIT);
    EXPECT_EQ(jeu.getGameState(), GameState::INIT);
}

TEST(DevinetteTest, DebugModeToggleTest) {
    MockDevinette jeu;
    
    // Mode debug initial (désactivé)
    EXPECT_FALSE(jeu.isDebugModeEnabled());
    
    // Activer le mode debug
    jeu.setDebugMode(true);
    EXPECT_TRUE(jeu.isDebugModeEnabled());
    
    // Désactiver le mode debug
    jeu.setDebugMode(false);
    EXPECT_FALSE(jeu.isDebugModeEnabled());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}