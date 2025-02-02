#include <iostream>
using namespace std;

class Date {
private:
    int M_jour, M_mois, M_annee;

public:
    // Constructeur par défaut
    Date(int jour = 1, int mois = 1, int annee = 2000) {
        if (est_valide(jour, mois, annee)) {
            M_jour = jour;
            M_mois = mois;
            M_annee = annee;
        } else {
            M_jour = 1;
            M_mois = 1;
            M_annee = 2000;
            cout << "Date invalide. Initialisation à 01/01/2000." << endl;
        }
    }

    // Méthode pour afficher la date sous la forme J/M/A
    void afficher() const {
        cout << M_jour << "/" << M_mois << "/" << M_annee << endl;
    }

    // Méthode statique pour vérifier si la date est valide
    static bool est_valide(int jour, int mois, int annee) {
        // Vérification de la validité du mois
        if (mois < 1 || mois > 12) {
            cout << "Mois invalide." << endl;
            return false;
        }

        // Vérification de la validité du jour en fonction du mois
        int jours_max = 0;

        // Tableau des jours maximum pour chaque mois
        switch (mois) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                jours_max = 31;
                break;
            case 4: case 6: case 9: case 11:
                jours_max = 30;
                break;
            case 2:
                jours_max = 28;  // Février a toujours 28 jours selon l'énoncé
                break;
        }

        if (jour < 1 || jour > jours_max) {
            cout << "Jour invalide pour le mois " << mois << "." << endl;
            return false;
        }

        return true;
    }

    // Mutateur pour le jour
    void set_jour(int jour) {
        if (est_valide(jour, M_mois, M_annee)) {
            M_jour = jour;
        } else {
            cout << "Jour invalide. La date n'a pas été modifiée." << endl;
        }
    }

    // Mutateur pour le mois
    void set_mois(int mois) {
        if (est_valide(M_jour, mois, M_annee)) {
            M_mois = mois;
        } else {
            cout << "Mois invalide. La date n'a pas été modifiée." << endl;
        }
    }

    // Mutateur pour l'année
    void set_annee(int annee) {
        if (est_valide(M_jour, M_mois, annee)) {
            M_annee = annee;
        } else {
            cout << "Année invalide. La date n'a pas été modifiée." << endl;
        }
    }
};

int main() {
    Date d1(29, 2, 2024); // Exemple de date valide (année bissextile)
    d1.afficher();

    Date d2(31, 4, 2023); // Exemple de date invalide (mois d'avril a 30 jours)
    d2.afficher();

    Date d3;
    d3.set_jour(30); // Modification valide
    d3.set_mois(2);  // Modification invalide (février a 28 jours)
    d3.set_annee(2022); // Modification valide
    d3.afficher();

    return 0;
}




