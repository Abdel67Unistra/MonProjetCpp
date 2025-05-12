# ChatbotL3 : Jeu de Devinettes avec IA 🤖

Un jeu interactif et moderne où l'IA choisit un mot et le joueur doit le deviner, ou inversement.

## ✨ Fonctionnalités

- Interface en ligne de commande avec coloration syntaxique
- Support de plusieurs catégories de mots (Fruits, Légumes, Animaux, Villes, Objets)
- Communication robuste avec les APIs de modèles de langage (LLM)
- Gestion des erreurs et reconnexion automatique
- Support pour différents formats d'API LLM (OpenAI, Llama.cpp, etc.)
- Statistiques détaillées de jeu
- Trois niveaux de difficulté
- Mode inversé où l'IA devine le mot choisi par le joueur
- Documentation complète générée par Doxygen

## 🎮 Modes de jeu

### 1. Mode Classique : Le joueur devine le mot de l'IA
- L'IA choisit un mot aléatoire dans une catégorie
- Le joueur a un nombre limité de questions selon la difficulté (8, 12 ou 16)
- L'IA répond uniquement par "Oui" ou "Non"
- Le joueur peut proposer directement un mot pour le deviner
- Interface colorée pour une meilleure lisibilité

### 2. Mode Inversé : L'IA devine le mot du joueur
- Le joueur pense à un mot (de préférence un nom commun concret)
- L'IA pose des questions stratégiques générées dynamiquement
- Le joueur répond par "oui", "non" ou "trouve"
- L'IA analyse l'historique complet pour affiner sa stratégie
- Algorithme amélioré pour des questions plus pertinentes

## 🔧 Architecture du projet

Le projet suit une architecture modulaire à plusieurs couches :

### Couche de communication
- `ApiClient` : Gestion robuste des requêtes HTTP avec retry et timeouts
- Abstraction des différents formats d'API (OpenAI, llama.cpp)

### Couche métier
- `Chatbot` : Logique d'interaction avec les LLM
- `Devinette` : Règles et logique du jeu

### Couche utilitaire
- `JsonUtils` : Manipulation robuste de JSON
- `ColorTerminal` : Interface utilisateur colorée

## 🚀 Installation

### Prérequis

- C++17 ou supérieur
- CMake 3.10 ou supérieur
- Bibliothèques :
  - libcpr (pour les requêtes HTTP)
  - nlohmann-json (pour le traitement JSON)
  - GTest (pour les tests, optionnel)

### Compilation

```bash
# Cloner le dépôt
git clone https://gitlab.com/votre-utilisateur/chatbotl3.git
cd chatbotl3

# Créer un dossier de build
mkdir build && cd build

# Configurer avec CMake
cmake ..

# Compiler
make

# Exécuter les tests (optionnel)
make check

# Générer la documentation (optionnel, requiert Doxygen)
make doc
```

## 🖥️ Utilisation

1. Lancer le jeu :
```bash
./chatbot
```

2. Pour utiliser une clé API spécifique :
```bash
export LLM_API_KEY="votre_clé_api"
./chatbot
```

3. Pour utiliser un serveur LLM spécifique :
```bash
./chatbot "" "nom_du_modele" "http://adresse_du_serveur/chemin"
```

4. Pour utiliser llama.cpp en local, consultez le guide [LLAMA_SETUP.md](LLAMA_SETUP.md).

## 📋 Options de compilation

Le projet propose plusieurs options de compilation via CMake :

```bash
# Activer les avertissements de compilation (ON par défaut)
cmake -DCHATBOT_ENABLE_WARNINGS=ON ..

# Activer les sanitizers en mode debug
cmake -DCHATBOT_ENABLE_SANITIZERS=ON -DCMAKE_BUILD_TYPE=Debug ..

# Désactiver la construction des tests
cmake -DCHATBOT_BUILD_TESTS=OFF ..
```

## 📚 Documentation

Une documentation complète est disponible grâce à Doxygen. Pour la générer :

```bash
make doc
```

La documentation sera disponible dans `build/doc/html/index.html`.

## 🧪 Tests

Le projet inclut une suite de tests unitaires et d'intégration pour garantir sa qualité :

```bash
# Exécuter tous les tests
make check

# Exécuter uniquement les tests de la classe Chatbot
./test_chatbot

# Exécuter uniquement les tests de la classe ApiClient
./test_api_client

# Exécuter uniquement les tests de la classe JsonUtils
./test_json_utils
```

## 📦 Packaging

Pour créer un package d'installation (nécessite CMake) :

```bash
make package
```

## 📝 Licence

Ce projet est distribué sous la licence MIT. Voir le fichier [LICENSE](LICENSE) pour plus de détails.

## 🤝 Contribution

Les contributions sont les bienvenues ! N'hésitez pas à :
1. Fork le projet
2. Créer une branche (`git checkout -b feature/nouvelle-fonctionnalite`)
3. Commit vos changements (`git commit -m 'Ajout d'une nouvelle fonctionnalité'`)
4. Push vers la branche (`git push origin feature/nouvelle-fonctionnalite`)
5. Ouvrir une Pull Request

## 📧 Contact

Pour toute question, vous pouvez contacter [votre-email@example.com].

## 🔄 Dernières améliorations

1. **Architecture modulaire**
   - Refactoring complet pour une meilleure séparation des responsabilités
   - Implémentation du pattern Factory pour la création des objets

2. **Interface utilisateur améliorée**
   - Ajout de couleurs pour une meilleure lisibilité
   - Messages d'erreur plus clairs et informatifs

3. **Gestion robuste des erreurs**
   - Mécanisme de retry pour les requêtes réseau
   - Validation des entrées utilisateur

4. **Tests unitaires complets**
   - Couverture de code > 80%
   - Tests d'intégration avec des mocks pour les APIs externes

5. **Documentation complète**
   - Documentation de l'API avec Doxygen
   - Guides d'utilisation détaillés

6. **Optimisations de performance**
   - Réduction des appels API inutiles
   - Mise en cache des résultats fréquemment utilisés 