#include <gtest/gtest.h>
#include "Chatbot.hpp"
#include "Devinette.hpp"
#include <string>

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Utiliser une URL fictive pour éviter les appels réseau réels pendant les tests
        chatbot = new Chatbot("test_key", "test-model", "http://localhost:8090/v1/chat/completions");
        devinette = new Devinette("test_key", "test-model", "http://localhost:8090/v1/chat/completions");
    }

    void TearDown() override {
        delete chatbot;
        delete devinette;
    }

    Chatbot* chatbot;
    Devinette* devinette;
};

// Test d'intégration entre Chatbot et son API
// Ce test nécessiterait normalement un serveur mock, mais nous vérifions juste 
// que l'appel ne cause pas de crash
TEST_F(IntegrationTest, ChatbotAPI) {
    // Ce test devrait échouer proprement car l'URL est fictive
    EXPECT_FALSE(chatbot->checkConnection());
}

// Test d'intégration du jeu complet
// Ce test vérifie que l'état du jeu est cohérent après initialisation
TEST_F(IntegrationTest, JeuComplet) {
    EXPECT_EQ(devinette->getGameState(), GameState::INIT);
    EXPECT_EQ(devinette->getScorePlayer(), 0);
    EXPECT_EQ(devinette->getScoreAI(), 0);
}

// Test d'intégration du cycle de vie du jeu
TEST_F(IntegrationTest, CycleVieJeu) {
    // Vérifier l'état initial
    EXPECT_EQ(devinette->getGameState(), GameState::INIT);
    
    // Note: Nous ne pouvons pas facilement tester le cycle complet car cela nécessiterait
    // une interaction utilisateur ou des modifications de la classe pour permettre des tests automatisés
}

// Test d'intégration de la communication Chatbot-API
TEST_F(IntegrationTest, CommunicationAPI) {
    // Vérifier que les appels d'API échouent proprement avec notre URL fictive
    EXPECT_FALSE(chatbot->checkConnection());
    EXPECT_FALSE(chatbot->getLastError().empty());
}

// Test de l'intégration du mode de devinette utilisateur
TEST_F(IntegrationTest, ModeDevinetteUtilisateur) {
    // Vérifie que le chatbot peut préparer un jeu sans erreur fatale
    EXPECT_NO_THROW(chatbot->thinkOfWord());
}

// Test de l'intégration du mode de devinette IA
TEST_F(IntegrationTest, ModeDevinetteIA) {
    // Vérifie que le chatbot peut préparer un mode IA sans erreur fatale
    EXPECT_NO_THROW(chatbot->setThinking());
}

// Test d'intégration de la réinitialisation complète
TEST_F(IntegrationTest, ReinitialisationComplete) {
    // Réinitialiser le chatbot et vérifier l'état
    chatbot->resetGame();
    const auto& history = chatbot->getHistory();
    EXPECT_TRUE(history.empty());
    
    // Vérifier que le jeu est dans un état initial cohérent
    EXPECT_EQ(devinette->getGameState(), GameState::INIT);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}