/**
 * @file main.cpp
 * @brief Point d'entrée du programme pour le jeu de devinettes avec IA
 * 
 * Ce fichier contient la fonction main qui initialise et démarre le jeu de devinettes.
 * Il gère également la récupération de la clé API et des paramètres de configuration.
 */

#include "Devinette.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <stdexcept>

/**
 * @brief Récupère la clé API depuis la variable d'environnement
 * 
 * Tente de récupérer la clé API depuis la variable d'environnement LLM_API_KEY.
 * Si la variable n'est pas définie, retourne une chaîne vide.
 * 
 * @return La clé API ou une chaîne vide si non définie
 */
std::string getApiKey() {
    const char* env_key = std::getenv("LLM_API_KEY");
    if (env_key) {
        std::cout << "Clé API récupérée depuis LLM_API_KEY: " << std::string(env_key).substr(0, 5) << "..." << std::endl;
        return std::string(env_key);
    }
    return "";  // La clé API peut être vide pour le serveur du professeur
}

/**
 * @brief Fonction principale du programme
 * 
 * Initialise le générateur de nombres aléatoires, récupère les paramètres de configuration
 * et démarre le jeu de devinettes.
 * 
 * @param argc Nombre d'arguments de la ligne de commande
 * @param argv Tableau des arguments de la ligne de commande
 * @return Code de sortie du programme (0 en cas de succès)
 */
int main(int argc, char* argv[]) {
    // Initialisation du générateur de nombres aléatoires
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Configuration par défaut
    std::string api_key = getApiKey();
    std::string model = "gpt-3.5-turbo";
    std::string server_url = "http://llmcode.math.unistra.fr:8090/v1/chat/completions";

    // Récupération des paramètres depuis la ligne de commande
    if (argc > 1) {
        api_key = argv[1];
        std::cout << "Clé API fournie via argument: " << api_key.substr(0, 5) << "..." << std::endl;
    }
    if (argc > 2) model = argv[2];
    if (argc > 3) server_url = argv[3];

    // Affichage des informations de démarrage
    std::cout << "Démarrage du jeu de devinettes avec l'IA...\n";
    std::cout << "Serveur: " << server_url << "\n";
    std::cout << "Modèle: " << model << "\n";

    try {
        // Création et démarrage du jeu
        Devinette jeu(api_key, model, server_url);
        jeu.start();
    } catch (const std::exception& e) {
        std::cerr << "Erreur critique: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Erreur inconnue détectée.\n";
        return 2;
    }

    return 0;
}