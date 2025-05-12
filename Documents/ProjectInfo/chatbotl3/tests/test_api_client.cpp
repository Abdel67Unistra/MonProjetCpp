#include <gtest/gtest.h>
#include "ApiClient.hpp"
#include <string>

class ApiClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Utiliser une URL fictive pour éviter les appels réseau réels pendant les tests
        api_client = new ApiClient("test_key", "http://localhost:8090/v1/chat/completions");
    }

    void TearDown() override {
        delete api_client;
    }

    ApiClient* api_client;
};

TEST_F(ApiClientTest, ConstructeurCorrect) {
    EXPECT_EQ(api_client->getLastError(), "");
}

TEST_F(ApiClientTest, GestionErreursTestConnection) {
    // URL invalide devrait échouer proprement
    ApiClient badClient("", "http://invalidurl:12345");
    EXPECT_FALSE(badClient.testConnection());
    EXPECT_FALSE(badClient.getLastError().empty());
}

TEST_F(ApiClientTest, CompletionVerifieParametres) {
    // Les paramètres incorrects ne devraient pas causer de crash
    ApiClient::ModelParams params;
    params.model = "test-model";
    params.temperature = -1.0f;  // Valeur invalide

    // Devrait retourner une chaîne vide en cas d'erreur
    EXPECT_EQ(api_client->completion("Test prompt", params), "");
}

TEST_F(ApiClientTest, ChatCompletionAccepteMessagesVide) {
    // Un tableau de messages vide est techniquement invalide mais ne devrait pas causer de crash
    std::vector<nlohmann::json> empty_messages;
    
    ApiClient::ModelParams params;
    params.model = "test-model";
    
    // Devrait retourner une chaîne vide en cas d'erreur
    EXPECT_EQ(api_client->chatCompletion(empty_messages, params), "");
}

TEST_F(ApiClientTest, OptionsRequeteCorrectementUtilisees) {
    // Tester que les options de requête sont bien utilisées
    ApiClient::RequestOptions options;
    options.timeout_ms = 1;  // Timeout très court pour forcer une erreur
    
    std::vector<nlohmann::json> messages = {
        {{"role", "user"}, {"content", "Hello"}}
    };
    
    ApiClient::ModelParams params;
    params.model = "test-model";
    
    // Devrait échouer avec un timeout si court
    EXPECT_EQ(api_client->chatCompletion(messages, params, options), "");
    EXPECT_FALSE(api_client->getLastError().empty());
}

TEST_F(ApiClientTest, EnregistrementRequetesActif) {
    // Activer l'enregistrement
    api_client->setLogging(true);
    
    // Faire une requête (qui échouera, mais sera enregistrée)
    std::vector<nlohmann::json> messages = {
        {{"role", "user"}, {"content", "Hello"}}
    };
    
    ApiClient::ModelParams params;
    params.model = "test-model";
    
    api_client->chatCompletion(messages, params);
    
    // Vérifier que l'historique n'est pas vide
    EXPECT_FALSE(api_client->getRequestHistory().empty());
}

TEST_F(ApiClientTest, EffacementHistorique) {
    // Activer l'enregistrement
    api_client->setLogging(true);
    
    // Faire une requête (qui échouera, mais sera enregistrée)
    std::vector<nlohmann::json> messages = {
        {{"role", "user"}, {"content", "Hello"}}
    };
    
    ApiClient::ModelParams params;
    params.model = "test-model";
    
    api_client->chatCompletion(messages, params);
    
    // Effacer l'historique
    api_client->clearRequestHistory();
    
    // Vérifier que l'historique est vide
    EXPECT_TRUE(api_client->getRequestHistory().empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 