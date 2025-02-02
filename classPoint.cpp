#include <iostream>

class Point {
private:
    int x, y;

public:
    // Constructeur
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    // Déclaration de la surcharge de l'opérateur << comme friend
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

// Implémentation de la surcharge de l'opérateur << en dehors de la classe
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os; // Retourne le flux pour permettre le chaînage
}

int main() {
    Point p1(5, 10);
    Point p2(3, 7);

    std::cout << "Point 1 : " << p1 << std::endl;
    std::cout << "Point 2 : " << p2 << std::endl;

    return 0;
}
