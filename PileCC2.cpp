#include<iostream>
#include <stdexcept> // Pour std::out_of_range et exceptions

class Pile
{
    int M_size;        // Taille maximale de la pile
    int* M_elem;       // Tableau dynamique pour stocker les éléments
    int M_sommet;      // Index du sommet de la pile

public:
    // Constructeur
    Pile(int size)
    {
        if (size <= 0) {
            throw std::invalid_argument("La taille doit être positive.");
        }
        M_size = size;
        M_elem = new int[M_size];  // Allocation dynamique
        M_sommet = -1;             // La pile est vide au début
    }

    // Constructeur par copie
    Pile(const Pile& b)
    {
        M_size = b.M_size;
        M_elem = new int[M_size];  // Allocation mémoire
        M_sommet = b.M_sommet;    // Copier la valeur du sommet
        for (int i = 0; i <= M_sommet; ++i) {
            M_elem[i] = b.M_elem[i];  // Copier les éléments
        }
    }

    // Initialisation de la pile avec des valeurs
    void init()
    {
        for (int i = 0; i < M_size - 10; ++i) {
            M_elem[i] = i + 1;
            M_sommet++;
        }
    }

    // Affichage des éléments de la pile
    void print()
    {
        for (int i = 0; i <= M_sommet; ++i) {
            std::cout << M_elem[i] << std::endl;
        }
    }

    // Empiler un élément (Push)
    void Push(int val)
    {
        if (M_sommet >= M_size - 1) {
            throw std::out_of_range("La pile est pleine.");
        }
        M_elem[++M_sommet] = val;
    }

    // Dépiler un élément (Pop)
    void pop()
    {
        if (M_sommet == -1) {
            throw std::out_of_range("La pile est vide.");
        }
        M_elem[M_sommet--] = 0;  // Décrémenter le sommet
    }

    // Consulter l'élément au sommet de la pile
    int Top()
    {
        if (M_sommet == -1) {
            throw std::out_of_range("La pile est vide.");
        }
        return M_elem[M_sommet];
    }

    // Destructeur pour libérer la mémoire
    ~Pile() {
        delete[] M_elem;
    }

    // Surcharge de l'opérateur + pour concaténer deux piles
    friend Pile operator+(const Pile& a, const Pile& b);
};

// Surcharge de l'opérateur + pour concaténer deux piles
Pile operator+(const Pile& a, const Pile& b)
{
    Pile c(a.M_size + b.M_size);  // Créer une pile de taille combinée
    int j = 0;
    
    // Ajouter les éléments de la première pile
    for (int i = 0; i <= a.M_sommet; ++i) {
        c.Push(a.M_elem[i]);
    }

    // Ajouter les éléments de la deuxième pile
    for (int i = 0; i <= b.M_sommet; ++i) {
        c.Push(b.M_elem[i]);
    }

    return c;
}

int main()
{
        // Création et initialisation de la pile a
        Pile a(10);
        a.init();
        std::cout << "Pile a : " << std::endl;
        a.print();

        // Création d'une pile b en copie de a
        Pile b(a);
        std::cout << "Pile b (copie de a) :"<<std::endl;

        return 0 ;
}