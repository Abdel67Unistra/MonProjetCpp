#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

/**
 * @class JsonUtils
 * @brief Utilitaires pour manipuler les objets JSON de manière robuste
 * 
 * Cette classe fournit des méthodes statiques pour faciliter
 * la manipulation des objets JSON avec gestion des erreurs.
 */
class JsonUtils {
public:
    /**
     * @brief Extrait une valeur string d'un objet JSON avec gestion d'erreur
     * @param j Objet JSON
     * @param key Clé à extraire
     * @param defaultValue Valeur par défaut si la clé n'existe pas
     * @return La valeur extraite ou la valeur par défaut
     */
    static std::string getString(const nlohmann::json& j, const std::string& key, const std::string& defaultValue = "");
    
    /**
     * @brief Extrait une valeur int d'un objet JSON avec gestion d'erreur
     * @param j Objet JSON
     * @param key Clé à extraire
     * @param defaultValue Valeur par défaut si la clé n'existe pas
     * @return La valeur extraite ou la valeur par défaut
     */
    static int getInt(const nlohmann::json& j, const std::string& key, int defaultValue = 0);
    
    /**
     * @brief Extrait une valeur bool d'un objet JSON avec gestion d'erreur
     * @param j Objet JSON
     * @param key Clé à extraire
     * @param defaultValue Valeur par défaut si la clé n'existe pas
     * @return La valeur extraite ou la valeur par défaut
     */
    static bool getBool(const nlohmann::json& j, const std::string& key, bool defaultValue = false);
    
    /**
     * @brief Extrait un tableau de strings d'un objet JSON avec gestion d'erreur
     * @param j Objet JSON
     * @param key Clé à extraire
     * @return Le tableau extrait ou un tableau vide
     */
    static std::vector<std::string> getStringArray(const nlohmann::json& j, const std::string& key);
    
    /**
     * @brief Extrait un tableau d'entiers d'un objet JSON avec gestion d'erreur
     * @param j Objet JSON
     * @param key Clé à extraire
     * @return Le tableau extrait ou un tableau vide
     */
    static std::vector<int> getIntArray(const nlohmann::json& j, const std::string& key);
    
    /**
     * @brief Extrait un objet JSON avec gestion d'erreur
     * @param j Objet JSON parent
     * @param key Clé à extraire
     * @return L'objet extrait ou un objet vide
     */
    static nlohmann::json getObject(const nlohmann::json& j, const std::string& key);
    
    /**
     * @brief Vérifie si une clé existe dans l'objet JSON
     * @param j Objet JSON
     * @param key Clé à vérifier
     * @return true si la clé existe, false sinon
     */
    static bool hasKey(const nlohmann::json& j, const std::string& key);
    
    /**
     * @brief Parse une chaîne JSON avec gestion des exceptions
     * @param jsonStr Chaîne JSON à parser
     * @param defaultValue Valeur par défaut en cas d'erreur
     * @return L'objet JSON parsé ou la valeur par défaut
     */
    static nlohmann::json parse(const std::string& jsonStr, const nlohmann::json& defaultValue = nlohmann::json::object());
    
    /**
     * @brief Convertit un objet JSON en chaîne formatée
     * @param j Objet JSON
     * @param pretty Formatage avec indentation si true
     * @return La chaîne JSON formatée
     */
    static std::string stringify(const nlohmann::json& j, bool pretty = false);
    
    /**
     * @brief Fusionne deux objets JSON
     * @param target Objet cible qui sera modifié
     * @param source Objet source dont les valeurs seront copiées
     * @param overwrite Si true, les valeurs existantes seront écrasées
     */
    static void merge(nlohmann::json& target, const nlohmann::json& source, bool overwrite = true);
};

#endif // JSON_UTILS_HPP 