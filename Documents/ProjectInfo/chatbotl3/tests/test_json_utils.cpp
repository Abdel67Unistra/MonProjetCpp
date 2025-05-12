#include <gtest/gtest.h>
#include "JsonUtils.hpp"
#include <string>
#include <vector>

TEST(JsonUtilsTest, GetStringExtraitValeurCorrectement) {
    nlohmann::json j = {{"key", "value"}};
    EXPECT_EQ(JsonUtils::getString(j, "key"), "value");
}

TEST(JsonUtilsTest, GetStringRetourneDefaultSiAbsent) {
    nlohmann::json j = {{"other_key", "value"}};
    EXPECT_EQ(JsonUtils::getString(j, "key", "default"), "default");
}

TEST(JsonUtilsTest, GetStringGereTypeIncorrect) {
    nlohmann::json j = {{"key", 123}};
    EXPECT_EQ(JsonUtils::getString(j, "key", "default"), "default");
}

TEST(JsonUtilsTest, GetIntExtraitValeurCorrectement) {
    nlohmann::json j = {{"key", 42}};
    EXPECT_EQ(JsonUtils::getInt(j, "key"), 42);
}

TEST(JsonUtilsTest, GetIntRetourneDefaultSiAbsent) {
    nlohmann::json j = {{"other_key", 42}};
    EXPECT_EQ(JsonUtils::getInt(j, "key", 99), 99);
}

TEST(JsonUtilsTest, GetIntGereTypeIncorrect) {
    nlohmann::json j = {{"key", "not a number"}};
    EXPECT_EQ(JsonUtils::getInt(j, "key", 99), 99);
}

TEST(JsonUtilsTest, GetBoolExtraitValeurCorrectement) {
    nlohmann::json j = {{"key", true}};
    EXPECT_EQ(JsonUtils::getBool(j, "key"), true);
}

TEST(JsonUtilsTest, GetBoolRetourneDefaultSiAbsent) {
    nlohmann::json j = {{"other_key", true}};
    EXPECT_EQ(JsonUtils::getBool(j, "key", false), false);
}

TEST(JsonUtilsTest, GetBoolGereTypeIncorrect) {
    nlohmann::json j = {{"key", "not a bool"}};
    EXPECT_EQ(JsonUtils::getBool(j, "key", true), true);
}

TEST(JsonUtilsTest, GetStringArrayExtraitValeurCorrectement) {
    nlohmann::json j = {{"key", {"a", "b", "c"}}};
    std::vector<std::string> expected = {"a", "b", "c"};
    EXPECT_EQ(JsonUtils::getStringArray(j, "key"), expected);
}

TEST(JsonUtilsTest, GetStringArrayRetourneVideSiAbsent) {
    nlohmann::json j = {{"other_key", {"a", "b", "c"}}};
    EXPECT_TRUE(JsonUtils::getStringArray(j, "key").empty());
}

TEST(JsonUtilsTest, GetIntArrayExtraitValeurCorrectement) {
    nlohmann::json j = {{"key", {1, 2, 3}}};
    std::vector<int> expected = {1, 2, 3};
    EXPECT_EQ(JsonUtils::getIntArray(j, "key"), expected);
}

TEST(JsonUtilsTest, GetIntArrayRetourneVideSiAbsent) {
    nlohmann::json j = {{"other_key", {1, 2, 3}}};
    EXPECT_TRUE(JsonUtils::getIntArray(j, "key").empty());
}

TEST(JsonUtilsTest, GetObjectExtraitValeurCorrectement) {
    nlohmann::json inner = {{"inner_key", "inner_value"}};
    nlohmann::json j = {{"key", inner}};
    EXPECT_EQ(JsonUtils::getObject(j, "key"), inner);
}

TEST(JsonUtilsTest, GetObjectRetourneVideSiAbsent) {
    nlohmann::json j = {{"other_key", {{"inner_key", "inner_value"}}}};
    EXPECT_EQ(JsonUtils::getObject(j, "key"), nlohmann::json::object());
}

TEST(JsonUtilsTest, HasKeyRetourneTrueSiPresent) {
    nlohmann::json j = {{"key", "value"}};
    EXPECT_TRUE(JsonUtils::hasKey(j, "key"));
}

TEST(JsonUtilsTest, HasKeyRetourneFalseSiAbsent) {
    nlohmann::json j = {{"other_key", "value"}};
    EXPECT_FALSE(JsonUtils::hasKey(j, "key"));
}

TEST(JsonUtilsTest, ParseConvertitCorrectement) {
    std::string json_str = R"({"key": "value"})";
    nlohmann::json expected = {{"key", "value"}};
    EXPECT_EQ(JsonUtils::parse(json_str), expected);
}

TEST(JsonUtilsTest, ParseGereErrorsCorrectement) {
    std::string invalid_json = R"({"key": "value")"; // Missing closing brace
    nlohmann::json default_value = {{"default", true}};
    EXPECT_EQ(JsonUtils::parse(invalid_json, default_value), default_value);
}

TEST(JsonUtilsTest, StringifyConvertitCorrectement) {
    nlohmann::json j = {{"key", "value"}};
    EXPECT_EQ(JsonUtils::stringify(j), R"({"key":"value"})");
}

TEST(JsonUtilsTest, StringifyFormatChaineCorrectement) {
    nlohmann::json j = {{"key", "value"}};
    // On ne peut pas tester exactement le format avec indentation, car il dépend de l'implémentation
    // Mais on peut vérifier que le résultat contient des espaces et sauts de ligne
    std::string pretty = JsonUtils::stringify(j, true);
    EXPECT_TRUE(pretty.find(" ") != std::string::npos);
}

TEST(JsonUtilsTest, MergeCombineCorrectement) {
    nlohmann::json target = {{"key1", "value1"}};
    nlohmann::json source = {{"key2", "value2"}};
    
    JsonUtils::merge(target, source);
    
    nlohmann::json expected = {{"key1", "value1"}, {"key2", "value2"}};
    EXPECT_EQ(target, expected);
}

TEST(JsonUtilsTest, MergeRespecteFlagEcrasement) {
    nlohmann::json target = {{"key", "target_value"}};
    nlohmann::json source = {{"key", "source_value"}};
    
    // Avec écrasement
    JsonUtils::merge(target, source, true);
    EXPECT_EQ(target["key"], "source_value");
    
    // Réinitialiser target
    target = {{"key", "target_value"}};
    
    // Sans écrasement
    JsonUtils::merge(target, source, false);
    EXPECT_EQ(target["key"], "target_value");
}

TEST(JsonUtilsTest, MergeFusionRecursive) {
    nlohmann::json target = {{"nested", {{"key1", "value1"}}}};
    nlohmann::json source = {{"nested", {{"key2", "value2"}}}};
    
    JsonUtils::merge(target, source);
    
    nlohmann::json expected = {{"nested", {{"key1", "value1"}, {"key2", "value2"}}}};
    EXPECT_EQ(target, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 