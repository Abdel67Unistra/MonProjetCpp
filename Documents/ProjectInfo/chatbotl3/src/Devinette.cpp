#include "Devinette.hpp"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <thread>
#include <chrono>
#include <iomanip>
#include <set>

void pause(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Devinette::Devinette(const std::string& api_key, const std::string& model_name,
                     const std::string& server_url)
    : game_state(GameState::INIT),
      difficulty_level(1),
      score_player(0),
      score_ai(0),
      max_attempts_player(16),
      max_attempts_ai(10),
      debug_mode(false),
      chatbot(api_key, model_name.empty() ? "gpt-3.5-turbo" : model_name, server_url, false) {
    std::cout << "Initialisation du jeu de devinettes..." << std::endl;
}

void Devinette::start() {
    clearScreen();
    displayWelcome();

    bool continue_playing = true;
    while (continue_playing) {
        switch (game_state) {
            case GameState::INIT:
                showMenu();
                break;
            case GameState::PLAYER_GUESSING:
                playerGuessingRound();
                break;
            case GameState::AI_GUESSING:
                aiGuessingRound();
                break;
            case GameState::GAME_OVER:
                continue_playing = askPlayAgain();
                break;
        }
    }

    displayGoodbye();
}

void Devinette::displayWelcome() {
    std::cout << "\n🎮 ================================================= 🎮" << std::endl;
    std::cout << "🎲       JEU DE DEVINETTES AVEC IA       🎲" << std::endl;
    std::cout << "🎮 ================================================= 🎮\n" << std::endl;

    std::cout << "🔹 Mode 1: Devinez le mot auquel l'IA pense" << std::endl;
    std::cout << "🔹 Mode 2: L'IA devine le mot auquel vous pensez\n" << std::endl;

    std::cout << "💡 Comment jouer:" << std::endl;
    std::cout << "   - Posez des questions qui se répondent par oui/non" << std::endl;
    std::cout << "   - Quand vous pensez avoir trouvé, proposez directement le mot" << std::endl;
    std::cout << "   - Tapez 'abandonner' à tout moment pour revenir au menu\n" << std::endl;

    std::cout << "📊 Difficulté actuelle: " << difficulty_level << " ("
              << (difficulty_level == 1 ? "Facile" : (difficulty_level == 2 ? "Normal" : "Difficile"))
              << ")" << std::endl;

    std::cout << "\nAppuyez sur Entrée pour continuer..." << std::endl;
    std::cin.get();
    clearScreen();
}

void Devinette::showMenu() {
    clearScreen();
    std::cout << "\n🎮 --- Menu Principal --- 🎮" << std::endl;
    std::cout << "1. Deviner le mot de l'IA" << std::endl;
    std::cout << "2. L'IA devine mon mot" << std::endl;
    std::cout << "3. Difficulté (actuelle: " << difficulty_level << " - "
              << (difficulty_level == 1 ? "Facile" : (difficulty_level == 2 ? "Normal" : "Difficile"))
              << ")" << std::endl;
    std::cout << "4. Score" << std::endl;
    std::cout << "5. Historique" << std::endl;
    std::cout << "6. Aide" << std::endl;
    std::cout << (debug_mode ? "7. Désactiver le mode débogage" : "7. Activer le mode débogage") << std::endl;
    std::cout << "8. Quitter" << std::endl;
    std::cout << "\nChoix: ";

    int choice = getIntInput(1, 8);

    switch (choice) {
        case 1:
            game_state = GameState::PLAYER_GUESSING;
            preparePlayerGuessingRound();
            break;
        case 2:
            game_state = GameState::AI_GUESSING;
            prepareAIGuessingRound();
            break;
        case 3:
            setDifficulty();
            break;
        case 4:
            displayScore();
            break;
        case 5:
            displayHistory();
            break;
        case 6:
            displayHelp();
            break;
        case 7:
            toggleDebugMode();
            break;
        case 8:
            game_state = GameState::GAME_OVER;
            break;
    }
}

void Devinette::preparePlayerGuessingRound() {
    clearScreen();
    std::cout << "\n🎲 --- Mode: Devinez le mot de l'IA --- 🎲" << std::endl;
    std::cout << "\n⏳ L'IA est en train de penser à un mot..." << std::endl;

    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush();
        pause(600);
    }
}

void Devinette::playerGuessingRound() {
    // Initialiser le jeu
    chatbot.thinkOfWord();
    
    std::string word = chatbot.getChosenWord();
    std::string category = chatbot.getChosenCategory();
    
    int attempts = 0;
    bool game_won = false;
    std::string question;
    std::vector<std::string> questions;
    std::vector<std::string> answers;
    
    while (attempts < max_attempts_player && !game_won) {
        // Afficher l'historique des questions et réponses
        clearScreen();
        std::cout << "\n🎲 --- Mode: Devinez le mot de l'IA --- 🎲" << std::endl;
        
        // Afficher le mot uniquement en mode débogage
        if (debug_mode) {
            std::cout << "\nMot à deviner: " << word << std::endl;
        }
        
        std::cout << "Questions restantes: " << (max_attempts_player - attempts) << "/" << max_attempts_player << std::endl;
        
        // Afficher l'historique
        if (!questions.empty()) {
            std::cout << "\nHistorique des questions:" << std::endl;
            for (size_t i = 0; i < questions.size(); ++i) {
                std::cout << "Q" << (i + 1) << ": " << questions[i] << std::endl;
                std::cout << "R" << (i + 1) << ": " << answers[i] << std::endl;
            }
        }
        
        // Demander une nouvelle question
        std::cout << "\nQuestion " << (attempts + 1) << "/" << max_attempts_player << ": ";
        std::getline(std::cin, question);
        
        // Vérifier si l'utilisateur veut abandonner
        if (question == "abandonner") {
            std::cout << "Partie abandonnée. Le mot était: " << word << std::endl;
            pause(2000);
            game_state = GameState::INIT;
            return;
        }
        
        // Traiter la question avec l'IA
        std::string response = chatbot.askQuestion(question);
        
        // Vérifier si la réponse indique une victoire
        if (response.find("VICTOIRE") != std::string::npos) {
            clearScreen();
            std::cout << "\n🎲 --- Mode: Devinez le mot de l'IA --- 🎲" << std::endl;
            
            // Afficher l'historique complet
            if (!questions.empty()) {
                std::cout << "\nHistorique des questions:" << std::endl;
                for (size_t i = 0; i < questions.size(); ++i) {
                    std::cout << "Q" << (i + 1) << ": " << questions[i] << std::endl;
                    std::cout << "R" << (i + 1) << ": " << answers[i] << std::endl;
                }
            }
            
            // Afficher la dernière question et la réponse de victoire
            std::cout << "\nQ" << (questions.size() + 1) << ": " << question << std::endl;
            std::cout << response << std::endl;
            
            score_player++;
            game_won = true;
            pause(2000);
        } else {
            // Ajouter la question et la réponse à l'historique
            questions.push_back(question);
            answers.push_back(response);
        }
        
        attempts++;
    }
    
    if (!game_won) {
        clearScreen();
        std::cout << "\n⏰ Temps écoulé ! Le mot était: " << word << std::endl;
        score_ai++;
    }
    
    std::cout << "\nAppuyez sur Entrée pour continuer...";
    std::cin.get();
    game_state = GameState::INIT;
}

void Devinette::prepareAIGuessingRound() {
    clearScreen();
    std::cout << "\n🎲 --- Mode: L'IA devine votre mot --- 🎲" << std::endl;
    std::cout << "\n💡 Instructions:" << std::endl;
    std::cout << "1. Pensez à un mot simple du quotidien (nom commun concret)" << std::endl;
    std::cout << "2. L'IA vous posera des questions auxquelles vous répondrez par 'oui' ou 'non'" << std::endl;
    std::cout << "3. Quand l'IA propose un mot, indiquez si c'est correct en répondant 'oui'" << std::endl;
    std::cout << "4. Tapez 'abandonner' à tout moment pour revenir au menu" << std::endl;

    if (difficulty_level == 1) {
        max_attempts_ai = 15;
        std::cout << "\nMode Facile: L'IA a " << max_attempts_ai << " questions à poser." << std::endl;
    } else if (difficulty_level == 2) {
        max_attempts_ai = 12;
        std::cout << "\nMode Normal: L'IA a " << max_attempts_ai << " questions à poser." << std::endl;
    } else {
        max_attempts_ai = 8;
        std::cout << "\nMode Difficile: L'IA n'a que " << max_attempts_ai << " questions à poser." << std::endl;
    }
}

void Devinette::aiGuessingRound() {
    chatbot.setThinking();
    
    int attempts = 0;
    bool game_won = false;
    std::string user_answer;
    std::string ai_question = "Est-ce que c'est un objet concret ?"; // Première question
    std::vector<std::string> ai_questions;
    std::vector<std::string> user_answers;
    
    std::cout << "\nPensez à un mot et l'IA va essayer de le deviner." << std::endl;
    std::cout << "Répondez par 'oui', 'non', 'trouve' ou 'abandonner'." << std::endl;
    pause(1000);
    
    while (attempts < max_attempts_ai && !game_won) {
        // Afficher l'historique des questions et réponses
        clearScreen();
        std::cout << "\n🎲 --- Mode: L'IA devine votre mot --- 🎲" << std::endl;
        std::cout << "\nQuestions restantes: " << (max_attempts_ai - attempts) << "/" << max_attempts_ai << std::endl;
        
        // Afficher l'historique
        if (!ai_questions.empty()) {
            std::cout << "\nHistorique des échanges:" << std::endl;
            for (size_t i = 0; i < ai_questions.size(); ++i) {
                std::cout << "IA " << (i + 1) << ": " << ai_questions[i] << std::endl;
                std::cout << "Vous " << (i + 1) << ": " << user_answers[i] << std::endl;
            }
        }
        
        // Afficher la question actuelle de l'IA
        std::cout << "\nQuestion " << (attempts + 1) << "/" << max_attempts_ai << ": " << ai_question << std::endl;
        
        // Vérifier si l'IA propose un mot
        bool is_guess = false;
        std::string proposed_word = "";
        
        // Algorithme amélioré pour détecter les propositions de mots
        // Patterns pour détecter une proposition de mot
        std::vector<std::string> guess_patterns = {
            "Est-ce que ton mot est ",
            "Est-ce que le mot est ",
            "Est-ce que c'est le mot ",
            "Est-ce que c'est ",
            "Est-ce que ce serait ",
            "Ton mot est-il ",
            "Le mot est-il ",
            "S'agit-il de ",
            "S'agit-il du mot ",
            "Je pense que c'est "
        };
        
        // Rechercher les patterns dans la question
        for (const auto& pattern : guess_patterns) {
            size_t pos = ai_question.find(pattern);
            if (pos != std::string::npos) {
                is_guess = true;
                // Extraire le mot proposé
                proposed_word = ai_question.substr(pos + pattern.length());
                break;
            }
        }
        
        // Si un pattern est trouvé, nettoyer le mot proposé
        if (is_guess && !proposed_word.empty()) {
            // Supprimer le point d'interrogation et les espaces à la fin
            while (!proposed_word.empty() && (proposed_word.back() == '?' || 
                                             proposed_word.back() == '.' || 
                                             std::isspace(proposed_word.back()))) {
                proposed_word.pop_back();
            }
            
            // Supprimer les guillemets et apostrophes
            proposed_word.erase(std::remove(proposed_word.begin(), proposed_word.end(), '\''), proposed_word.end());
            proposed_word.erase(std::remove(proposed_word.begin(), proposed_word.end(), '"'), proposed_word.end());
            
            // Supprimer les espaces au début et à la fin
            while (!proposed_word.empty() && std::isspace(proposed_word.front())) {
                proposed_word.erase(0, 1);
            }
            while (!proposed_word.empty() && std::isspace(proposed_word.back())) {
                proposed_word.pop_back();
            }
            
            // Supprimer les articles au début
            std::vector<std::string> articles = {"un ", "une ", "le ", "la ", "les ", "des ", "l'"};
            for (const auto& article : articles) {
                if (proposed_word.length() > article.length() && 
                    proposed_word.substr(0, article.length()) == article) {
                    proposed_word = proposed_word.substr(article.length());
                    break;
                }
            }
        }
        
        if (is_guess && !proposed_word.empty()) {
            std::cout << "💡 L'IA propose le mot: " << proposed_word << std::endl;
            std::cout << "Si c'est correct, répondez 'trouve', sinon 'oui' ou 'non'." << std::endl;
        }
        
        std::cout << "Votre réponse: ";
        std::getline(std::cin, user_answer);
        
        // Convertir en minuscules
        std::string user_answer_lower = user_answer;
        std::transform(user_answer_lower.begin(), user_answer_lower.end(), user_answer_lower.begin(),
                      [](unsigned char c){ return std::tolower(c); });
        
        if (user_answer_lower == "abandonner") {
            std::cout << "Partie abandonnée." << std::endl;
            pause(2000);
            game_state = GameState::INIT;
            return;
        } else if (user_answer_lower == "trouve") {
            std::cout << "🎉 L'IA a trouvé votre mot !" << std::endl;
            if (!proposed_word.empty()) {
                std::cout << "Le mot était: " << proposed_word << std::endl;
            } else {
                std::cout << "Quel était votre mot ? ";
                std::string player_word;
                std::getline(std::cin, player_word);
                std::cout << "Merci ! L'IA a bien deviné '" << player_word << "'." << std::endl;
            }
            score_ai++;
            game_won = true;
            ai_questions.push_back(ai_question);
            user_answers.push_back(user_answer);
            pause(2000);
        } else if (user_answer_lower != "oui" && user_answer_lower != "non") {
            std::cout << "❌ Veuillez répondre par 'oui', 'non', 'trouve' ou 'abandonner'." << std::endl;
            pause(1500);
            continue; // Ne pas compter cette tentative
        } else {
            // Enregistrer la question et la réponse
            ai_questions.push_back(ai_question);
            user_answers.push_back(user_answer);
            
            // Utiliser la fonction guess() pour générer la prochaine question
            ai_question = chatbot.guess(user_answer_lower);
        }
        
        attempts++;
    }
    
    // Afficher le résultat final
    clearScreen();
    std::cout << "\n🎲 --- Mode: L'IA devine votre mot --- 🎲" << std::endl;
    
    // Afficher l'historique complet
    std::cout << "\nHistorique des échanges:" << std::endl;
    for (size_t i = 0; i < ai_questions.size(); ++i) {
        std::cout << "IA " << (i + 1) << ": " << ai_questions[i] << std::endl;
        std::cout << "Vous " << (i + 1) << ": " << user_answers[i] << std::endl;
    }
    
    if (!game_won) {
        std::cout << "\n⏰ L'IA n'a pas réussi à deviner votre mot en " << max_attempts_ai << " questions." << std::endl;
        std::cout << "Quel était votre mot ? ";
        std::string player_word;
        std::getline(std::cin, player_word);
        std::cout << "Merci ! Ce mot sera utilisé pour améliorer l'IA." << std::endl;
        score_player++;
    } else {
        std::cout << "\n🎉 L'IA a deviné votre mot avec succès !" << std::endl;
    }
    
    std::cout << "\nAppuyez sur Entrée pour continuer...";
    std::cin.get();
    game_state = GameState::INIT;
}

void Devinette::setDifficulty() {
    clearScreen();
    std::cout << "\n⚙️ --- Réglage de la difficulté --- ⚙️" << std::endl;
    std::cout << "1. Facile (16 questions pour le joueur, 15 pour l'IA)" << std::endl;
    std::cout << "2. Normal (12 questions pour le joueur, 12 pour l'IA)" << std::endl;
    std::cout << "3. Difficile (8 questions pour le joueur, 8 pour l'IA)" << std::endl;
    std::cout << "\nChoix: ";

    int choice = getIntInput(1, 3);
    difficulty_level = choice;

    switch (choice) {
        case 1:
            max_attempts_player = 16;
            max_attempts_ai = 15;
            break;
        case 2:
            max_attempts_player = 12;
            max_attempts_ai = 12;
            break;
        case 3:
            max_attempts_player = 8;
            max_attempts_ai = 8;
            break;
    }

    std::cout << "\nDifficulté réglée sur " << (difficulty_level == 1 ? "Facile" : (difficulty_level == 2 ? "Normal" : "Difficile")) << std::endl;
    std::cout << "Appuyez sur Entrée pour continuer...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Devinette::displayScore() {
    clearScreen();
    std::cout << "\n📊 --- Scores --- 📊" << std::endl;
    std::cout << "Joueur: " << score_player << " victoire(s)" << std::endl;
    std::cout << "IA: " << score_ai << " victoire(s)" << std::endl;
    
    if (score_player + score_ai > 0) {
        double win_rate = (static_cast<double>(score_player) / (score_player + score_ai)) * 100;
        std::cout << "Taux de victoire: " << std::fixed << std::setprecision(1) << win_rate << "%" << std::endl;
    }
    
    std::cout << "\nAppuyez sur Entrée pour continuer...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Devinette::displayHistory() {
    clearScreen();
    std::cout << "\n📜 --- Historique --- 📜" << std::endl;
    std::cout << "Fonction en développement" << std::endl;
    
    std::cout << "\nAppuyez sur Entrée pour continuer...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

bool Devinette::askPlayAgain() {
    clearScreen();
    std::cout << "\nVoulez-vous jouer une nouvelle partie ? (o/n): ";
    char response;
    std::cin >> response;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (response == 'o' || response == 'O') {
        game_state = GameState::INIT;
        return true;
    }
    
    return false;
}

void Devinette::displayGoodbye() {
    clearScreen();
    std::cout << "\n👋 Merci d'avoir joué au jeu de devinettes avec IA !" << std::endl;
    std::cout << "À bientôt !" << std::endl;
}

int Devinette::getIntInput(int min, int max) {
    int choice;
    while (!(std::cin >> choice) || choice < min || choice > max) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrée invalide. Veuillez entrer un nombre entre " << min << " et " << max << ": ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

bool Devinette::isValidQuestion(const std::string& question) {
    // Implémentation minimale
    return true;
}

bool Devinette::similarQuestions(const std::string& q1, const std::string& q2) {
    // Implémentation minimale
    return false;
}

void Devinette::displayHelp() {
    clearScreen();
    std::cout << "\n❓ --- Aide --- ❓" << std::endl;
    std::cout << "Comment jouer au jeu de devinettes:" << std::endl;
    std::cout << "\n🔹 Mode 'Devinez le mot de l'IA':" << std::endl;
    std::cout << "  - L'IA choisit un mot dans une catégorie" << std::endl;
    std::cout << "  - Posez des questions qui se répondent par oui/non" << std::endl;
    std::cout << "  - Quand vous pensez avoir trouvé, proposez directement le mot" << std::endl;
    std::cout << "  - Vous avez un nombre limité de questions selon la difficulté" << std::endl;
    
    std::cout << "\n🔹 Mode 'L'IA devine votre mot':" << std::endl;
    std::cout << "  - Pensez à un mot simple (nom commun concret)" << std::endl;
    std::cout << "  - L'IA vous pose des questions, répondez par oui ou non" << std::endl;
    std::cout << "  - Si l'IA devine votre mot, confirmez en répondant 'oui'" << std::endl;
    std::cout << "  - L'IA a un nombre limité de questions selon la difficulté" << std::endl;
    
    std::cout << "\nAppuyez sur Entrée pour continuer...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void Devinette::toggleDebugMode() {
    debug_mode = !debug_mode;
    std::cout << "\nMode débogage " << (debug_mode ? "activé" : "désactivé") << "." << std::endl;
    std::cout << "Appuyez sur Entrée pour continuer...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

GameState Devinette::getGameState() const {
    return game_state;
}

int Devinette::getScorePlayer() const {
    return score_player;
}

int Devinette::getScoreAI() const {
    return score_ai;
}

bool Devinette::isGameWon() const {
    return false;  // Implémentation minimale
}