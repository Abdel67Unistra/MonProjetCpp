#include "JsonUtils.hpp"
#include <iostream>

std::string JsonUtils::getString(const nlohmann::json& j, const std::string& key, const std::string& defaultValue) {
    try {
        if (j.contains(key) && j[key].is_string()) {
            return j[key].get<std::string>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'extraction de la clé string '" << key << "': " << e.what() << std::endl;
    }
    return defaultValue;
}

int JsonUtils::getInt(const nlohmann::json& j, const std::string& key, int defaultValue) {
    try {
        if (j.contains(key) && j[key].is_number()) {
            return j[key].get<int>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'extraction de la clé int '" << key << "': " << e.what() << std::endl;
    }
    return defaultValue;
}

bool JsonUtils::getBool(const nlohmann::json& j, const std::string& key, bool defaultValue) {
    try {
        if (j.contains(key) && j[key].is_boolean()) {
            return j[key].get<bool>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'extraction de la clé bool '" << key << "': " << e.what() << std::endl;
    }
    return defaultValue;
}

std::vector<std::string> JsonUtils::getStringArray(const nlohmann::json& j, const std::string& key) {
    std::vector<std::string> result;
    try {
        if (j.contains(key) && j[key].is_array()) {
            for (const auto& item : j[key]) {
                if (item.is_string()) {
                    result.push_back(item.get<std::string>());
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'extraction du tableau de strings '" << key << "': " << e.what() << std::endl;
    }
    return result;
}

std::vector<int> JsonUtils::getIntArray(const nlohmann::json& j, const std::string& key) {
    std::vector<int> result;
    try {
        if (j.contains(key) && j[key].is_array()) {
            for (const auto& item : j[key]) {
                if (item.is_number()) {
                    result.push_back(item.get<int>());
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'extraction du tableau d'entiers '" << key << "': " << e.what() << std::endl;
    }
    return result;
}

nlohmann::json JsonUtils::getObject(const nlohmann::json& j, const std::string& key) {
    try {
        if (j.contains(key) && j[key].is_object()) {
            return j[key];
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'extraction de l'objet '" << key << "': " << e.what() << std::endl;
    }
    return nlohmann::json::object();
}

bool JsonUtils::hasKey(const nlohmann::json& j, const std::string& key) {
    try {
        return j.contains(key);
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la vérification de l'existence de la clé '" << key << "': " << e.what() << std::endl;
        return false;
    }
}

nlohmann::json JsonUtils::parse(const std::string& jsonStr, const nlohmann::json& defaultValue) {
    try {
        if (jsonStr.empty()) {
            return defaultValue;
        }
        return nlohmann::json::parse(jsonStr);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Erreur lors du parsing JSON: " << e.what() << std::endl;
        return defaultValue;
    } catch (const std::exception& e) {
        std::cerr << "Erreur générique lors du parsing JSON: " << e.what() << std::endl;
        return defaultValue;
    }
}

std::string JsonUtils::stringify(const nlohmann::json& j, bool pretty) {
    try {
        if (pretty) {
            return j.dump(4); // Avec indentation de 4 espaces
        } else {
            return j.dump();
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la conversion JSON vers string: " << e.what() << std::endl;
        return "{}";
    }
}

void JsonUtils::merge(nlohmann::json& target, const nlohmann::json& source, bool overwrite) {
    try {
        if (!source.is_object()) {
            return;
        }
        
        if (!target.is_object()) {
            target = nlohmann::json::object();
        }
        
        for (auto it = source.begin(); it != source.end(); ++it) {
            // Si la clé n'existe pas dans la cible ou si on veut écraser
            if (!target.contains(it.key()) || overwrite) {
                if (it.value().is_object() && target.contains(it.key()) && target[it.key()].is_object()) {
                    // Fusion récursive des sous-objets
                    merge(target[it.key()], it.value(), overwrite);
                } else {
                    // Copie directe de la valeur
                    target[it.key()] = it.value();
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la fusion des objets JSON: " << e.what() << std::endl;
    }
} 