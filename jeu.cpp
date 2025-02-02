#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Structure pour représenter une question et ses réponses possibles
struct Question {
    string questionText;
    string reponseOui;
    string reponseNon;
};

// Structure pour représenter un objet/élément à deviner
struct Objet {
    string nom;
    vector<Question> questions;

    // Constructeur pour initialiser facilement un Objet
    Objet(string n, vector<Question> q) : nom(n), questions(q) {}
};

void jouerJeu(const vector<Objet>& objets) {
    string reponseUtilisateur;
    bool trouve = false;
    
    // Choisir un objet aléatoire (ici on prend toujours le premier objet)
    Objet objetADeviner = objets[0];

    cout << "Bienvenue dans le jeu 'Est-ce que c'est ?'\n";
    cout << "J'ai un objet en tête. Pose-moi des questions fermées pour le deviner !\n";
    
    // Poser les questions jusqu'à ce qu'on trouve la réponse
    for (auto& question : objetADeviner.questions) {
        cout << question.questionText << " (oui/non): ";
        cin >> reponseUtilisateur;

        if (reponseUtilisateur == "oui") {
            cout << question.reponseOui << endl;
        } else if (reponseUtilisateur == "non") {
            cout << question.reponseNon << endl;
        } else {
            cout << "Je ne comprends pas cette réponse. Veuillez répondre par 'oui' ou 'non'.\n";
            continue; // Répéter la question si la réponse est invalide
        }
    }

    cout << "Est-ce que tu veux deviner l'objet ? (oui/non): ";
    cin >> reponseUtilisateur;
    if (reponseUtilisateur == "oui") {
        cout << "L'objet que j'avais en tête est : " << objetADeviner.nom << endl;
        trouve = true;
    }
    
    if (!trouve) {
        cout << "Désolé, je n'ai pas trouvé l'objet.\n";
    }
}

int main() {
    // Créer les objets et leurs questions avec le constructeur explicite
    vector<Objet> objets;
    
    // Utilisation de push_back pour ajouter des objets et leurs questions
    vector<Question> questionsChat = {
        {"Est-ce que c'est un animal ?", "C'est un animal domestique.", "Ce n'est pas un animal."},
        {"Est-ce qu'il a des poils ?", "Il a des poils.", "Il n'a pas de poils."},
        {"Est-ce qu'il miaule ?", "Il miaule.", "Il ne miaule pas."}
    };
    objets.push_back(Objet("chat", questionsChat));

    vector<Question> questionsVoiture = {
        {"Est-ce que c'est un véhicule ?", "C'est un véhicule motorisé.", "Ce n'est pas un véhicule."},
        {"Est-ce qu'il a quatre roues ?", "Il a quatre roues.", "Il n'a pas quatre roues."},
        {"Est-ce qu'il roule sur la route ?", "Il roule sur la route.", "Il ne roule pas sur la route."}
    };
    objets.push_back(Objet("voiture", questionsVoiture));

    vector<Question> questionsArbre = {
        {"Est-ce que c'est une plante ?", "C'est une plante.", "Ce n'est pas une plante."},
        {"Est-ce qu'il a des racines ?", "Il a des racines.", "Il n'a pas de racines."},
        {"Est-ce qu'il pousse dehors ?", "Il pousse dehors.", "Il ne pousse pas dehors."}
    };
    objets.push_back(Objet("arbre", questionsArbre));

    jouerJeu(objets);  // Commencer le jeu

    return 0;
}
