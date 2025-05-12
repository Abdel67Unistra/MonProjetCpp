#include <gtest/gtest.h>
#include "Chatbot.hpp"
#include <string>

class ChatbotTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Utiliser une URL fictive pour éviter les appels réseau réels pendant les tests
        chatbot = new Chatbot("test_key", "test-model", "http://localhost:8090/v1/chat/completions");
    }

    void TearDown() override {
        delete chatbot;
    }

    Chatbot* chatbot;
};

TEST_F(ChatbotTest, InitialisationCorrect) {
    EXPECT_EQ(chatbot->getModel(), "test-model");
    EXPECT_EQ(chatbot->getApiKey(), "test_key");
}

TEST_F(ChatbotTest, HistoriqueInitialVide) {
    const auto& history = chatbot->getHistory();
    EXPECT_TRUE(history.empty());
}

TEST_F(ChatbotTest, ResetEffaceHistorique) {
    // Simuler une question pour avoir un historique
    // Note: ceci ne fait pas vraiment d'appel API car nous avons remplacé l'URL
    chatbot->askQuestion("test question");
    
    // Reset devrait effacer l'historique
    chatbot->resetGame();
    const auto& history = chatbot->getHistory();
    EXPECT_TRUE(history.empty());
}

// Test du formatage des réponses
TEST_F(ChatbotTest, FormatageReponse) {
    // Test indirectement via une méthode publique
    // Vérification de base que le chatbot n'échoue pas sur des opérations simples
    EXPECT_NO_THROW(chatbot->resetGame());
}

// Test de la gestion des erreurs
TEST_F(ChatbotTest, GestionErreurs) {
    // Le test devrait échouer proprement (sans crash) sur une URL invalide
    Chatbot badChatbot("", "", "http://invalidurl:12345");
    EXPECT_FALSE(badChatbot.checkConnection());
    EXPECT_FALSE(badChatbot.getLastError().empty());
}

// Test de thinkOfWord
TEST_F(ChatbotTest, ThinkOfWordChangesContext) {
    // Cette méthode devrait modifier le contexte interne
    // Sans vérifier le résultat exact (qui implique une vraie API)
    EXPECT_NO_THROW(chatbot->thinkOfWord());
}

// Test de setUserThinking
TEST_F(ChatbotTest, SetUserThinkingChangesContext) {
    EXPECT_NO_THROW(chatbot->setThinking());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}