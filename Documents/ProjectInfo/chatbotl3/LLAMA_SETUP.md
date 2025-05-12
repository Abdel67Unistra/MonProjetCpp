# Configuration de Llama.cpp pour le Jeu de Devinettes

Ce guide explique comment configurer et utiliser [llama.cpp](https://github.com/ggerganov/llama.cpp) avec notre jeu de devinettes.

## Pourquoi utiliser llama.cpp?

Llama.cpp est une implémentation efficace en C++ qui permet d'exécuter des modèles de langage de grande taille (LLM) localement sur votre ordinateur. Cela présente plusieurs avantages:

- **Confidentialité**: vos conversations restent sur votre machine
- **Gratuité**: pas besoin d'acheter des crédits API
- **Stabilité**: ne dépend pas d'une connexion internet stable
- **Personnalisation**: possibilité d'ajuster de nombreux paramètres

## Prérequis

- Un ordinateur avec au moins 8 Go de RAM (16 Go recommandés)
- 10-20 Go d'espace disque libre (selon le modèle choisi)
- Système d'exploitation: Linux, macOS ou Windows

## Installation de llama.cpp

1. Clonez le dépôt llama.cpp:
```bash
git clone https://github.com/ggerganov/llama.cpp.git
cd llama.cpp
```

2. Compilez le projet:
```bash
make
```

Si vous disposez d'une carte graphique compatible CUDA, vous pouvez également compiler avec la prise en charge CUDA:
```bash
make LLAMA_CUBLAS=1
```

## Téléchargement d'un modèle

Plusieurs modèles sont compatibles avec llama.cpp. Les modèles au format GGUF sont recommandés.

Quelques suggestions:
- [Llama 2 7B](https://huggingface.co/TheBloke/Llama-2-7B-GGUF)
- [Mistral 7B](https://huggingface.co/TheBloke/Mistral-7B-v0.1-GGUF)
- [Phi-2 (plus léger)](https://huggingface.co/TheBloke/phi-2-GGUF)

Téléchargez le fichier .gguf correspondant (choisissez une variante adaptée à votre RAM, par exemple Q4_K_M pour un bon équilibre performance/qualité).

## Lancement du serveur llama.cpp

Une fois le modèle téléchargé, démarrez le serveur:

```bash
./server -m chemin/vers/votre/modele.gguf -c 2048
```

Options importantes:
- `-m`: chemin vers le fichier modèle
- `-c`: taille du contexte (en tokens, 2048 est un bon point de départ)
- `-t`: nombre de threads CPU (par défaut tous les threads disponibles)
- `-ngl`: nombre de couches à charger sur GPU (si CUDA est activé)

Par défaut, le serveur sera accessible à l'adresse `http://localhost:8080`.

## Connexion du jeu au serveur llama.cpp

Lancez notre jeu de devinettes en spécifiant l'URL du serveur llama.cpp:

```bash
./chatbot "" "mistral-7b" "http://localhost:8080/v1/chat/completions"
```

- Le premier paramètre est vide car aucune clé API n'est nécessaire
- Le deuxième paramètre est le nom du modèle (informatif uniquement)
- Le troisième paramètre est l'URL du serveur llama.cpp

## Résolution des problèmes courants

### Le serveur llama.cpp ne démarre pas
- Vérifiez que vous avez assez de RAM disponible
- Essayez un modèle plus petit ou une quantification plus agressive (Q2_K)
- Réduisez la taille du contexte avec l'option `-c`

### Les réponses sont incohérentes
- Augmentez la température (llama.cpp permet de la régler avec `-temp`)
- Essayez un modèle de meilleure qualité ou moins quantifié
- Augmentez le paramètre max_tokens dans le code (si nécessaire)

### Le jeu ne se connecte pas au serveur
- Vérifiez que l'URL du serveur est correcte (par défaut: http://localhost:8080)
- Assurez-vous que le serveur est en cours d'exécution
- Activez le mode debug dans le jeu pour voir les détails des requêtes

## Modèles recommandés pour ce jeu

Pour le jeu de devinettes, nous recommandons:
- Mistral 7B: bon équilibre taille/performance
- Llama 2 7B Chat: bien adapté aux interactions conversationnelles
- Phi-2: plus léger, fonctionne sur des machines moins puissantes

Amusez-vous bien! 