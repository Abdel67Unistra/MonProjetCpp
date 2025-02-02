#include <iostream>

// Déclaration d'une fonction simple
int addition(int a, int b) {
    return a + b;
}

// Déclaration d'une fonction qui utilise un pointeur de fonction
void utiliser_pointeur(int (*pointeur)(int, int), int x, int y) {
    std::cout << "Le résultat de l'addition est : " << pointeur(x, y) << std::endl;
}

int main() {
    // Déclaration et initialisation du pointeur de fonction
    int (*ptr_add)(int, int) = addition;

    // Utilisation du pointeur de fonction
    utiliser_pointeur(ptr_add, 5, 3);  // Affiche "Le résultat de l'addition est : 8"

    return 0;
}
