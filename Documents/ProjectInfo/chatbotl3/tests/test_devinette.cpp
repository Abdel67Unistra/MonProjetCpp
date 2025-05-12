#include <gtest/gtest.h>
#include "Devinette.hpp"
#include <string>

class DevinetteTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Utiliser une URL fictive pour éviter les appels réseau réels pendant les tests
        devinette = new Devinette("test_key", "test-model", "http://localhost:8090/v1/chat/completions");
    }

    void TearDown() override {
        delete devinette;
    }

    Devinette* devinette;
};

// Test de l'état initial
TEST_F(DevinetteTest, EtatInitial) {
    EXPECT_EQ(devinette->getGameState(), GameState::INIT);
    EXPECT_EQ(devinette->getScorePlayer(), 0);
    EXPECT_EQ(devinette->getScoreAI(), 0);
    EXPECT_FALSE(devinette->isGameWon());
}

// Test des scores
TEST_F(DevinetteTest, GestionScores) {
    // Simuler une partie gagnée par le joueur
    // Note: Nous devons accéder à des méthodes privées, donc ce test est limité
    
    // Vérification indirecte via isGameWon
    EXPECT_FALSE(devinette->isGameWon()); // Au départ, pas de gagnant
}

// Test de validation des questions
TEST_F(DevinetteTest, ValidationQuestion) {
    // Cette méthode est privée, donc nous ne pouvons pas la tester directement sans modifier la classe
    // Un test indirect serait nécessaire via une méthode publique qui l'utilise
    
    // Vérification de base que l'objet est dans un état cohérent
    EXPECT_EQ(devinette->getGameState(), GameState::INIT);
}

// Test de changement de difficulté
// Note: Cette méthode est privée, nous testons donc indirectement
TEST_F(DevinetteTest, ChangementDifficulte) {
    // Vérification que l'état initial est correct
    EXPECT_EQ(devinette->getGameState(), GameState::INIT);
}

// Test d'initialisation avec différents paramètres
TEST_F(DevinetteTest, InitialisationParametres) {
    Devinette customDevinette("custom_key", "custom-model", "http://example.com");
    EXPECT_EQ(customDevinette.getGameState(), GameState::INIT);
}

// Test de l'état du jeu
TEST_F(DevinetteTest, EtatJeu) {
    EXPECT_EQ(devinette->getGameState(), GameState::INIT);
    // Les changements d'état nécessitent une interaction utilisateur,
    // donc difficiles à tester automatiquement
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}