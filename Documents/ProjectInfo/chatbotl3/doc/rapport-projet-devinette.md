# Rapport de Projet : Jeu de Devinettes avec API LLM

**Auteur**: Cheriet Abdelmalek  
**Date**: Mai 2025  
**Version**: 2.0

## Table des Matières

1. [Introduction](#introduction)
2. [Présentation du Projet](#présentation-du-projet)
   - [Objectifs](#objectifs)
   - [Fonctionnalités Implémentées](#fonctionnalités-implémentées)
3. [Architecture du Système](#architecture-du-système)
   - [Vue d'ensemble](#vue-densemble)
   - [Diagramme UML](#diagramme-uml)
4. [Implémentation Détaillée](#implémentation-détaillée)
   - [Classe Chatbot](#classe-chatbot)
   - [Classe Devinette](#classe-devinette)
   - [Communication avec l'API LLM](#communication-avec-lapi-llm)
5. [Modes de Jeu](#modes-de-jeu)
   - [Mode LLM-Pense](#mode-llm-pense)
   - [Mode Utilisateur-Pense](#mode-utilisateur-pense)
6. [Gestion des Cas d'Erreur](#gestion-des-cas-derreur)
   - [Erreurs Réseau](#erreurs-réseau)
   - [Erreurs de Requête](#erreurs-de-requête)
   - [Erreurs de Réponse](#erreurs-de-réponse)
7. [Tests et Qualité du Code](#tests-et-qualité-du-code)
   - [Stratégie de Tests](#stratégie-de-tests)
   - [Gestion de la Qualité](#gestion-de-la-qualité)
   - [Automatisation des Tests](#automatisation-des-tests)
8. [Optimisations](#optimisations)
   - [Promptes Optimisés](#promptes-optimisés)
   - [Gestion de l'Historique](#gestion-de-lhistorique)
   - [Formatage des Réponses](#formatage-des-réponses)
9. [Guide d'Utilisation](#guide-dutilisation)
   - [Prérequis](#prérequis)
   - [Compilation](#compilation)
   - [Exécution](#exécution)
10. [Conclusion et Perspectives](#conclusion-et-perspectives)
11. [Annexes](#annexes)
    - [Annexe A: Prompts Utilisés](#annexe-a-prompts-utilisés)
    - [Annexe B: Journal de Développement](#annexe-b-journal-de-développement)
    - [Annexe C: Références et Ressources](#annexe-c-références-et-ressources)

## Introduction

Ce projet implémente un jeu de devinettes interactif en C++ utilisant un modèle de langage de grande taille (LLM) via une API REST. Le jeu permet deux modes d'interaction distincts : soit le modèle LLM pense à un mot et l'utilisateur tente de le deviner par des questions oui/non, soit l'utilisateur pense à un mot et le LLM essaie de le deviner en posant des questions stratégiques.

L'implémentation repose sur une architecture modulaire, avec une séparation claire entre la logique de communication avec l'API et la gestion du jeu lui-même. Cette approche garantit une flexibilité maximale et facilite la maintenance et les évolutions futures du projet.

## Présentation du Projet

### Objectifs

Le projet vise à atteindre les objectifs suivants :

1. **Créer une interface de jeu interactive** entre l'utilisateur et un modèle LLM
2. **Implémenter deux modes de jeu distincts** (LLM-Pense et Utilisateur-Pense)
3. **Établir une communication robuste** avec l'API du serveur LLM
4. **Gérer efficacement les erreurs et les cas limites** pour assurer une expérience utilisateur fluide
5. **Démontrer une utilisation avancée des LLMs** à travers une application ludique en C++

### Fonctionnalités Implémentées

Le jeu de devinettes offre les fonctionnalités suivantes :

- **Structure modulaire** avec deux classes principales : `Chatbot` et `Devinette`
- **Communication HTTP sécurisée** via la bibliothèque CPR (C++ Requests)
- **Traitement JSON avancé** grâce à la bibliothèque nlohmann/json
- **Deux modes de jeu complets** :
  - Mode où le LLM choisit un mot et l'utilisateur doit le deviner
  - Mode où l'utilisateur pense à un mot et le LLM tente de le deviner
- **Gestion robuste des erreurs** avec messages explicites et stratégies de récupération
- **Formatage intelligent des réponses** pour garantir une expérience utilisateur cohérente
- **Système de session** avec identifiants uniques pour chaque partie
- **Mode verbeux** pour le débogage et le développement

## Architecture du Système

### Vue d'ensemble

L'architecture du système repose sur une approche modulaire à deux niveaux :

1. **Couche de Communication (Classe Chatbot)** :
   - Gère toutes les interactions avec l'API du serveur LLM
   - Encapsule les détails de la communication HTTP et du traitement des réponses
   - Maintient l'état de la session et l'historique des conversations

2. **Couche de Logique de Jeu (Classe Devinette)** :
   - Implémente les règles du jeu et la logique métier
   - Gère l'interface utilisateur et les interactions
   - Utilise la classe Chatbot pour communiquer avec le LLM

Cette séparation des préoccupations garantit une base de code maintenable et extensible.

### Diagramme UML

```
+-------------------+       +-------------------+
|     Chatbot       |<------|     Devinette     |
+-------------------+       +-------------------+
| - api_key: string |       | - chatbot: Chatbot|
| - model: string   |       | - game_mode: int  |
| - server_url: string|     | - debug_mode: bool|
| - session_id: string|     +-------------------+
| - context: string |       | + start(): void   |
| - last_error: string|     | + playerGuessing()|
| - rng: mt19937    |       | + aiGuessing()    |
| - history: vector<string>| | + showMenu()     |
| - message_history: json|  | + setGameMode()   |
+-------------------+       | + testConnection()|
| + Chatbot(...)    |       | + displayHelp()   |
| + testConnection()|       | + toggleDebugMode()|
| + reset(): void   |       +-------------------+
| + ask(): string   |
| + thinkOfWord()   |
| + setUserThinking()|
| + guess(): string |
| - sendRequest()   |
| - escapeString()  |
| - formatResponse()|
+-------------------+
```

## Implémentation Détaillée

### Classe Chatbot

La classe `Chatbot` est le cœur du système de communication avec l'API LLM. Elle encapsule toute la logique nécessaire pour envoyer des requêtes, interpréter les réponses et maintenir le contexte de la conversation.

```cpp
class Chatbot {
private:
    std::string api_key;
    std::string model;
    std::string server_url;
    std::string session_id;
    std::string context;
    std::string last_error;
    std::mt19937 rng;
    std::vector<std::string> history;
    nlohmann::json message_history;

    // Méthodes privées
    std::string sendRequest(const std::string& prompt);
    std::string escapeString(const std::string& input);
    std::string formatResponse(const std::string& response);

public:
    // Constructeur
    Chatbot(const std::string& api_key, const std::string& model, const std::string& server_url);
    
    // Méthodes principales
    bool testConnection();
    void reset();
    std::string ask(const std::string& question);
    void thinkOfWord();
    void setUserThinking();
    std::string guess(const std::string& last_answer);
};
```

#### Méthodes clés de la classe Chatbot

- **`Chatbot(api_key, model, server_url)`** : Initialise une nouvelle instance avec les paramètres de connexion.
- **`testConnection()`** : Vérifie la connexion à l'API en envoyant une requête de test.
- **`reset()`** : Réinitialise le contexte et l'historique, générant une nouvelle session.
- **`ask(question)`** : Envoie une question au LLM et retourne sa réponse.
- **`thinkOfWord()`** : Configure le LLM pour choisir un mot à deviner (mode LLM-Pense).
- **`setUserThinking()`** : Configure le LLM pour qu'il devine le mot de l'utilisateur (mode Utilisateur-Pense).
- **`guess(last_answer)`** : Génère une nouvelle question ou suggestion basée sur la réponse précédente.

#### Méthodes privées importantes

- **`sendRequest(prompt)`** : Méthode centrale qui gère l'envoi de requêtes HTTP à l'API et le traitement des réponses.
- **`formatResponse(response)`** : Nettoie et standardise les réponses du LLM pour garantir une expérience cohérente.
- **`escapeString(input)`** : Échappe les caractères spéciaux pour le traitement JSON.

### Classe Devinette

La classe `Devinette` implémente la logique du jeu et gère l'interface utilisateur. Elle utilise une instance de `Chatbot` pour communiquer avec le LLM.

```cpp
class Devinette {
private:
    Chatbot chatbot;
    int game_mode;
    bool debug_mode;
    
    // Méthodes privées
    bool isValidQuestion(const std::string& question);
    int getIntInput(int min, int max);

public:
    // Constructeur
    Devinette(const std::string& api_key, const std::string& model, const std::string& server_url);
    
    // Méthodes principales
    void start();
    void showMenu();
    void playerGuessingRound();
    void aiGuessingRound();
    void setGameMode(int mode);
    bool testConnection();
    void displayHelp();
    void toggleDebugMode();
};
```

#### Méthodes clés de la classe Devinette

- **`start()`** : Point d'entrée principal du jeu, affiche le menu et gère la boucle principale.
- **`showMenu()`** : Affiche le menu principal et traite les choix de l'utilisateur.
- **`playerGuessingRound()`** : Implémente une partie où l'utilisateur devine le mot du LLM.
- **`aiGuessingRound()`** : Implémente une partie où le LLM devine le mot de l'utilisateur.
- **`testConnection()`** : Vérifie la connexion à l'API avant de commencer le jeu.
- **`displayHelp()`** : Affiche les instructions et règles du jeu.
- **`toggleDebugMode()`** : Active/désactive le mode de débogage pour afficher des informations supplémentaires.

### Communication avec l'API LLM

La communication avec l'API LLM est réalisée à travers la bibliothèque CPR (C++ Requests), qui offre une interface moderne et intuitive pour les requêtes HTTP. Le projet utilise les fonctionnalités suivantes de CPR :

- **Requêtes POST** avec corps JSON pour envoyer des prompts au LLM
- **Headers d'authentification** pour l'API key via Bearer token
- **Gestion des timeout** pour éviter les blocages en cas de problèmes de réseau
- **Mode verbeux** pour le débogage des requêtes et réponses

Exemple d'implémentation d'une requête à l'API dans la méthode `sendRequest()` :

```cpp
cpr::Response r = cpr::Post(
    cpr::Url{server_url},
    cpr::Header{
        {"Content-Type", "application/json"},
        {"Accept", "application/json"},
        {"Authorization", "Bearer " + api_key}
    },
    cpr::Body{parameters.dump()},
    cpr::Timeout{60000}, // 60 secondes
    cpr::Verbose{} // Mode verbeux pour le débogage
);
```

Le traitement des réponses est effectué à l'aide de la bibliothèque nlohmann/json, qui permet de manipuler facilement les structures JSON :

```cpp
json response = json::parse(r.text);
if (response.contains("choices") && !response["choices"].empty()) {
    std::string text = response["choices"][0]["message"]["content"].get<std::string>();
    return formatResponse(text);
}
```

## Modes de Jeu

### Mode LLM-Pense

Dans ce mode, le LLM choisit secrètement un mot et l'utilisateur doit le deviner en posant des questions auxquelles le LLM répond par "oui" ou "non".

#### Initialisation

```cpp
void Chatbot::thinkOfWord() {
    context = "Tu es un assistant pour un jeu de devinettes. Choisis un mot simple du quotidien (nom commun concret) et réponds uniquement par 'oui', 'non' ou 'Bravo, tu as trouvé !' aux questions posées.";
    
    // Réinitialiser l'historique des messages
    message_history.clear();
    
    // Ajouter le message système
    message_history.push_back({
        {"role", "system"},
        {"content", context}
    });
    
    // Ajouter la demande de choisir un mot
    message_history.push_back({
        {"role", "user"},
        {"content", "Choisis un mot et réponds simplement 'J'ai choisi un mot.'"}
    });
    
    // Envoyer la requête et traiter la réponse...
}
```

#### Déroulement du jeu

1. L'utilisateur pose une question par tour.
2. La question est envoyée au LLM via la méthode `ask()`.
3. Le LLM répond par "oui", "non" ou indique que l'utilisateur a trouvé le mot.
4. Le jeu continue jusqu'à ce que l'utilisateur trouve le mot ou abandonne.

### Mode Utilisateur-Pense

Dans ce mode, l'utilisateur pense à un mot et le LLM tente de le deviner en posant des questions stratégiques auxquelles l'utilisateur répond par "oui" ou "non".

#### Initialisation

```cpp
void Chatbot::setUserThinking() {
    context = "L'utilisateur a choisi un mot. Pose des questions oui/non stratégiques pour deviner le mot, ou propose une réponse si tu penses l'avoir trouvé. Ne répète pas les questions déjà posées. Assure-toi que chaque nouvelle question réduit efficacement l'ensemble des possibilités.";
    
    // Réinitialiser l'historique des messages
    message_history.clear();
    
    // Ajouter le message système
    message_history.push_back({
        {"role", "system"},
        {"content", context}
    });
    
    history.clear();
}
```

#### Déroulement du jeu

1. Le LLM pose une question via la méthode `guess()`.
2. L'utilisateur répond par "oui" ou "non".
3. La réponse est envoyée au LLM qui formule une nouvelle question ou propose une solution.
4. Le jeu continue jusqu'à ce que le LLM trouve le mot ou abandonne.

## Gestion des Cas d'Erreur

Le projet implémente une gestion robuste des erreurs à plusieurs niveaux pour garantir une expérience utilisateur fluide même en cas de problèmes.

### Erreurs Réseau

Les erreurs réseau (timeout, connexion refusée, etc.) sont capturées et traitées de manière explicite :

```cpp
if (r.error.code == cpr::ErrorCode::OPERATION_TIMEDOUT) {
    last_error = "Erreur: délai d'attente dépassé pour la connexion au serveur.";
    std::cerr << last_error << std::endl;
    return "Erreur: le serveur met trop de temps à répondre.";
}
```

### Erreurs de Requête

Les codes d'erreur HTTP sont analysés pour fournir des messages d'erreur pertinents :

```cpp
if (r.status_code != 200) {
    last_error = "Erreur HTTP " + std::to_string(r.status_code) + ": " + r.text + " - " + r.error.message;
    if (r.status_code == 429) last_error += " (limite de requêtes dépassée)";
    else if (r.status_code == 401) last_error += " (authentification invalide)";
    else if (r.status_code == 0) last_error += " (erreur réseau ou serveur inaccessible)";
    std::cerr << last_error << std::endl;
    return "Erreur: impossible de contacter l'API.";
}
```

### Erreurs de Réponse

Les réponses invalides ou mal formatées sont détectées et gérées :

```cpp
try {
    json response = json::parse(r.text);
    if (response.contains("choices") && !response["choices"].empty()) {
        std::string text = response["choices"][0]["message"]["content"].get<std::string>();
        return formatResponse(text);
    }

    last_error = "Réponse invalide de l'API: " + r.text;
    std::cerr << last_error << std::endl;
    return "Erreur: réponse invalide.";
} catch (const std::exception& e) {
    last_error = "Erreur de traitement de la réponse: " + std::string(e.what());
    std::cerr << last_error << std::endl;
    return "Erreur: problème lors de la requête.";
}
```

## Tests et Qualité du Code

### Stratégie de Tests

Pour garantir la qualité et la fiabilité du projet, une stratégie de tests complète a été mise en place. Cette stratégie repose sur plusieurs niveaux de tests :

#### Tests Unitaires

Les tests unitaires vérifient le bon fonctionnement des composants individuels du système :

```cpp
TEST_F(ChatbotTest, InitialisationCorrect) {
    EXPECT_EQ(chatbot->getModel(), "test-model");
    EXPECT_EQ(chatbot->getApiKey(), "test_key");
}

TEST_F(ChatbotTest, HistoriqueInitialVide) {
    const auto& history = chatbot->getHistory();
    EXPECT_TRUE(history.empty());
}

TEST_F(ChatbotTest, ResetEffaceHistorique) {
    chatbot->ask("test question");
    chatbot->reset();
    const auto& history = chatbot->getHistory();
    EXPECT_TRUE(history.empty());
}
```

Ces tests vérifient :
- L'initialisation correcte des objets
- La gestion de l'historique des conversations
- Le comportement du formatage des réponses
- La robustesse face aux erreurs réseau et API

#### Tests d'Intégration

Les tests d'intégration vérifient les interactions entre les différents composants du système :

```cpp
TEST_F(IntegrationTest, ChatbotDevinetteInteraction) {
    Devinette game("test_key", "test-model", "http://localhost:8080");
    EXPECT_EQ(game.getGameState(), GameState::INIT);
    
    // Simulation d'un jeu
    game.preparePlayerGuessingRound();
    EXPECT_EQ(game.getGameState(), GameState::PLAYER_GUESSING);
}
```

Ces tests s'assurent que :
- Les classes `Chatbot` et `Devinette` interagissent correctement
- Les transitions d'état du jeu fonctionnent comme prévu
- Les scénarios de jeu complets aboutissent aux résultats attendus

### Gestion de la Qualité

Plusieurs pratiques ont été mises en place pour assurer la qualité du code :

- **Compilation sans avertissements** : Le code est compilé avec les options `-Wall -Wextra` pour détecter les problèmes potentiels
- **Vérification de la mémoire** : Utilisation de Valgrind pour détecter les fuites de mémoire
- **Analyse statique** : Utilisation d'outils comme Clang-Tidy pour améliorer la qualité du code
- **Revue de code** : Révision régulière du code pour identifier les optimisations possibles

### Automatisation des Tests

Un système d'automatisation des tests a été mis en place via CMake, permettant d'exécuter facilement l'ensemble des tests :

```bash
cd build
make check
```

Cette commande exécute tous les tests et rapporte les éventuels échecs, facilitant ainsi la détection rapide des régressions.

## Optimisations

### Promptes Optimisés

Les prompts utilisés pour interagir avec le LLM ont été soigneusement conçus pour maximiser la qualité des réponses :

```cpp
// Pour le mode LLM-Pense
context = "Tu es un assistant pour un jeu de devinettes. Choisis un mot simple du quotidien (nom commun concret) et réponds uniquement par 'oui', 'non' ou 'Bravo, tu as trouvé !' aux questions posées.";

// Pour le mode Utilisateur-Pense
context = "L'utilisateur a choisi un mot. Pose des questions oui/non stratégiques pour deviner le mot, ou propose une réponse si tu penses l'avoir trouvé. Ne répète pas les questions déjà posées. Assure-toi que chaque nouvelle question réduit efficacement l'ensemble des possibilités.";
```

Ces prompts sont précis et directifs, ce qui aide le LLM à maintenir le contexte et à fournir des réponses cohérentes.

### Gestion de l'Historique

L'historique des conversations est maintenu de deux manières complémentaires :

1. **Un historique simple** sous forme de vecteur de chaînes de caractères pour un affichage facile et un débogage :

```cpp
history.push_back("Utilisateur: " + question + "\nIA: " + response);
```

2. **Un historique structuré** au format JSON conforme à l'API pour maintenir le contexte des conversations :

```cpp
message_history.push_back({
    {"role", "user"},
    {"content", prompt}
});

// Puis après réception de la réponse
message_history.push_back({
    {"role", "assistant"},
    {"content", response}
});
```

Cette double approche permet à la fois une interaction fluide avec l'API et un suivi facile des conversations pour l'utilisateur.

### Formatage des Réponses

Les réponses du LLM sont traitées par une fonction de formatage qui garantit une expérience utilisateur cohérente :

```cpp
std::string Chatbot::formatResponse(const std::string& response) {
    std::string formatted = response;
    formatted.erase(std::remove(formatted.begin(), formatted.end(), '\r'), formatted.end());
    formatted = formatted.substr(formatted.find_first_not_of(" \n\t"));
    formatted = formatted.substr(0, formatted.find_last_not_of(" \n\t") + 1);

    std::string lower = formatted;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "oui" || lower == "non" || 
        lower.find("bravo") != std::string::npos || 
        lower.find("trouvé") != std::string::npos ||
        lower.find("correct") != std::string::npos) {
        formatted.erase(std::remove(formatted.begin(), formatted.end(), '\n'), formatted.end());
    }
    return formatted.empty() ? "Erreur: réponse vide." : formatted;
}
```

Cette fonction :
- Supprime les caractères de retour chariot
- Élimine les espaces, tabulations et sauts de ligne superflus
- Simplifie les réponses courtes (oui/non/bravo)
- Détecte les réponses vides et les remplace par un message d'erreur

## Guide d'Utilisation

### Prérequis

- Compilateur C++ compatible C++17 ou plus récent
- CMake 3.10 ou plus récent
- Bibliothèques:
  - CPR (C++ Requests)
  - nlohmann/json
  - Une clé API valide pour accéder au serveur LLM

### Compilation

1. Cloner le dépôt et accéder au répertoire du projet:

```bash
git clone https://gitlab.univ-lr.fr/[votre-compte]/chatbot-cpp.git
cd chatbot-cpp
```

2. Créer un répertoire de build et générer les fichiers de compilation:

```bash
mkdir build
cd build
cmake ..
```

3. Compiler le projet:

```bash
make
```

### Exécution

1. Exécuter le programme principal:

```bash
./chatbot-cpp [clé_API] [modèle] [serveur_url]
```

Arguments:
- `clé_API`: Votre clé API pour le serveur LLM (obligatoire)
- `modèle`: Le modèle à utiliser (facultatif, par défaut: "mistral-large")
- `serveur_url`: L'URL du serveur LLM (facultatif, par défaut: "https://api.mistral.ai/v1/chat/completions")

2. Utiliser le menu interactif pour choisir un mode de jeu et commencer à jouer.

## Conclusion et Perspectives

Ce projet démontre l'intégration réussie d'une API LLM dans une application C++ interactive. Le jeu de devinettes offre une expérience utilisateur engageante tout en illustrant les capacités des modèles de langage modernes.

Les points forts du projet incluent:
- Une architecture modulaire bien conçue avec une séparation claire des responsabilités
- Une gestion robuste des erreurs à tous les niveaux
- Une optimisation des prompts pour obtenir des réponses cohérentes et pertinentes
- Une expérience utilisateur fluide malgré les contraintes inhérentes à l'interaction avec une API distante

### Perspectives d'amélioration

Plusieurs pistes d'amélioration pourraient être explorées à l'avenir:

1. **Interface graphique** : Développer une interface graphique avec une bibliothèque comme Qt ou ImGui
2. **Mode multijoueur** : Permettre à plusieurs utilisateurs de jouer simultanément
3. **Niveaux de difficulté** : Ajouter différents niveaux de difficulté avec des catégories de mots spécifiques
4. **Statistiques de jeu** : Implémenter un système de suivi des performances pour analyser les stratégies
5. **Cache local** : Mettre en cache certaines réponses pour réduire la dépendance au réseau
6. **Support multilingue** : Étendre le jeu à d'autres langues en modifiant les prompts

## Gestion de la Fin de Partie et Détection de Victoire

### Problème Initial
Le système ne détectait pas correctement la fin de partie lorsque le joueur trouvait le mot, et continuait à accepter des questions même après une victoire.

### Solution Implémentée

#### 1. Détection de Victoire
- Ajout d'un signal spécial "VICTOIRE" dans la réponse de l'IA
- Vérification de multiples variations de réponses positives ("bravo", "trouvé", "gagné", "correct")
- Marquage immédiat de la fin de partie lors de la détection d'une victoire

#### 2. Gestion de l'État de la Partie
- Implémentation d'une classe `Devinette` pour gérer l'état de la partie
- Suivi du nombre de tentatives restantes
- Gestion des messages de victoire et de défaite

#### 3. Amélioration de l'Interface Utilisateur
- Affichage clair de l'état de la partie
- Messages de victoire et de défaite formatés
- Compteur de tentatives utilisées

### Améliorations Apportées

#### 1. Détection de Victoire
```cpp
if (answer == "VICTOIRE") {
    std::cout << "Nombre de tentatives utilisées : " << (attempts + 1) << std::endl;
    return;  // Terminer la partie
}
```

#### 2. Gestion des Tentatives
```cpp
while (attempts < maxAttempts) {
    std::cout << "Vous avez " << (maxAttempts - attempts) << " tentatives.\n\n";
    // ... reste du code ...
}
```

#### 3. Messages de Fin de Partie
```cpp
// Victoire
std::cout << "🎉 Félicitations! Vous avez trouvé le mot!\n";

// Défaite
std::cout << "\n❌ Game Over ! Vous avez épuisé toutes vos tentatives.\n";
std::cout << "Le mot était : " << chatbot.getChosenWord() << std::endl;
```

### Perspectives d'Amélioration
1. Ajout de statistiques de jeu
2. Implémentation d'animations de fin
3. Possibilité de partager son score
4. Système de classement des meilleurs scores

## Annexes

### Annexe A: Prompts Utilisés

#### Prompt Système pour le Mode LLM-Pense

```
Tu es un assistant pour un jeu de devinettes. Choisis un mot simple du quotidien (nom commun concret) et réponds uniquement par 'oui', 'non' ou 'Bravo, tu as trouvé !' aux questions posées.
```

#### Prompt Utilisateur Initial pour le Mode LLM-Pense

```
Choisis un mot et réponds simplement 'J'ai choisi un mot.'
```

#### Prompt Système pour le Mode Utilisateur-Pense

```
L'utilisateur a choisi un mot. Pose des questions oui/non stratégiques pour deviner le mot, ou propose une réponse si tu penses l'avoir trouvé. Ne répète pas les questions déjà posées. Assure-toi que chaque nouvelle question réduit efficacement l'ensemble des possibilités.
```

#### Prompt pour Générer une Question dans le Mode Utilisateur-Pense

```
Basé sur les réponses précédentes, pose une nouvelle question oui/non stratégique qui n'a pas encore été posée, ou propose une réponse si tu es sûr du mot. Réponds uniquement avec la question ou la proposition.
```

### Annexe B: Journal de Développement

#### Phase 1: Conception et Préparation (Semaine 1)
- Analyse des besoins du projet
- Étude de l'API LLM et de ses capacités
- Choix des bibliothèques (CPR, nlohmann/json)
- Conception de l'architecture du système

#### Phase 2: Implémentation de Base (Semaine 2)
- Création de la classe Chatbot
- Implémentation des requêtes HTTP avec CPR
- Mise en place du système de gestion des erreurs
- Tests de connexion à l'API

#### Phase 3: Logique de Jeu (Semaine 3)
- Développement de la classe Devinette
- Implémentation du mode LLM-Pense
- Création de l'interface console
- Tests d'intégration

#### Phase 4: Finalisation (Semaine 4)
- Implémentation du mode Utilisateur-Pense
- Optimisation des prompts
- Polissage de l'interface utilisateur
- Documentation complète du code
- Rédaction du rapport

### Annexe C: Références et Ressources

#### Bibliothèques Utilisées
- [CPR (C++ Requests)](https://github.com/libcpr/cpr) - Bibliothèque pour les requêtes HTTP
- [nlohmann/json](https://github.com/nlohmann/json) - Bibliothèque pour le traitement JSON
- [C++ Standard Library](https://en.cppreference.com/w/cpp/standard_library) - Bibliothèque standard C++

#### Documentation API
- [API LLM Documentation](https://docs.mistral.ai/) - Documentation officielle de l'API Mistral
- [HTTP Status Codes](https://developer.mozilla.org/en-US/docs/Web/HTTP/Status) - Référence des codes d'état HTTP

#### Ressources C++
- [C++ Best Practices](https://github.com/cpp-best-practices/cppbestpractices) - Bonnes pratiques de programmation C++
- [CMake Documentation](https://cmake.org/documentation/) - Documentation officielle de CMake