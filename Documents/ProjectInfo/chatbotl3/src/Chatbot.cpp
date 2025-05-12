#include "Chatbot.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <cpr/cpr.h>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <map>
#include <set>

using json = nlohmann::json;

Chatbot::Chatbot(const std::string& api_key, const std::string& model, const std::string& server_url, bool debug_mode)
    : api_key(api_key), model(model), server_url(server_url), debug_mode(debug_mode), 
      gameEnded(false), first_display(true), is_thinking(false),
      is_connected(false), connection_tested(false),
      score(0), total_games(0), games_won(0) {
    initializeRNG();
    reset();
}

void Chatbot::initializeRNG() {
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(static_cast<unsigned int>(seed));
}

void Chatbot::reset() {
    gameEnded = false;
    first_display = true;
    is_thinking = false;
    // Ne pas réinitialiser is_connected et connection_tested ici
    chosen_word.clear();
    chosen_category.clear();
    session_id.clear();
    context.clear();
    current_context.clear();
    initial_context.clear();
    last_error.clear();
    game_history.clear();
    message_history.clear();
    history.clear();
    reponses.clear();
    current_display.clear();
    
    // Nettoyer l'écran
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Chatbot::thinkOfWord() {
    if (is_thinking) return;
    is_thinking = true;
    
    reset();
    
    // Génération d'une marque temporelle pour rendre chaque requête plus unique
    auto now = std::chrono::system_clock::now();
    auto time_str = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
    
    // Création d'une graine aléatoire pour l'unicité
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 10000);
    int random_seed = dist(gen);
    
    // Demander à l'IA de choisir un mot et une catégorie avec une instruction favorisant la diversité
    std::string prompt = std::string("Tu es un assistant pour un jeu de devinettes. ")
                        + "Choisis un mot simple et sa catégorie pour un jeu de devinettes. "
                        + "Le mot doit être un nom commun concret, facile à deviner avec des questions oui/non. "
                        + "Évite les mots abstraits, les verbes, les adjectifs ou les mots trop spécifiques. "
                        + "Choisis parmi ces catégories: Fruits, Légumes, Animaux, Objets, Villes, Pays, Sports, Métiers. "
                        + "IMPORTANT: Sois créatif et évite les mots trop communs. Choisis un mot original mais pas trop difficile. "
                        + "Utilise cette marque temporelle unique comme inspiration: " + time_str + " et ce nombre aléatoire: " + std::to_string(random_seed) + ". "
                        + "Réponds UNIQUEMENT avec ce format exact (deux lignes):\n"
                        + "Catégorie: [catégorie]\n"
                        + "Mot: [mot simple en français]";
    
    std::string response = sendRequest(prompt);
    
    if (debug_mode) {
        std::cout << "Réponse brute de l'IA: " << response << std::endl;
    }
    
    // Extraire la catégorie et le mot de la réponse de manière plus robuste
    std::string category = "";
    std::string word = "";
    
    // Utiliser une approche ligne par ligne
    std::istringstream iss(response);
    std::string line;
    while (std::getline(iss, line)) {
        // Nettoyer la ligne
        line.erase(0, line.find_first_not_of(" \n\r\t"));
        
        if (line.find("Catégorie:") == 0 || line.find("Categorie:") == 0) {
            size_t start = line.find(":") + 1;
            if (start < line.length()) {
                category = line.substr(start);
                // Nettoyer la catégorie
                category.erase(0, category.find_first_not_of(" \n\r\t"));
                category.erase(category.find_last_not_of(" \n\r\t") + 1);
            }
        } 
        else if (line.find("Mot:") == 0) {
            size_t start = line.find(":") + 1;
            if (start < line.length()) {
                word = line.substr(start);
                // Nettoyer le mot
                word.erase(0, word.find_first_not_of(" \n\r\t"));
                word.erase(word.find_last_not_of(" \n\r\t") + 1);
                // Supprimer les guillemets s'il y en a
                if (!word.empty() && (word.front() == '"' || word.front() == '\'')) {
                    word.erase(0, 1);
                }
                if (!word.empty() && (word.back() == '"' || word.back() == '\'')) {
                    word.pop_back();
                }
            }
        }
    }
    
    // Vérifier si la catégorie et le mot sont valides
    bool valid_category = !category.empty() && category.length() < 30;
    bool valid_word = !word.empty() && word.length() >= 3 && word.length() <= 15;
    
    // Si l'extraction a échoué, faire une seconde tentative avec un prompt encore plus direct
    if (!valid_category || !valid_word) {
        if (debug_mode) {
            std::cout << "Première tentative échouée. Essai avec un prompt plus direct." << std::endl;
        }
        
        // Générer une nouvelle graine aléatoire pour cette seconde tentative
        random_seed = dist(gen);
        
        std::string retry_prompt = "Choisis un mot simple et une catégorie ORIGINALE pour un jeu de devinettes. "
                                  "Utilise ce nombre comme inspiration pour être créatif: " + std::to_string(random_seed) + ". "
                                  "Réponds UNIQUEMENT avec deux lignes dans ce format exact:\n"
                                  "Catégorie: [une seule catégorie parmi: Fruits, Légumes, Animaux, Objets, Villes, Pays, Sports, Métiers]\n"
                                  "Mot: [un seul mot simple en français, pas trop commun mais pas trop difficile]";
        
        response = sendRequest(retry_prompt);
        
        if (debug_mode) {
            std::cout << "Réponse de la seconde tentative: " << response << std::endl;
        }
        
        // Réessayer l'extraction
        category = "";
        word = "";
        std::istringstream retry_iss(response);
        while (std::getline(retry_iss, line)) {
            line.erase(0, line.find_first_not_of(" \n\r\t"));
            
            if (line.find("Catégorie:") == 0 || line.find("Categorie:") == 0) {
                size_t start = line.find(":") + 1;
                if (start < line.length()) {
                    category = line.substr(start);
                    category.erase(0, category.find_first_not_of(" \n\r\t"));
                    category.erase(category.find_last_not_of(" \n\r\t") + 1);
                }
            } 
            else if (line.find("Mot:") == 0) {
                size_t start = line.find(":") + 1;
                if (start < line.length()) {
                    word = line.substr(start);
                    word.erase(0, word.find_first_not_of(" \n\r\t"));
                    word.erase(word.find_last_not_of(" \n\r\t") + 1);
                    if (!word.empty() && (word.front() == '"' || word.front() == '\'')) {
                        word.erase(0, 1);
                    }
                    if (!word.empty() && (word.back() == '"' || word.back() == '\'')) {
                        word.pop_back();
                    }
                }
            }
        }
        
        valid_category = !category.empty() && category.length() < 30;
        valid_word = !word.empty() && word.length() >= 3 && word.length() <= 15;
    }
    
    // Si l'extraction a encore échoué, utiliser des valeurs générées aléatoirement à partir d'une liste étendue
    if (!valid_category || !valid_word) {
        // Liste étendue de catégories et mots de secours
        std::map<std::string, std::vector<std::string>> categories = {
            {"Fruits", {
                "pomme", "banane", "orange", "fraise", "kiwi", "mangue", "ananas", "poire", 
                "cerise", "pêche", "abricot", "raisin", "pastèque", "melon", "prune", "grenade",
                "citron", "pamplemousse", "myrtille", "framboise", "mûre", "figue", "datte",
                "litchi", "papaye", "mandarine", "clémentine", "cassis", "groseille", "nectarine"
            }},
            {"Légumes", {
                "carotte", "tomate", "poivron", "courgette", "brocoli", "haricot", "poireau", "chou",
                "salade", "épinard", "concombre", "aubergine", "asperge", "artichaut", "céleri", "maïs",
                "oignon", "ail", "radis", "betterave", "navet", "champignon", "potiron", "citrouille",
                "fenouil", "panais", "patate", "pomme de terre", "pois", "lentille"
            }},
            {"Animaux", {
                "chat", "chien", "lion", "éléphant", "girafe", "tigre", "zèbre", "singe", "ours", "panda",
                "loup", "renard", "écureuil", "lapin", "cerf", "sanglier", "hibou", "aigle", "mouette", "pingouin",
                "crocodile", "serpent", "lézard", "tortue", "baleine", "dauphin", "requin", "pieuvre", "méduse",
                "fourmi", "abeille", "guêpe", "papillon", "mouche", "moustique", "araignée", "scorpion"
            }},
            {"Objets", {
                "stylo", "crayon", "gomme", "règle", "ciseaux", "agrafeuse", "calculatrice", "trombone", "tasse", "lampe",
                "livre", "cahier", "classeur", "agenda", "montre", "téléphone", "ordinateur", "clavier", "souris", "écran",
                "appareil photo", "télécommande", "télévision", "radio", "casque", "micro", "enceinte", "caméra", "horloge",
                "réveil", "table", "chaise", "bureau", "fauteuil", "canapé", "armoire", "étagère", "miroir", "parapluie", "clé"
            }},
            {"Villes", {
                "paris", "marseille", "lyon", "toulouse", "nice", "nantes", "montpellier", "strasbourg", "bordeaux", "lille",
                "rennes", "reims", "toulon", "grenoble", "dijon", "angers", "nîmes", "villeurbanne", "saint-étienne", "metz",
                "londres", "berlin", "madrid", "rome", "amsterdam", "bruxelles", "lisbonne", "vienne", "stockholm", "athènes",
                "varsovie", "budapest", "prague", "oslo", "helsinki", "dublin", "new york", "pékin", "tokyo", "moscou", "sydney"
            }},
            {"Pays", {
                "france", "espagne", "italie", "allemagne", "portugal", "suisse", "belgique", "autriche", "irlande", "grèce",
                "suède", "finlande", "norvège", "danemark", "islande", "royaume-uni", "pologne", "ukraine", "tchéquie", "croatie",
                "japon", "chine", "canada", "mexique", "brésil", "argentine", "chili", "australie", "nouvelle-zélande", "maroc",
                "tunisie", "sénégal", "kenya", "afrique du sud", "inde", "thaïlande", "vietnam", "corée", "égypte", "turquie"
            }},
            {"Sports", {
                "football", "basketball", "tennis", "golf", "natation", "cyclisme", "ski", "patinage", "snowboard", "escalade",
                "randonnée", "voile", "surf", "plongée", "boxe", "judo", "karaté", "taekwondo", "escrime", "tir à l'arc",
                "marathon", "triathlon", "gymnastique", "équitation", "volleyball", "handball", "rugby", "hockey", "ping-pong", "badminton"
            }},
            {"Métiers", {
                "médecin", "infirmier", "pharmacien", "dentiste", "vétérinaire", "enseignant", "professeur", "chercheur", "avocat", "juge",
                "architecte", "ingénieur", "développeur", "programmeur", "designer", "graphiste", "journaliste", "écrivain", "traducteur", "musicien",
                "boulanger", "pâtissier", "cuisinier", "chef", "boucher", "fromager", "agriculteur", "fleuriste", "jardinier", "paysagiste",
                "pilote", "astronaute", "pompier", "policier", "militaire", "plombier", "électricien", "mécanicien", "couturier", "coiffeur"
            }}
        };

        // Sélection aléatoire de la catégorie avec un générateur vraiment aléatoire
    std::vector<std::string> category_names;
    for (const auto& pair : categories) {
        category_names.push_back(pair.first);
    }
        
    std::uniform_int_distribution<size_t> cat_dist(0, category_names.size() - 1);
        category = category_names[cat_dist(gen)];

    // Sélection aléatoire du mot dans la catégorie
        const auto& words = categories.at(category);
    std::uniform_int_distribution<size_t> word_dist(0, words.size() - 1);
        word = words[word_dist(gen)];

    if (debug_mode) {
            std::cout << "Utilisation des valeurs par défaut car l'extraction a échoué." << std::endl;
            std::cout << "Catégorie valide: " << valid_category << ", Mot valide: " << valid_word << std::endl;
        }
    }
    
    // Première lettre en majuscule pour la catégorie
    if (!category.empty()) {
        category[0] = std::toupper(category[0]);
    }
    
    // Assigner les valeurs extraites
    chosen_category = category;
    chosen_word = word;
    
    if (debug_mode) {
        std::cout << "Catégorie choisie par l'IA : " << chosen_category << std::endl;
        std::cout << "Mot choisi par l'IA : " << chosen_word << std::endl;
    }
    
    setupGameContext();
    initializeWordKnowledge();
    displayGameInterface();
    
    is_thinking = false;
}

void Chatbot::displayGameInterface() {
    // Vérifier l'état de connexion de l'API (utilise le cache)
    bool is_api_connected = isConnected();
    
    if (first_display) {
        // Nettoyer l'écran
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        
        std::cout << "\n🎲 --- Mode: Devinez le mot de l'IA --- 🎲\n\n";
        
        // Afficher le mot uniquement en mode débogage
        if (debug_mode) {
            std::cout << "Mot à deviner: " << chosen_word << "\n";
        }
        
        std::cout << "Questions restantes: 15/15\n\n";
        std::cout << "=== Jeu de Devinettes ===\n";
        std::cout << "L'IA a choisi un mot dans la catégorie : " << chosen_category << "\n";
        std::cout << "Vous avez 15 questions pour le deviner.\n";
        std::cout << "Posez vos questions en français.\n";
        std::cout << "Commandes spéciales :\n";
        std::cout << "  - /reset : Réinitialiser le jeu\n";
        std::cout << "  - /stats : Afficher les statistiques\n";
        std::cout << "  - /quit : Quitter le jeu\n";
        std::cout << "  - trouve [mot] : Pour deviner le mot\n\n";
        
        // Afficher l'état de connexion en bas à droite
        if (is_api_connected) {
            // Code ANSI pour le texte vert
            std::cout << "\033[32mCONNEXION\033[0m";
        } else {
            // Code ANSI pour le texte rouge
            std::cout << "\033[31mHORS CONNEXION\033[0m";
        }
        std::cout << std::endl;
        
        first_display = false;
    } else {
        // Afficher l'historique des questions et réponses
        std::cout << "\n🎲 --- Mode: Devinez le mot de l'IA --- 🎲\n\n";
        
        // Afficher le mot uniquement en mode débogage
        if (debug_mode) {
            std::cout << "Mot à deviner: " << chosen_word << "\n";
        }
        
        std::cout << "Questions restantes: " << (15 - history.size()) << "/15\n\n";
        
        if (!history.empty()) {
            std::cout << "Historique des questions:\n";
            for (size_t i = 0; i < history.size(); ++i) {
                std::cout << "Q" << (i + 1) << ": " << history[i] << "\n";
                if (i < reponses.size()) {
                    std::cout << "R" << (i + 1) << ": " << reponses[i] << "\n";
                }
            }
            std::cout << "\n";
        }
        
        // Afficher l'état de connexion en bas à droite
        std::cout << "\n";
        if (is_api_connected) {
            // Code ANSI pour le texte vert
            std::cout << "\033[32mCONNEXION\033[0m";
        } else {
            // Code ANSI pour le texte rouge
            std::cout << "\033[31mHORS CONNEXION\033[0m";
        }
        std::cout << "\n\n";
        
        std::cout << "Question " << (history.size() + 1) << "/15: ";
    }
}

void Chatbot::setupGameContext() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    session_id = std::to_string(now_ms.count());

    initial_context = "Je suis un assistant qui joue à un jeu de devinettes. "
                     "J'ai choisi le mot '" + chosen_word + "' dans la catégorie '" + 
                     chosen_category + "'. Je dois répondre aux questions du joueur "
                     "en gardant ce mot secret.";
    context = initial_context;
}

std::string Chatbot::ask(const std::string& question) {
    if (question == "/reset") {
        reset();
        thinkOfWord();
        return "Nouvelle partie commencée !";
    }
    if (gameEnded) {
        return "Le jeu est terminé. Utilisez /reset pour recommencer.";
    }
    if (history.size() >= 15) {
        gameEnded = true;
        total_games++;
        return "Partie terminée ! Le mot était '" + chosen_word + "'.";
    }
    if (isSpecialCommand(question)) {
        return handleSpecialCommand(question);
    }

    std::string clean_question = cleanInputQuestion(question);
    std::string history_context = buildHistoryContext();
    std::string response = processQuestion(clean_question, history_context);

    // Ajout à l'historique
    history.push_back(question);
    reponses.push_back(response);

    // Si l'IA a détecté une victoire ou une défaite
    if (response.find("VICTOIRE") != std::string::npos || 
        response.find("DÉFAITE") != std::string::npos) {
        gameEnded = true;
        return response;
    }

    // Afficher l'interface mise à jour avec l'historique
    displayGameInterface();

    // Retourner simplement la réponse
    return response;
}

bool Chatbot::isSpecialCommand(const std::string& question) {
    return question == "/reset" || question == "/stats" || question == "/quit";
}

std::string Chatbot::handleSpecialCommand(const std::string& command) {
    if (command == "/stats") {
        displayGameStats();
        return "";
    }
    else if (command == "/quit") {
        displayFinalStats();
        gameEnded = true;
        return "Merci d'avoir joué !";
    }
    return "";
}

void Chatbot::displayGameStats() {
    if (gameEnded) {
        return;  // Ne pas afficher les stats si la partie est terminée
    }

    std::cout << "\n📊 Statistiques de la partie:\n";
    std::cout << "   Tentatives restantes: " << (15 - history.size()) << "\n";
    std::cout << "   Questions posées: " << history.size() << "\n";
    std::cout << "   Score actuel: " << score << " point(s)\n";
    std::cout << "   Parties gagnées: " << games_won << "/" << total_games << "\n\n";

    if (!history.empty()) {
        std::cout << "📜 Historique des dernières questions:\n";
        for (size_t i = 0; i < history.size(); ++i) {
            std::cout << "   Q: " << history[i] << "\n";
            if (i < reponses.size()) {
                std::cout << "   R: " << reponses[i] << "\n";
            }
        }
        std::cout << "\n";
    }
}

std::string Chatbot::cleanInputQuestion(const std::string& question) {
    std::string cleaned = question;
    std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(), ::tolower);
    return cleaned;
}

std::string Chatbot::buildHistoryContext() {
    if (history.empty()) {
        return "";
    }

    std::stringstream ss;
    ss << "Historique des questions et réponses :\n";
    for (size_t i = 0; i < history.size(); ++i) {
        ss << "Q" << (i + 1) << ": " << history[i] << "\n";
        if (i < reponses.size()) {
            ss << "R" << (i + 1) << ": " << reponses[i] << "\n";
        }
    }
    ss << "\nInstructions pour l'analyse :\n";
    ss << "1. Vérifiez la cohérence entre les questions\n";
    ss << "2. Assurez-vous que les réponses ne se contredisent pas\n";
    ss << "3. Prenez en compte le contexte des questions précédentes\n";
    return ss.str();
}

std::string Chatbot::processQuestion(const std::string& question, const std::string& /*history_context*/) {
    // Si c'est une réponse à une question de l'IA (dans le mode où l'IA devine)
    if (question == "oui" || question == "non" || question == "o" || question == "n") {
        std::string normalized = question;
        std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
        
        // Convertir en format standard
        if (normalized == "o") normalized = "oui";
        if (normalized == "n") normalized = "non";
        
        // Ajouter à l'historique des réponses
        if (!history.empty()) {
            reponses.push_back(normalized);
            
            // Générer la prochaine question
            std::string next_question = guess(normalized);
            return next_question;
        }
        return "Veuillez d'abord laisser l'IA poser une question.";
    }
    
    // Mode où le joueur devine le mot de l'IA
    // Nettoyer et normaliser la question
    std::string normalized_question = normalizeWord(question);
    std::string normalized_word = normalizeWord(chosen_word);
    
    // Vérifier si c'est une tentative de devinette explicite avec le mot "trouve"
    if (question.find("trouve") == 0) {
        std::string guess = question.substr(7); // Après "trouve "
        if (normalizeWord(guess) == normalized_word) {
            gameEnded = true;
            games_won++;
            total_games++;
            score += 10;
            return "VICTOIRE ! Bravo, vous avez trouvé le mot '" + chosen_word + "' !";
        } else {
            return "Non, ce n'est pas '" + guess + "'. Continuez à chercher.";
        }
    }
    
    // Vérifier si la question contient directement le mot à deviner
    if (normalized_question.find(normalized_word) != std::string::npos) {
        // Vérifier si c'est vraiment une tentative de deviner le mot et pas juste une mention
        if (normalized_question.find("est") != std::string::npos || 
            normalized_question.find("serait") != std::string::npos ||
            normalized_question.find("c'est") != std::string::npos ||
            normalized_question.find("ce sont") != std::string::npos) {
            gameEnded = true;
            games_won++;
            total_games++;
            score += 10;
            return "VICTOIRE ! Bravo, vous avez trouvé le mot '" + chosen_word + "' !";
        }
    }
    
    // Vérification simple pour les catégories
    if (normalized_question.find("categorie") != std::string::npos) {
        return "La catégorie est '" + chosen_category + "'.";
    }

    // Vérification spéciale pour les questions de catégorie de base
    if (normalized_question.find(chosen_category.substr(0, chosen_category.length()-1).c_str()) != std::string::npos ||
        (chosen_category == "Légumes" && normalized_question.find("legume") != std::string::npos) ||
        (chosen_category == "Fruits" && normalized_question.find("fruit") != std::string::npos) ||
        (chosen_category == "Animaux" && normalized_question.find("animal") != std::string::npos) ||
        (chosen_category == "Objets" && normalized_question.find("objet") != std::string::npos) ||
        (chosen_category == "Villes" && normalized_question.find("ville") != std::string::npos) ||
        (chosen_category == "Pays" && normalized_question.find("pays") != std::string::npos) ||
        (chosen_category == "Sports" && normalized_question.find("sport") != std::string::npos) ||
        (chosen_category == "Métiers" && normalized_question.find("metier") != std::string::npos)) {
        return "Oui";
    }
    
    // Vérifier si la question contient directement le mot
    if (normalized_question.find(normalized_word) != std::string::npos) {
        return "Oui";
    }
    
    // Recourir directement à l'IA pour obtenir la réponse
    
    // Construction d'un prompt simple et direct pour l'IA
    std::string prompt = std::string("Tu es un expert en jeu de devinettes. Tu dois répondre à des questions sur un mot secret.\n\n")
                         + "Le mot à deviner est '" + chosen_word + "' dans la catégorie '" + chosen_category + "'.\n\n"
                         + "Question actuelle: \"" + question + "\"\n\n"
                         + "INSTRUCTIONS CRITIQUES:\n"
                         + "1. Réponds UNIQUEMENT par le mot 'Oui' ou le mot 'Non' (avec la première lettre majuscule).\n"
                         + "2. Ne donne AUCUNE explication, justification ou détail supplémentaire.\n"
                         + "3. N'utilise PAS de ponctuation ni de phrases complètes.\n"
                         + "4. Ta réponse doit contenir EXACTEMENT un seul mot: soit 'Oui' soit 'Non'.\n\n";
    
    // Ajouter l'historique complet des questions et réponses pour assurer la cohérence
    if (!history.empty() && !reponses.empty()) {
        prompt += "Historique des questions et réponses précédentes:\n";
        for (size_t i = 0; i < history.size() && i < reponses.size(); ++i) {
            prompt += "Q" + std::to_string(i+1) + ": " + history[i] + "\n";
            prompt += "R" + std::to_string(i+1) + ": " + reponses[i] + "\n";
        }
        
        prompt += "\nIMPORTANT: Ta réponse DOIT être cohérente avec l'historique des questions et réponses précédentes. "
                  "ÉVITE ABSOLUMENT toute contradiction avec ces réponses. "
                  "Vérifie attentivement la logique de tes réponses.\n";
    }
    
    prompt += "\nRéponds maintenant UNIQUEMENT par le mot 'Oui' ou le mot 'Non'. Rien d'autre.";
    
    try {
        // ALGORITHME AMÉLIORÉ POUR L'EXTRACTION DES RÉPONSES OUI/NON
        
        // Faire plusieurs requêtes pour obtenir un consensus plus solide
        const int NUM_REQUESTS = 5; // Augmentation du nombre de requêtes pour un meilleur consensus
        std::vector<std::string> raw_responses;
        std::vector<std::string> classified_responses;
        
        // 1. Collecter plusieurs réponses brutes
        for (int i = 0; i < NUM_REQUESTS; i++) {
            std::string response = sendRequest(prompt);
            raw_responses.push_back(response);
            
            if (debug_mode) {
                std::cout << "Réponse brute " << (i+1) << ": " << response << std::endl;
            }
        }
        
        // 2. Analyse approfondie de chaque réponse avec un algorithme plus robuste
        for (const auto& response : raw_responses) {
            // Normaliser et nettoyer la réponse
            std::string cleaned = response;
            
            // Supprimer les espaces en début et fin
            cleaned.erase(0, cleaned.find_first_not_of(" \n\r\t"));
            cleaned.erase(cleaned.find_last_not_of(" \n\r\t") + 1);
            
            // Normaliser en minuscules pour l'analyse
            std::string normalized = normalizeWord(cleaned);
            
            // Extraction précise - vérifier d'abord si la réponse est exactement "Oui" ou "Non"
            if (cleaned == "Oui" || cleaned == "OUI" || cleaned == "oui") {
                classified_responses.push_back("oui");
                continue;
            } 
            else if (cleaned == "Non" || cleaned == "NON" || cleaned == "non") {
                classified_responses.push_back("non");
                continue;
            }
            
            // Si la réponse n'est pas exactement "Oui" ou "Non", analyser plus en profondeur
            
            // Vérifier si la réponse commence par "Oui" ou "Non" (cas le plus courant)
            if (normalized.find("oui") == 0 || normalized.find("yes") == 0) {
                classified_responses.push_back("oui");
                continue;
            }
            else if (normalized.find("non") == 0 || normalized.find("no") == 0) {
                classified_responses.push_back("non");
                continue;
            }
            
            // Analyse plus complexe si nécessaire
            bool has_oui = normalized.find("oui") != std::string::npos || normalized.find("yes") != std::string::npos;
            bool has_non = normalized.find("non") != std::string::npos || normalized.find("no") != std::string::npos;
            
            // Vérifier les cas de négation explicite avec une fenêtre plus large
            bool has_pas_oui = normalized.find("pas oui") != std::string::npos || 
                               normalized.find("n'est pas oui") != std::string::npos ||
                               normalized.find("n est pas oui") != std::string::npos ||
                               normalized.find("ne serait pas oui") != std::string::npos;
                               
            bool has_pas_non = normalized.find("pas non") != std::string::npos || 
                               normalized.find("n'est pas non") != std::string::npos ||
                               normalized.find("n est pas non") != std::string::npos ||
                               normalized.find("ne serait pas non") != std::string::npos;
            
            // Tenir compte des négations
            if (has_pas_oui) has_oui = false;
            if (has_pas_non) has_non = false;
            
            if (has_oui && !has_non) {
                classified_responses.push_back("oui");
            } 
            else if (has_non && !has_oui) {
                classified_responses.push_back("non");
            }
            else if (has_oui && has_non) {
                // Si les deux sont présents, analyser la position et le contexte
                size_t oui_pos = normalized.find("oui");
                size_t non_pos = normalized.find("non");
                
                // Vérifier si l'un des termes est dans une négation
                bool oui_negated = false;
                bool non_negated = false;
                
                if (oui_pos > 0) {
                    // Rechercher des marqueurs de négation dans une fenêtre de 15 caractères avant "oui"
                    std::string before_oui = normalized.substr(std::max(0, static_cast<int>(oui_pos) - 15), 15);
                    oui_negated = before_oui.find("pas ") != std::string::npos || 
                                  before_oui.find("ne ") != std::string::npos ||
                                  before_oui.find("n'") != std::string::npos;
                }
                
                if (non_pos > 0) {
                    // Rechercher des marqueurs de négation dans une fenêtre de 15 caractères avant "non"
                    std::string before_non = normalized.substr(std::max(0, static_cast<int>(non_pos) - 15), 15);
                    non_negated = before_non.find("pas ") != std::string::npos || 
                                 before_non.find("ne ") != std::string::npos ||
                                 before_non.find("n'") != std::string::npos;
                }
                
                // Prendre une décision basée sur la négation et la position
                if (oui_negated && !non_negated) {
                    classified_responses.push_back("non");
                } 
                else if (!oui_negated && non_negated) {
                    classified_responses.push_back("oui");
                }
                else if (oui_negated && non_negated) {
                    // Les deux sont niés, décision difficile, utiliser la position comme heuristique
                    if (oui_pos < non_pos) {
                        classified_responses.push_back("non");
                    } else {
                        classified_responses.push_back("oui");
                    }
                }
                else {
                    // Aucun n'est nié, utiliser la position comme heuristique
                    if (oui_pos < non_pos) {
                        classified_responses.push_back("oui");
                    } else {
                        classified_responses.push_back("non");
                    }
                }
            }
            else {
                // Recherche d'expressions alternatives
                std::vector<std::string> positive_expressions = {
                    "affirmatif", "exact", "correct", "tout a fait", "effectivement", "certainement",
                    "absolument", "bien sur", "evidemment", "positif", "je confirme", "c'est ca",
                    "c'est exact", "c'est juste", "c'est bien", "c'est vrai", "en effet"
                };
                
                std::vector<std::string> negative_expressions = {
                    "negatif", "incorrect", "faux", "pas du tout", "absolument pas", "certainement pas",
                    "en aucun cas", "jamais", "aucunement", "je ne pense pas", "je ne crois pas",
                    "ce n'est pas", "ce n est pas", "ce n'est pas ca", "ce n'est pas le cas"
                };
                
                bool found_positive = false;
                bool found_negative = false;
                
                for (const auto& expr : positive_expressions) {
                    if (normalized.find(expr) != std::string::npos) {
                        found_positive = true;
                        break;
                    }
                }
                
                for (const auto& expr : negative_expressions) {
                    if (normalized.find(expr) != std::string::npos) {
                        found_negative = true;
                        break;
                    }
                }
                
                if (found_positive && !found_negative) {
                    classified_responses.push_back("oui");
                } else if (found_negative && !found_positive) {
                    classified_responses.push_back("non");
                }
                // Si aucune expression n'est trouvée ou si les deux types sont trouvés,
                // ne pas ajouter de réponse au vote
            }
        }
        
        // 3. Vérification de cohérence avec l'historique
        if (!history.empty() && !reponses.empty()) {
            // Vérifier si la question actuelle est similaire à une question précédente
            for (size_t i = 0; i < history.size() && i < reponses.size(); ++i) {
                if (similarQuestions(question, history[i])) {
                    std::string previous_answer = normalizeWord(reponses[i]);
                    
                    // Donner plus de poids à la réponse précédente pour assurer la cohérence
                    for (int j = 0; j < 3; j++) {  // Augmentation du poids pour plus de cohérence
                        classified_responses.push_back(previous_answer);
                    }
                    
                    if (debug_mode) {
                        std::cout << "Question similaire trouvée: " << history[i] << " (Réponse: " << previous_answer << ")" << std::endl;
                    }
                    
                    break;
                }
            }
            
            // Vérification supplémentaire pour les questions liées aux attributs déjà mentionnés
            std::string normalized_q = normalizeWord(question);
            for (size_t i = 0; i < history.size() && i < reponses.size(); ++i) {
                std::string normalized_hist_q = normalizeWord(history[i]);
                
                // Si la question actuelle et une question précédente partagent des mots clés significatifs
                // et que les réponses devraient logiquement être liées
                std::vector<std::string> key_attributes = {"couleur", "taille", "forme", "usage", "utilisation", 
                                                          "fonction", "origine", "habitat", "comestible", "mangeable"};
                
                for (const auto& attr : key_attributes) {
                    if (normalized_q.find(attr) != std::string::npos && 
                        normalized_hist_q.find(attr) != std::string::npos) {
                        
                        // Les deux questions concernent le même attribut
                        // Analyser si les réponses doivent être cohérentes
                        
                        // Exemple: si une question précédente était "Est-ce que c'est rouge?" avec réponse "Oui",
                        // et la question actuelle est "Est-ce que c'est bleu?", la réponse devrait être "Non"
                        
                        // Pour l'instant, simplement augmenter le poids de la cohérence globale
                        // Une implémentation plus sophistiquée pourrait analyser la relation logique entre les questions
                        std::string previous_answer = normalizeWord(reponses[i]);
                        classified_responses.push_back(previous_answer);
                        break;
                    }
                }
            }
        }
        
        // 4. Vote final avec pondération
        int yes_count = std::count(classified_responses.begin(), classified_responses.end(), "oui");
        int no_count = std::count(classified_responses.begin(), classified_responses.end(), "non");
        
        if (debug_mode) {
            std::cout << "Votes: Oui = " << yes_count << ", Non = " << no_count << std::endl;
        }
        
        // 5. Décision finale
        if (yes_count > no_count) {
            // Vérifier la cohérence logique avant de retourner "Oui"
            if (checkLogicalConsistency(question, "Oui")) {
                return "Oui";
            } else {
                // Si "Oui" est incohérent, retourner "Non"
                if (debug_mode) {
                    std::cout << "Réponse 'Oui' incohérente, corrigée en 'Non'" << std::endl;
                }
                return "Non";
            }
        } else if (no_count > yes_count) {
            // Vérifier la cohérence logique avant de retourner "Non"
            if (checkLogicalConsistency(question, "Non")) {
                return "Non";
            } else {
                // Si "Non" est incohérent, retourner "Oui"
                if (debug_mode) {
                    std::cout << "Réponse 'Non' incohérente, corrigée en 'Oui'" << std::endl;
                }
                return "Oui";
            }
        } else if (!classified_responses.empty()) {
            // En cas d'égalité, faire une requête supplémentaire avec un prompt encore plus direct
            std::string direct_prompt = std::string("Pour le mot '") + chosen_word + "', réponds UNIQUEMENT par le mot 'Oui' ou le mot 'Non' à la question: '" + 
                question + "'. IMPORTANT: Sois cohérent avec les réponses précédentes. Ta réponse doit être EXACTEMENT un seul mot, sans aucune ponctuation ni explication. Juste 'Oui' ou 'Non'.";
            
            std::string direct_response = sendRequest(direct_prompt);
            std::string normalized_direct = normalizeWord(direct_response);
            
            if (normalized_direct.find("oui") != std::string::npos && 
                normalized_direct.find("non") == std::string::npos) {
                // Vérifier la cohérence logique
                if (checkLogicalConsistency(question, "Oui")) {
                    return "Oui";
                } else {
                    return "Non";
                }
            } else if (normalized_direct.find("non") != std::string::npos && 
                      normalized_direct.find("oui") == std::string::npos) {
                // Vérifier la cohérence logique
                if (checkLogicalConsistency(question, "Non")) {
                    return "Non";
                } else {
                    return "Oui";
                }
            } else {
                // Si toujours ambigu, prendre la première réponse classifiée et vérifier sa cohérence
                std::string proposed_answer = (classified_responses[0] == "oui") ? "Oui" : "Non";
                if (checkLogicalConsistency(question, proposed_answer)) {
                    return proposed_answer;
                } else {
                    return (proposed_answer == "Oui") ? "Non" : "Oui";
                }
            }
        } else {
            // Si aucune réponse n'a été classifiée, faire une dernière tentative directe
            std::string direct_prompt = std::string("Pour le mot '") + chosen_word + "', réponds UNIQUEMENT par le mot 'Oui' ou le mot 'Non' à la question: '" + 
                question + "'. Ta réponse doit être EXACTEMENT un seul mot, sans aucune ponctuation ni explication. Juste 'Oui' ou 'Non'.";
            
            std::string direct_response = sendRequest(direct_prompt);
            std::string normalized_direct = normalizeWord(direct_response);
            
            if (normalized_direct.find("oui") != std::string::npos && 
                normalized_direct.find("non") == std::string::npos) {
                // Vérifier la cohérence logique
                if (checkLogicalConsistency(question, "Oui")) {
                    return "Oui";
                } else {
                    return "Non";
                }
            } else {
                // Vérifier la cohérence logique
                if (checkLogicalConsistency(question, "Non")) {
                    return "Non";
                } else {
                    return "Oui";
                }
            }
        }
    } catch (const std::exception& e) {
        last_error = "Erreur lors de la communication avec l'IA: " + std::string(e.what());
        return "Erreur: Impossible de communiquer avec l'IA.";
    }
}

std::string Chatbot::handleVictory() {
    score += 1;
    games_won++;
    total_games++;
    
    // Nettoyer l'écran
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    std::stringstream ss;
    ss << "\n🎉 FÉLICITATIONS ! 🎉\n";
    ss << "Vous avez trouvé le mot '" << chosen_word << "' !\n";
    ss << "Score actuel : " << score << " point(s)\n";
    ss << "Parties gagnées : " << games_won << "/" << total_games << "\n\n";
    
    std::cout << ss.str();
    
    if (askNewGame()) {
        reset();
        thinkOfWord();
        return "Nouvelle partie commencée !";
    } else {
        displayFinalStats();
        gameEnded = true;
        return "Fin de la partie. Merci d'avoir joué !";
    }
}

void Chatbot::displayFinalStats() const {
    // Nettoyer l'écran
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    std::cout << "\n=== Statistiques Finales ===\n";
    std::cout << "Score total : " << score << " point(s)\n";
    std::cout << "Parties jouées : " << total_games << "\n";
    std::cout << "Parties gagnées : " << games_won << "\n";
    if (total_games > 0) {
        double win_rate = (static_cast<double>(games_won) / total_games) * 100;
        std::cout << "Taux de réussite : " << std::fixed << std::setprecision(1) << win_rate << "%\n";
    }
    std::cout << "========================\n\n";
}

bool Chatbot::askNewGame() const {
    std::cout << "\nVoulez-vous jouer une nouvelle partie ? (o/n) : ";
    std::string response;
    std::getline(std::cin, response);
    std::transform(response.begin(), response.end(), response.begin(), ::tolower);
    return response == "o" || response == "oui";
}

std::string Chatbot::normalizeWord(const std::string& word) const {
    std::string normalized = word;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    normalized.erase(std::remove_if(normalized.begin(), normalized.end(), ::ispunct), normalized.end());
    return normalized;
}

std::string Chatbot::sendRequest(const std::string& prompt) {
    nlohmann::json payload = buildPayload(prompt);
    cpr::Response r = makeApiRequest(payload);
    
    if (r.status_code == 200) {
        return handleSuccessfulResponse(r);
    } else {
        return handleErrorResponse(r);
    }
}

nlohmann::json Chatbot::buildPayload(const std::string& prompt) {
    nlohmann::json payload = {
        {"model", model},
        {"messages", {
            {
                {"role", "user"},
                {"content", prompt}
            }
        }}
    };

    if (debug_mode) {
        std::cout << "Payload construit : " << payload.dump(2) << std::endl;
    }

    return payload;
}

bool Chatbot::isLlamaModel() {
    return model.find("llama") != std::string::npos;
}

void Chatbot::addLlamaSpecificOptions(nlohmann::json& payload) {
    payload["top_p"] = 0.9;
    payload["frequency_penalty"] = 0.0;
    payload["presence_penalty"] = 0.0;
}

cpr::Response Chatbot::makeApiRequest(const nlohmann::json& payload) {
    if (debug_mode) {
        std::cout << "Tentative de connexion à : " << server_url << std::endl;
        std::cout << "Payload : " << payload.dump(2) << std::endl;
    }

    cpr::Response response = cpr::Post(
        cpr::Url{server_url},
        cpr::Header{
            {"Content-Type", "application/json"},
            {"Authorization", "Bearer " + api_key}
        },
            cpr::Body{payload.dump()},
        cpr::Timeout{30000}  // 30 secondes de timeout
        );

        if (debug_mode) {
        std::cout << "Code de réponse : " << response.status_code << std::endl;
        if (response.status_code != 200) {
            std::cout << "Erreur : " << response.text << std::endl;
        }
    }

    return response;
}

std::string Chatbot::handleSuccessfulResponse(const cpr::Response& r) {
    try {
        nlohmann::json response = nlohmann::json::parse(r.text);
        return extractContent(response);
    } catch (const std::exception& e) {
        last_error = "Erreur lors du parsing de la réponse : " + std::string(e.what());
        return "Désolé, une erreur s'est produite.";
    }
}

std::string Chatbot::extractContent(const nlohmann::json& response) {
    if (debug_mode) {
        std::cout << "Réponse complète : " << response.dump(2) << std::endl;
    }

    if (response.contains("choices") && !response["choices"].empty() &&
        response["choices"][0].contains("message") &&
                        response["choices"][0]["message"].contains("content")) {
        return response["choices"][0]["message"]["content"].get<std::string>();
    }
    return "Désolé, je n'ai pas pu générer une réponse.";
}

std::string Chatbot::handleErrorResponse(const cpr::Response& r) {
    last_error = "Erreur API : " + std::to_string(r.status_code) + " - " + r.text;
    return "Désolé, une erreur s'est produite lors de la communication avec l'API.";
}

bool Chatbot::testConnection() {
    const int MAX_RETRIES = 3;
    const int RETRY_DELAY_MS = 1000;  // 1 seconde

    if (debug_mode) {
        std::cout << "Test de connexion au serveur : " << server_url << std::endl;
        std::cout << "Modèle : " << model << std::endl;
    }

    for (int attempt = 1; attempt <= MAX_RETRIES; attempt++) {
        try {
            std::string test_prompt = "Test de connexion";
            nlohmann::json payload = buildPayload(test_prompt);
            cpr::Response r = makeApiRequest(payload);
            
            if (r.status_code == 200) {
                if (debug_mode) {
                    std::cout << "✓ Connexion à l'API établie avec succès (tentative " << attempt << "/" << MAX_RETRIES << ")" << std::endl;
                }
                return true;
            } else {
                if (debug_mode) {
                    std::cout << "⚠️ Tentative " << attempt << "/" << MAX_RETRIES 
                              << " échouée. Code: " << r.status_code << std::endl;
                    std::cout << "Réponse du serveur : " << r.text << std::endl;
                }
                
                if (attempt < MAX_RETRIES) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_DELAY_MS));
                }
                }
            } catch (const std::exception& e) {
            if (debug_mode) {
                std::cout << "⚠️ Tentative " << attempt << "/" << MAX_RETRIES 
                          << " échouée. Erreur: " << e.what() << std::endl;
            }
            
            if (attempt < MAX_RETRIES) {
                std::this_thread::sleep_for(std::chrono::milliseconds(RETRY_DELAY_MS));
            }
        }
    }

    last_error = "Échec de la connexion après " + std::to_string(MAX_RETRIES) + " tentatives";
    return false;
}

void Chatbot::setUserThinking() {
    reset();
    
    // Choisir aléatoirement une catégorie et un mot
    thinkOfWord();
    
    // Modifier l'affichage pour le mode où l'IA devine
    first_display = true;
    
    // Vérifier l'état de connexion de l'API
    bool is_api_connected = isConnected();
    
    // Nettoyer l'écran
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    std::cout << "\n🎲 --- Mode: L'IA devine votre mot --- 🎲\n\n";
    std::cout << "=== Jeu de Devinettes ===\n";
    std::cout << "Pensez à un mot dans la catégorie : " << chosen_category << "\n";
    std::cout << "L'IA va vous poser des questions pour le deviner.\n";
    std::cout << "Répondez par 'oui' ou 'non' à ses questions.\n";
    std::cout << "Commandes spéciales :\n";
    std::cout << "  - /reset : Réinitialiser le jeu\n";
    std::cout << "  - /stats : Afficher les statistiques\n";
    std::cout << "  - /quit : Quitter le jeu\n\n";
    
    // Afficher l'état de connexion en bas à droite
    if (is_api_connected) {
        // Code ANSI pour le texte vert
        std::cout << "\033[32mCONNEXION\033[0m";
    } else {
        // Code ANSI pour le texte rouge
        std::cout << "\033[31mHORS CONNEXION\033[0m";
    }
    std::cout << std::endl << std::endl;
    
    // Créer un contexte pour l'IA
    context = "Tu es un assistant qui joue à un jeu de devinettes où tu dois deviner le mot auquel l'utilisateur pense. "
              "Tu dois poser des questions stratégiques auxquelles on peut répondre par oui ou non. "
              "Tu dois analyser attentivement les réponses précédentes pour ne pas poser de questions contradictoires. "
              "Sois méthodique et logique dans ta progression de questions. "
              "Quand tu penses avoir trouvé le mot, propose-le directement en demandant 'Est-ce que c'est X?'";
    
    gameEnded = false;
    history.clear();
    reponses.clear();
    
    // Poser la première question
    std::string first_question = guess("");
    history.push_back(first_question);
    
    // Afficher la première question
    std::cout << "Question 1: " << first_question << "\n";
    std::cout << "Votre réponse (oui/non): ";
}

std::string Chatbot::guess(const std::string& last_answer) {
    if (gameEnded) {
        return "Le jeu est terminé. Utilisez /reset pour recommencer.";
    }

    // Ajouter la dernière réponse à l'historique si ce n'est pas la première question
    if (history.size() > 0 && !last_answer.empty()) {
        reponses.push_back(last_answer);
        
        // Vérifier si l'IA a deviné correctement
        std::string last_question = history.back();
        if (last_question.find("Est-ce que c'est") != std::string::npos && 
            last_answer == "oui") {
            gameEnded = true;
            total_games++;
            // L'IA a gagné
            return "Super ! J'ai deviné votre mot. Voulez-vous jouer à nouveau ? (/reset pour recommencer)";
        }
    }

    // Construire un prompt amélioré pour générer des questions plus stratégiques
    std::string prompt = std::string("Tu es un expert en jeu de devinettes où tu dois deviner un mot auquel l'utilisateur pense. ")
                         + "Tu dois poser des questions auxquelles on peut répondre par oui ou non. "
                         + "Sois méthodique, stratégique et logique dans tes questions.\n\n";

    // Ajouter des instructions détaillées sur la stratégie à adopter
    prompt += "OBJECTIF: Poser une question stratégique qui divise efficacement l'espace des possibilités.\n\n";
    
    // Ajouter des stratégies spécifiques selon le nombre de questions déjà posées
    if (history.size() == 0) {
        prompt += "C'est ta première question. Commence par une question générale sur la nature du mot "
                 "(objet, être vivant, concept, etc.) pour établir une première catégorie large.\n";
    } else if (history.size() == 1) {
        prompt += "Pour ta deuxième question, affine la catégorie principale en fonction de la réponse précédente.\n";
        prompt += "- Si c'est un objet, demande sur son utilisation ou son environnement\n";
        prompt += "- Si c'est un être vivant, demande s'il s'agit d'un animal, végétal, etc.\n";
        prompt += "- Si c'est un concept, demande s'il est concret ou abstrait\n";
    } else if (history.size() < 5) {
        prompt += "À ce stade, pose des questions sur les caractéristiques principales du mot:\n";
        prompt += "- Caractéristiques physiques: taille, forme, couleur, matériau\n";
        prompt += "- Fonction ou utilisation: à quoi ça sert, comment on l'utilise\n";
        prompt += "- Localisation: où on le trouve habituellement\n";
        prompt += "- Temporalité: quand on l'utilise ou quand on le voit\n";
    } else if (history.size() < 8) {
        prompt += "Tu as déjà posé plusieurs questions. Cible maintenant des caractéristiques plus spécifiques "
                 "et élimine des possibilités. Utilise une approche de recherche binaire pour réduire "
                 "rapidement l'espace des possibilités.\n";
    } else {
        prompt += "Tu as déjà posé beaucoup de questions. À ce stade, tu devrais avoir une idée assez précise. "
                 "Si tu as une hypothèse forte, propose directement le mot en demandant 'Est-ce que c'est [mot]?'.\n";
    }

    // Ajouter l'historique des questions et réponses avec une analyse détaillée
    if (!history.empty() && !reponses.empty()) {
        prompt += "\nVoici l'historique complet des questions et réponses:\n";
        for (size_t i = 0; i < history.size() && i < reponses.size(); ++i) {
            prompt += "Q" + std::to_string(i+1) + ": " + history[i] + "\n";
            prompt += "R" + std::to_string(i+1) + ": " + reponses[i] + "\n";
        }
        
        // Ajouter une analyse structurée de l'historique
        prompt += "\nAnalyse systématique de ce que tu as appris:\n";
        prompt += "1. CARACTÉRISTIQUES CONFIRMÉES: Liste toutes les caractéristiques que tu sais être vraies\n";
        prompt += "2. CARACTÉRISTIQUES EXCLUES: Liste toutes les caractéristiques que tu sais être fausses\n";
        prompt += "3. HYPOTHÈSES ACTUELLES: Quels sont les mots possibles qui correspondent à ces caractéristiques?\n";
        prompt += "4. QUESTIONS REDONDANTES À ÉVITER: Ne pose pas de questions similaires à celles déjà posées\n";
        
        // Encourager l'IA à faire une proposition après plusieurs questions
        if (history.size() >= 5) {
            prompt += "\nTu as déjà posé " + std::to_string(history.size()) + " questions. ";
            prompt += "Si tu as une bonne idée du mot, n'hésite pas à faire une proposition directe.\n";
        }
        
        if (history.size() >= 8) {
            prompt += "\nTu devrais sérieusement envisager de proposer un mot maintenant. ";
            prompt += "Analyse les informations recueillies et propose le mot le plus probable.\n";
        }
        
        if (history.size() >= 10) {
            prompt += "\nATTENTION: Tu dois absolument faire une proposition maintenant. ";
            prompt += "Demande 'Est-ce que c'est [ta meilleure supposition]?'\n";
        }
    }
    
    // Instructions finales pour la formulation de la question
    prompt += "\nPose maintenant UNE SEULE question stratégique à laquelle on peut répondre par oui ou non. ";
    prompt += "Ta question doit être claire, précise, non ambiguë et formulée en français correct. ";
    prompt += "Assure-toi que ta question n'est pas redondante avec les précédentes et qu'elle apporte ";
    prompt += "réellement de nouvelles informations utiles pour identifier le mot.";

    try {
        // Faire plusieurs requêtes et choisir la meilleure question
        std::vector<std::string> candidate_questions;
        const int NUM_CANDIDATES = 2;
        
        for (int i = 0; i < NUM_CANDIDATES; i++) {
            std::string response = sendRequest(prompt);
            
            // S'assurer que la réponse est bien une question
            if (response.find("?") == std::string::npos) {
                response += " ?";
            }
            
            candidate_questions.push_back(response);
        }
        
        // Choisir la meilleure question (celle qui est la plus claire et stratégique)
        // Pour simplifier, on prend la première, mais on pourrait implémenter une heuristique plus complexe
        std::string best_question = candidate_questions[0];
        
        // Vérifier si la question est similaire à une question précédente
        bool is_similar = false;
        for (const auto& prev_question : history) {
            if (similarQuestions(best_question, prev_question)) {
                is_similar = true;
                break;
            }
        }
        
        // Si la question est similaire, essayer l'autre candidat ou générer une nouvelle question
        if (is_similar && candidate_questions.size() > 1) {
            best_question = candidate_questions[1];
        } else if (is_similar) {
            // Générer une nouvelle question en spécifiant d'éviter les questions similaires
            std::string new_prompt = prompt + "\n\nATTENTION: Ta précédente question était trop similaire à une question déjà posée. "
                                    "Pose une question COMPLÈTEMENT DIFFÉRENTE des questions précédentes.";
            best_question = sendRequest(new_prompt);
            
            if (best_question.find("?") == std::string::npos) {
                best_question += " ?";
            }
        }
        
        // Ajouter à l'historique
        history.push_back(best_question);
        
        return best_question;
    } catch (const std::exception& e) {
        last_error = "Erreur lors de la communication avec l'IA: " + std::string(e.what());
        return "Erreur: Impossible de générer une question. Veuillez réessayer.";
    }
}

bool Chatbot::similarQuestions(const std::string& q1, const std::string& q2) {
    // Normaliser les deux questions
    std::string n1 = normalizeWord(q1);
    std::string n2 = normalizeWord(q2);
    
    // Extraire les mots significatifs de chaque question
    std::vector<std::string> words1;
    std::istringstream iss1(n1);
    std::string word;
    while (iss1 >> word) {
        if (word.length() > 3) { // Ignorer les mots courts
            words1.push_back(word);
        }
    }
    
    std::vector<std::string> words2;
    std::istringstream iss2(n2);
    while (iss2 >> word) {
        if (word.length() > 3) { // Ignorer les mots courts
            words2.push_back(word);
        }
    }
    
    // Compter les mots communs
    int common_words = 0;
    for (const auto& word : words1) {
        if (n2.find(word) != std::string::npos) {
            common_words++;
        }
    }
    
    // Mots spécifiques qui indiquent des questions sur les mêmes attributs
    std::vector<std::pair<std::string, std::string>> related_attributes = {
        {"rouge", "couleur"}, {"bleu", "couleur"}, {"vert", "couleur"}, {"jaune", "couleur"}, {"noir", "couleur"},
        {"blanc", "couleur"}, {"orange", "couleur"}, {"violet", "couleur"}, {"marron", "couleur"}, {"gris", "couleur"},
        {"grand", "taille"}, {"petit", "taille"}, {"moyen", "taille"}, {"enorme", "taille"}, {"minuscule", "taille"},
        {"rond", "forme"}, {"carre", "forme"}, {"triangulaire", "forme"}, {"ovale", "forme"}, {"rectangulaire", "forme"},
        {"mange", "usage"}, {"boit", "usage"}, {"utilise", "usage"}, {"sert", "usage"}, {"joue", "usage"},
        {"animal", "categorie"}, {"fruit", "categorie"}, {"legume", "categorie"}, {"objet", "categorie"}, {"ville", "categorie"},
        {"pays", "categorie"}, {"sport", "categorie"}, {"metier", "categorie"}
    };
    
    // Vérifier si les questions portent sur des attributs liés
    bool related_attribute_questions = false;
    
    for (const auto& pair : related_attributes) {
        bool attr1_in_q1 = n1.find(pair.first) != std::string::npos;
        bool attr1_in_q2 = n2.find(pair.first) != std::string::npos;
        bool attr2_in_q1 = n1.find(pair.second) != std::string::npos;
        bool attr2_in_q2 = n2.find(pair.second) != std::string::npos;
        
        // Si les deux questions contiennent le même attribut spécifique
        if ((attr1_in_q1 && attr1_in_q2) || 
            // Ou si les deux questions parlent de la même catégorie d'attribut
            (attr2_in_q1 && attr2_in_q2) ||
            // Ou si une question contient l'attribut spécifique et l'autre la catégorie
            (attr1_in_q1 && attr2_in_q2) || (attr2_in_q1 && attr1_in_q2)) {
            related_attribute_questions = true;
            break;
        }
    }
    
    // Vérifier les questions de type oui/non qui sont formulées différemment mais ont le même sens
    bool similar_structure = false;
    std::vector<std::string> question_patterns = {
        "est", "est-ce", "est ce", "est-il", "est il", "est-elle", "est elle", 
        "a-t-il", "a t il", "a-t-elle", "a t elle", "peut", "peut-on", "peut on"
    };
    
    for (const auto& pattern : question_patterns) {
        if ((n1.find(pattern) != std::string::npos && n2.find(pattern) != std::string::npos)) {
            similar_structure = true;
            break;
        }
    }
    
    // Calculer le pourcentage de mots communs pour la première question
    double similarity_ratio1 = (words1.size() > 0) ? 
                              static_cast<double>(common_words) / words1.size() : 0.0;
    
    // Calculer le nombre de mots communs de la deuxième question vers la première
    int common_words2 = 0;
    for (const auto& word : words2) {
        if (n1.find(word) != std::string::npos) {
            common_words2++;
        }
    }
    
    // Calculer le pourcentage de mots communs pour la deuxième question
    double similarity_ratio2 = (words2.size() > 0) ? 
                              static_cast<double>(common_words2) / words2.size() : 0.0;
    
    // Prendre le ratio le plus élevé pour la décision finale
    double max_similarity = std::max(similarity_ratio1, similarity_ratio2);
    
    // Si plus de 60% des mots significatifs sont communs, ou si les questions portent sur des attributs liés
    // et ont une structure similaire avec au moins 30% de mots communs
    return (max_similarity > 0.6) || 
           (related_attribute_questions && similar_structure && max_similarity > 0.3);
}

bool Chatbot::checkLogicalConsistency(const std::string& question, const std::string& proposed_answer) {
    // Normaliser la question et la réponse proposée
    std::string norm_question = normalizeWord(question);
    std::string norm_answer = normalizeWord(proposed_answer);
    bool is_yes = (norm_answer == "oui");
    bool is_no = (norm_answer == "non");
    
    if (!is_yes && !is_no) {
        // Si la réponse n'est ni "oui" ni "non", on ne peut pas vérifier la cohérence
        return true;
    }
    
    // Vérifier la cohérence avec les attributs connus du mot
    std::string normalized_word = normalizeWord(chosen_word);
    if (factual_knowledge.find(normalized_word) != factual_knowledge.end()) {
        // Parcourir les attributs connus du mot
        for (const auto& attr_pair : factual_knowledge[normalized_word]) {
            const std::string& attribute = attr_pair.first;
            bool attribute_value = attr_pair.second;
            
            // Si la question contient cet attribut
            if (norm_question.find(attribute) != std::string::npos) {
                // La réponse doit être cohérente avec la valeur connue de l'attribut
                if ((attribute_value && is_no) || (!attribute_value && is_yes)) {
                    if (debug_mode) {
                        std::cout << "Incohérence détectée: attribut '" << attribute 
                                  << "' a la valeur " << (attribute_value ? "true" : "false") 
                                  << " mais la réponse proposée est " << (is_yes ? "oui" : "non") << std::endl;
                    }
                    return false;
                }
                return true;
            }
        }
    }
    
    // Vérifier la cohérence avec l'historique des questions et réponses
    for (size_t i = 0; i < history.size() && i < reponses.size(); ++i) {
        std::string prev_question = normalizeWord(history[i]);
        std::string prev_answer = normalizeWord(reponses[i]);
        bool prev_is_yes = (prev_answer == "oui");
        bool prev_is_no = (prev_answer == "non");
        
        // Si les questions sont similaires, les réponses devraient être cohérentes
        if (similarQuestions(question, history[i])) {
            if ((prev_is_yes && is_no) || (prev_is_no && is_yes)) {
                if (debug_mode) {
                    std::cout << "Incohérence détectée avec une question similaire précédente:" << std::endl;
                    std::cout << "Question précédente: " << history[i] << " (Réponse: " << reponses[i] << ")" << std::endl;
                    std::cout << "Question actuelle: " << question << " (Réponse proposée: " << proposed_answer << ")" << std::endl;
                }
                return false;
            }
            return true;
        }
        
        // Vérifier les contradictions logiques pour certains types de questions
        
        // Cas 1: Questions sur des attributs mutuellement exclusifs (couleurs, formes, etc.)
        std::vector<std::vector<std::string>> mutually_exclusive_attrs = {
            {"rouge", "bleu", "vert", "jaune", "noir", "blanc", "orange", "violet", "marron", "gris"},
            {"rond", "carre", "triangulaire", "ovale", "rectangulaire"}
        };
        
        for (const auto& attr_group : mutually_exclusive_attrs) {
            std::string attr1, attr2;
            bool found_in_current = false, found_in_prev = false;
            
            // Trouver les attributs dans les questions
            for (const auto& attr : attr_group) {
                if (norm_question.find(attr) != std::string::npos) {
                    attr1 = attr;
                    found_in_current = true;
                }
                if (prev_question.find(attr) != std::string::npos) {
                    attr2 = attr;
                    found_in_prev = true;
                }
            }
            
            // Si les deux questions concernent des attributs différents mais mutuellement exclusifs
            if (found_in_current && found_in_prev && attr1 != attr2) {
                // Si les deux réponses sont "oui", c'est incohérent
                if (prev_is_yes && is_yes) {
                    if (debug_mode) {
                        std::cout << "Incohérence détectée: attributs mutuellement exclusifs '" 
                                  << attr1 << "' et '" << attr2 << "' ne peuvent pas être tous les deux vrais" << std::endl;
                    }
                    return false;
                }
            }
        }
        
        // Cas 2: Questions sur la négation d'un attribut
        if (prev_question.find("pas ") != std::string::npos || prev_question.find("ne ") != std::string::npos) {
            // Extraire l'attribut nié
            size_t neg_pos = prev_question.find("pas ");
            if (neg_pos == std::string::npos) neg_pos = prev_question.find("ne ");
            
            if (neg_pos != std::string::npos) {
                // Chercher les mots significatifs après la négation
                std::string after_neg = prev_question.substr(neg_pos + 4); // "pas " ou "ne " + espace
                std::istringstream iss(after_neg);
                std::string word;
                while (iss >> word) {
                    if (word.length() > 3 && norm_question.find(word) != std::string::npos) {
                        // Si la question actuelle contient le même attribut que celui qui était nié
                        // et que les réponses sont incohérentes
                        if ((prev_is_yes && is_yes) || (prev_is_no && is_no)) {
                            if (debug_mode) {
                                std::cout << "Incohérence détectée: négation d'attribut" << std::endl;
                                std::cout << "Question précédente (avec négation): " << history[i] << " (Réponse: " << reponses[i] << ")" << std::endl;
                                std::cout << "Question actuelle: " << question << " (Réponse proposée: " << proposed_answer << ")" << std::endl;
                            }
                            return false;
                        }
                    }
                }
            }
        }
    }
    
    // Si aucune incohérence n'a été détectée
    return true;
}

void Chatbot::initializeWordKnowledge() {
    // Réinitialiser les bases de connaissances
    factual_knowledge.clear();
    synonyms.clear();
    
    // Initialiser les synonymes généraux
    synonyms["animal"] = {"bete", "creature", "faune", "bête"};
    synonyms["fruit"] = {"nourriture", "aliment", "comestible"};
    synonyms["legume"] = {"nourriture", "aliment", "comestible", "légume"};
    synonyms["objet"] = {"chose", "article", "item"};
    synonyms["boisson"] = {"boire", "liquide", "breuvage"};
    synonyms["couleur"] = {"teinte", "coloration", "coloris"};
    synonyms["taille"] = {"dimension", "grandeur", "mesure", "volume"};
    synonyms["forme"] = {"apparence", "aspect", "figure", "silhouette"};
    
    // Initialiser la base de connaissances pour les mots spécifiques
    std::string normalized_word = normalizeWord(chosen_word);
    
    // Connaissances pour les animaux
    if (normalized_word == "singe") {
        factual_knowledge[normalized_word]["primate"] = true;
        factual_knowledge[normalized_word]["grimpe"] = true;
        factual_knowledge[normalized_word]["banane"] = true;
        factual_knowledge[normalized_word]["arbre"] = true;
        factual_knowledge[normalized_word]["queue"] = true;
        factual_knowledge[normalized_word]["poil"] = true;
        factual_knowledge[normalized_word]["intelligent"] = true;
        factual_knowledge[normalized_word]["domestique"] = false;
    }
    else if (normalized_word == "chien") {
        factual_knowledge[normalized_word]["animal"] = true;
        factual_knowledge[normalized_word]["mammifere"] = true;
        factual_knowledge[normalized_word]["domestique"] = true;
        factual_knowledge[normalized_word]["aboie"] = true;
        factual_knowledge[normalized_word]["quatre pattes"] = true;
        factual_knowledge[normalized_word]["trois pattes"] = false;
        factual_knowledge[normalized_word]["cinq pattes"] = false;
        factual_knowledge[normalized_word]["poil"] = true;
        factual_knowledge[normalized_word]["compagnie"] = true;
        factual_knowledge[normalized_word]["fidele"] = true;
        factual_knowledge[normalized_word]["os"] = true;
        factual_knowledge[normalized_word]["croquette"] = true;
        factual_knowledge[normalized_word]["nourriture pour chien"] = true;
    }
    else if (normalized_word == "girafe") {
        factual_knowledge[normalized_word]["long cou"] = true;
        factual_knowledge[normalized_word]["taches"] = true;
        factual_knowledge[normalized_word]["grande"] = true;
        factual_knowledge[normalized_word]["afrique"] = true;
        factual_knowledge[normalized_word]["herbivore"] = true;
        factual_knowledge[normalized_word]["savane"] = true;
        factual_knowledge[normalized_word]["domestique"] = false;
    }
    
    // Connaissances pour les objets
    else if (normalized_word == "tasse") {
        factual_knowledge[normalized_word]["boire"] = true;
        factual_knowledge[normalized_word]["cuisine"] = true;
        factual_knowledge[normalized_word]["ceramique"] = true;
        factual_knowledge[normalized_word]["porcelaine"] = true;
        factual_knowledge[normalized_word]["cafe"] = true;
        factual_knowledge[normalized_word]["the"] = true;
        factual_knowledge[normalized_word]["liquide"] = true;
        factual_knowledge[normalized_word]["boisson"] = true;
        factual_knowledge[normalized_word]["contenant"] = true;
        factual_knowledge[normalized_word]["recipient"] = true;
    }
    
    // Connaissances pour les légumes
    else if (normalized_word == "poireau" || normalized_word == "poireaux") {
        factual_knowledge[normalized_word]["legume"] = true;
        factual_knowledge[normalized_word]["comestible"] = true;
        factual_knowledge[normalized_word]["vert"] = true;
        factual_knowledge[normalized_word]["blanc"] = true;
        factual_knowledge[normalized_word]["soupe"] = true;
        factual_knowledge[normalized_word]["cuisine"] = true;
        factual_knowledge[normalized_word]["potager"] = true;
        factual_knowledge[normalized_word]["long"] = true;
        factual_knowledge[normalized_word]["terre"] = true;
        factual_knowledge[normalized_word]["feuilles"] = true;
    }
    
    // Connaissances pour les pays
    else if (normalized_word == "suisse") {
        factual_knowledge[normalized_word]["europe"] = true;
        factual_knowledge[normalized_word]["montagne"] = true;
        factual_knowledge[normalized_word]["alpes"] = true;
        factual_knowledge[normalized_word]["fromage"] = true;
        factual_knowledge[normalized_word]["chocolat"] = true;
        factual_knowledge[normalized_word]["neutre"] = true;
        factual_knowledge[normalized_word]["banque"] = true;
        factual_knowledge[normalized_word]["horlogerie"] = true;
        factual_knowledge[normalized_word]["montre"] = true;
        factual_knowledge[normalized_word]["croix"] = true;
        factual_knowledge[normalized_word]["rouge"] = true;
    }
    else if (normalized_word == "canada") {
        factual_knowledge[normalized_word]["amerique"] = true;
        factual_knowledge[normalized_word]["nord"] = true;
        factual_knowledge[normalized_word]["froid"] = true;
        factual_knowledge[normalized_word]["erable"] = true;
        factual_knowledge[normalized_word]["sirop"] = true;
        factual_knowledge[normalized_word]["anglais"] = true;
        factual_knowledge[normalized_word]["francais"] = true;
        factual_knowledge[normalized_word]["grand"] = true;
    }
    
    // Connaissances pour les villes
    else if (normalized_word == "berlin") {
        factual_knowledge[normalized_word]["allemagne"] = true;
        factual_knowledge[normalized_word]["europe"] = true;
        factual_knowledge[normalized_word]["capitale"] = true;
        factual_knowledge[normalized_word]["mur"] = true;
        factual_knowledge[normalized_word]["histoire"] = true;
        factual_knowledge[normalized_word]["grand"] = true;
    }
    
    // Connaissances pour les sports
    else if (normalized_word == "escalade") {
        factual_knowledge[normalized_word]["grimpe"] = true;
        factual_knowledge[normalized_word]["grimper"] = true;
        factual_knowledge[normalized_word]["montagne"] = true;
        factual_knowledge[normalized_word]["rocher"] = true;
        factual_knowledge[normalized_word]["corde"] = true;
        factual_knowledge[normalized_word]["mousqueton"] = true;
        factual_knowledge[normalized_word]["ampoule"] = true;
        factual_knowledge[normalized_word]["main"] = true;
        factual_knowledge[normalized_word]["doigt"] = true;
        factual_knowledge[normalized_word]["securite"] = true;
        factual_knowledge[normalized_word]["hauteur"] = true;
        factual_knowledge[normalized_word]["individuel"] = true;
    }
    
    // On pourrait ajouter d'autres mots au besoin...
    
    if (debug_mode) {
        std::cout << "Base de connaissances initialisée pour le mot: " << chosen_word << std::endl;
        if (factual_knowledge.find(normalized_word) != factual_knowledge.end()) {
            std::cout << "Nombre d'attributs: " << factual_knowledge[normalized_word].size() << std::endl;
        }
    }
}

bool Chatbot::isConnected() {
    // Si nous n'avons pas encore testé la connexion ou si nous voulons la retester périodiquement
    if (!connection_tested) {
        is_connected = testConnection();
        connection_tested = true;
        
        if (debug_mode) {
            std::cout << "Test de connexion effectué: " << (is_connected ? "Connecté" : "Non connecté") << std::endl;
        }
    }
    
    return is_connected;
}

void Chatbot::checkConnectionStatus() {
    connection_tested = false; // Force un nouveau test
    is_connected = testConnection();
    connection_tested = true;
}