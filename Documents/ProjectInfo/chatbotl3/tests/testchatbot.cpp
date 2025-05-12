#include <gtest/gtest.h>
#include "Chatbot.hpp"
#include <string>
#include <regex>

// Création d'un Mock pour éviter les appels API réels
class MockChatbot : public Chatbot {
public:
    MockChatbot() : Chatbot("fake_api_key", "fake_model", "http://localhost:8000") {}
    
    std::string mockSendRequest(const std::string& prompt) {
        last_prompt = prompt;
        return mock_response;
    }
    
    void setMockResponse(const std::string& response) {
        mock_response = response;
    }
    
    std::string getLastPrompt() const {
        return last_prompt;
    }
    
    // Override de la méthode pour utiliser notre mock
    std::string sendRequest(const std::string& prompt) override {
        return mockSendRequest(prompt);
    }

private:
    std::string mock_response;
    std::string last_prompt;
};

// Suite de tests pour Chatbot
TEST(ChatbotTest, InitialisationTest) {
    Chatbot bot("test_key", "test_model", "http://localhost:8000");
    EXPECT_EQ(bot.getApiKey(), "test_key");
    EXPECT_EQ(bot.getModel(), "test_model");
    EXPECT_EQ(bot.getServerUrl(), "http://localhost:8000");
}

TEST(ChatbotTest, SessionIdFormatTest) {
    Chatbot bot("test_key", "test_model");
    std::string session_id = bot.getSessionId();
    // Vérifier que l'ID de session est un nombre à 5 chiffres
    std::regex pattern("^\\d{5}$");
    EXPECT_TRUE(std::regex_match(session_id, pattern));
}

TEST(ChatbotTest, FormatResponseTest) {
    MockChatbot bot;
    
    // Tester le formatage des réponses oui/non
    EXPECT_EQ(bot.formatResponse("oui"), "oui");
    EXPECT_EQ(bot.formatResponse("non"), "non");
    EXPECT_EQ(bot.formatResponse("Bravo, tu as trouvé !"), "Bravo, tu as trouvé !");
    
    // Tester le nettoyage des espaces et sauts de ligne
    EXPECT_EQ(bot.formatResponse("  oui  \n"), "oui");
    EXPECT_EQ(bot.formatResponse("\n\n Bravo, \n tu as trouvé ! \n"), "Bravo, tu as trouvé !");
    
    // Tester les réponses vides
    EXPECT_EQ(bot.formatResponse(""), "Erreur: réponse vide.");
}

TEST(ChatbotTest, EscapeStringTest) {
    Chatbot bot("test_key", "test_model");
    
    // Tester l'échappement des guillemets et sauts de ligne
    EXPECT_EQ(bot.escapeString("test\"string"), "test\\\"string");
    EXPECT_EQ(bot.escapeString("line1\nline2"), "line1\\nline2");
    EXPECT_EQ(bot.escapeString("\"quoted\"\ntext"), "\\\"quoted\\\"\\ntext");
}

TEST(ChatbotTest, AskTest) {
    MockChatbot bot;
    bot.setMockResponse("oui");
    
    // Tester la méthode ask
    std::string response = bot.askQuestion("Est-ce un animal ?");
    EXPECT_EQ(response, "oui");
    EXPECT_TRUE(bot.getLastPrompt().find("Est-ce un animal ?") != std::string::npos);
    
    // Vérifier que l'historique est mis à jour
    const auto& history = bot.getHistory();
    EXPECT_EQ(history.size(), 1);
    EXPECT_EQ(history[0], "Utilisateur: Est-ce un animal ?\nIA: oui");
}

TEST(ChatbotTest, ThinkOfWordTest) {
    MockChatbot bot;
    bot.setMockResponse("J'ai choisi un mot.");
    
    bot.thinkOfWord();
    // Vérifier que la bonne commande a été envoyée
    EXPECT_TRUE(bot.getLastPrompt().find("Choisis un mot") != std::string::npos);
    
    // Vérifier que l'historique est réinitialisé
    EXPECT_TRUE(bot.getHistory().empty());
}

TEST(ChatbotTest, SetUserThinkingTest) {
    MockChatbot bot;
    
    // Ajouter des éléments à l'historique
    bot.askQuestion("Est-ce un animal ?");
    EXPECT_FALSE(bot.getHistory().empty());
    
    // Réinitialiser le contexte
    bot.setThinking();
    EXPECT_TRUE(bot.getHistory().empty());
    EXPECT_TRUE(bot.getContext().find("L'utilisateur a choisi un mot") != std::string::npos);
}

TEST(ChatbotTest, GuessTest) {
    MockChatbot bot;
    bot.setMockResponse("Est-ce que c'est un animal ?");
    
    // Premier guess sans réponse précédente
    std::string guess1 = bot.guess("");
    EXPECT_EQ(guess1, "Est-ce que c'est un animal ?");
    
    // Vérifier que l'historique contient la question
    EXPECT_EQ(bot.getHistory().size(), 1);
    
    // Deuxième guess avec une réponse
    bot.setMockResponse("Est-ce que c'est un chien ?");
    std::string guess2 = bot.guess("oui");
    EXPECT_EQ(guess2, "Est-ce que c'est un chien ?");
    
    // Vérifier que l'historique est mis à jour
    EXPECT_EQ(bot.getHistory().size(), 3);  // Question IA, Réponse utilisateur, Nouvelle question IA
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}