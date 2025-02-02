#include <iostream>
#include <cmath>
#include <ctime>

class Rectangle 
{
    int M_largeur;
    int M_hauteur;
    
public:
    Rectangle(Rectangle const &r) {
        M_largeur = r.M_largeur;
        M_hauteur = r.M_hauteur;
    }
    
    Rectangle(double largeur, double hauteur) {
        M_largeur = largeur; 
        M_hauteur = hauteur; 
    }
    
    double get_larg() const {
        return M_largeur;
    }
    
    double get_haut() const {
        return M_hauteur;
    }
    
    void set_hauteur(double x) {
        M_hauteur = x;
    }
    
    void set_largeur(double y) {
        M_largeur = y;
    }
    
    double const perimetre() const {
        return 2 * (get_larg() + get_haut());
    }
    
    double const aire() const {
        return M_largeur * M_hauteur;
    }
    
    // Surcharge de l'opérateur ==
    bool operator==(const Rectangle& b) const {
        return (M_largeur == b.M_largeur && M_hauteur == b.M_hauteur);
    }

     // Surcharge de l'opérateur ==
    bool operator!=(const Rectangle& b) const {
        return (M_largeur != b.M_largeur || M_hauteur != b.M_hauteur);
    }

};


// Surcharge de l'opérateur <<
std::ostream& operator<<(std::ostream &o, Rectangle const &r) {
    o << "Rectangle(" << r.get_larg() << ", " << r.get_haut() << ")";
    return o;
}

int main() {
    Rectangle a(1.2, 1.2);
    a.set_hauteur(3.0);
    std::cout << "Hauteur de a : " << a.get_haut() << std::endl;
    
    Rectangle b = Rectangle(a); // Appel au constructeur de copie
    std::cout << "Hauteur de b (copie de a) : " << b.get_haut() << std::endl;
    
    // Affichage de l'objet rectangle avec l'opérateur <<
    std::cout << "Rectangle a : " << a << std::endl;
    std::cout << "Rectangle b : " << b << std::endl;
    
   
    return 0;
}
