#include <iostream>
#include <stdexcept> // Pour std::invalid_argument et exceptions

class File {
    int M_size;       // Taille maximale de la file
    int* M_elem;      // Tableau dynamique pour stocker les éléments
    int M_debut;      // Index du début de la file
    int M_fin;        // Index de la fin de la file
    int M_count;      // Nombre d'éléments actuels dans la file

public:
    // Constructeur
    File(int size) {
        if (size <= 0) {
            throw std::invalid_argument("La taille doit être positive.");
        }
        M_size = size;
        M_elem = new int[M_size];
        M_debut = 0;
        M_fin = -1;
        M_count = 0;
    }

    // Constructeur par copie
    File(const File& b) {
        M_size = b.M_size;
        M_elem = new int[M_size];
        M_debut = b.M_debut;
        M_fin = b.M_fin;
        M_count = b.M_count;

        for (int i = 0; i < M_count; ++i) {
            M_elem[(M_debut + i) % M_size] = b.M_elem[(b.M_debut + i) % b.M_size];
        }
    }

    // Initialisation de la file avec des valeurs
    void init() {
        for (int i = 1; i <= M_size - 5; ++i) {
            Enfiler(i); // Ajoute les valeurs 1, 2, 3, ...
        }
    }

    // Affichage des éléments de la file
    void print() {
        for (int i = 0; i < M_count; ++i) {
            std::cout << M_elem[(M_debut + i) % M_size] << " ";
        }
        std::cout << std::endl;
    }

    // Ajouter un élément à la fin de la file (Enfiler)
    void Enfiler(int val) {
        if (M_count >= M_size) {
            throw std::out_of_range("La file est pleine.");
        }
        M_fin = (M_fin + 1) % M_size;
        M_elem[M_fin] = val;
        ++M_count;
    }

    // Retirer un élément du début de la file (Defiler)
    void Defiler() {
        if (M_count == 0) {
            throw std::out_of_range("La file est vide.");
        }
        M_debut = (M_debut + 1) % M_size;
        --M_count;
    }

    // Consulter l'élément au début de la file
    int Front() {
        if (M_count == 0) {
            throw std::out_of_range("La file est vide.");
        }
        return M_elem[M_debut];
    }

    // Destructeur
    ~File() {
        delete[] M_elem;
    }

    // Surcharge de l'opérateur + pour concaténer deux files
    friend File operator+(const File& a, const File& b);
};

// Surcharge de l'opérateur + pour concaténer deux files
File operator+(const File& a, const File& b) {
    File c(a.M_size + b.M_size); // Créer une file de taille combinée

    // Ajouter les éléments de la première file
    for (int i = 0; i < a.M_count; ++i) {
        c.Enfiler(a.M_elem[(a.M_debut + i) % a.M_size]);
    }

    // Ajouter les éléments de la deuxième file
    for (int i = 0; i < b.M_count; ++i) {
        c.Enfiler(b.M_elem[(b.M_debut + i) % b.M_size]);
    }

    return c;
}

int main() {
    // Création et initialisation de la file a
    File a(10);
    a.init();
    std::cout << "File a : ";
    a.print();

    // Création d'une file b en copie de a
    File b(a);
    std::cout << "File b (copie de a) : ";
    b.print();

    // Concaténation des files a et b
    File c = a + b;
    std::cout << "File c (concaténation de a et b) : ";
    c.print();

    // Retrait d'un élément de la file
    c.Defiler();
    std::cout << "File c après Defiler : ";
    c.print();

    // Afficher l'élément en tête
    std::cout << "Élément en tête de la file c : " << c.Front() << std::endl;

    return 0;
}
