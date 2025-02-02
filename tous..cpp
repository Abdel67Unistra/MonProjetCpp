#include <iostream>
#include <stdexcept> // Pour std::invalid_argument
#include <numeric>   // Pour std::gcd

class Fraction {
    int numerateur, denominateur;

public:
    // Constructeur par défaut
    Fraction(int num = 0, int den = 1) {
        if (den == 0) {
            throw std::invalid_argument("Le dénominateur ne peut pas être 0.");
        }
        numerateur = num;
        denominateur = den;
        simplifier();
    }

    // Méthode pour simplifier la fraction
    void simplifier() {
        int pgcd = std::gcd(numerateur, denominateur);
        numerateur /= pgcd;
        denominateur /= pgcd;
    }

    // Surcharge de l'opérateur +
    friend Fraction operator+(const Fraction& a, const Fraction& b) {
        return Fraction(a.numerateur * b.denominateur + b.numerateur * a.denominateur,
                        a.denominateur * b.denominateur);
    }

    // Surcharge de l'opérateur <<
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        os << f.numerateur << "/" << f.denominateur;
        return os;
    }
};



//-----------------------------------------------------------------------------------------------------------------------------------------

int main() {
    Fraction a(3, 4), b(2, 5);
    Fraction c = a + b;
    std::cout << "Fraction a : " << a << std::endl;
    std::cout << "Fraction b : " << b << std::endl;
    std::cout << "Somme : " << c << std::endl;

    return 0;
}


#include <iostream>
#include <cmath>

class Vecteur3D {
    double x, y, z;

public:
    Vecteur3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    // Surcharge de l'opérateur +
    Vecteur3D operator+(const Vecteur3D& v) const {
        return Vecteur3D(x + v.x, y + v.y, z + v.z);
    }

    // Surcharge de l'opérateur -
    Vecteur3D operator-(const Vecteur3D& v) const {
        return Vecteur3D(x - v.x, y - v.y, z - v.z);
    }

    // Produit scalaire
    double operator*(const Vecteur3D& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Affichage avec <<
    friend std::ostream& operator<<(std::ostream& os, const Vecteur3D& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
};

int main() {
    Vecteur3D v1(1, 2, 3), v2(4, 5, 6);
    std::cout << "Vecteur v1 : " << v1 << std::endl;
    std::cout << "Vecteur v2 : " << v2 << std::endl;
    std::cout << "Addition : " << v1 + v2 << std::endl;
    std::cout << "Produit scalaire : " << v1 * v2 << std::endl;

    return 0;
}



//-----------------------------------------------------------------------------------------------------------------------------------------


#include <iostream>
#include <cmath>

class Complexe {
    double re, im;

public:
    Complexe(double re = 0, double im = 0) : re(re), im(im) {}

    // Module
    double module() const {
        return std::sqrt(re * re + im * im);
    }

    // Surcharge de l'opérateur >>
    friend std::istream& operator>>(std::istream& is, Complexe& c) {
        std::cout << "Entrez la partie réelle : ";
        is >> c.re;
        std::cout << "Entrez la partie imaginaire : ";
        is >> c.im;
        return is;
    }

    // Surcharge de l'opérateur <<
    friend std::ostream& operator<<(std::ostream& os, const Complexe& c) {
        os << c.re << (c.im >= 0 ? " + " : " - ") << std::abs(c.im) << "i";
        return os;
    }
};

int main() {
    Complexe c1, c2(3, 4);
    std::cout << "Entrez un nombre complexe c1 : " << std::endl;
    std::cin >> c1;
    std::cout << "c1 : " << c1 << std::endl;
    std::cout << "c2 : " << c2 << std::endl;
    std::cout << "Module de c2 : " << c2.module() << std::endl;

    return 0;
}




//-----------------------------------------------------------------------------------------------------------------------------------------


#include <iostream>

class Matrice {
    double mat[2][2];

public:
    Matrice(double a = 0, double b = 0, double c = 0, double d = 0) {
        mat[0][0] = a;
        mat[0][1] = b;
        mat[1][0] = c;
        mat[1][1] = d;
    }

    // Multiplication de deux matrices
    Matrice operator*(const Matrice& m) const {
        return Matrice(
            mat[0][0] * m.mat[0][0] + mat[0][1] * m.mat[1][0],
            mat[0][0] * m.mat[0][1] + mat[0][1] * m.mat[1][1],
            mat[1][0] * m.mat[0][0] + mat[1][1] * m.mat[1][0],
            mat[1][0] * m.mat[0][1] + mat[1][1] * m.mat[1][1]);
    }

    // Déterminant
    double determinant() const {
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    }

    // Affichage
    friend std::ostream& operator<<(std::ostream& os, const Matrice& m) {
        os << "| " << m.mat[0][0] << " " << m.mat[0][1] << " |\n";
        os << "| " << m.mat[1][0] << " " << m.mat[1][1] << " |";
        return os;
    }
};

int main() {
    Matrice m1(1, 2, 3, 4), m2(2, 0, 1, 2);
    Matrice m3 = m1 * m2;
    std::cout << "Matrice m1 :\n" << m1 << std::endl;
    std::cout << "Matrice m2 :\n" << m2 << std::endl;
    std::cout << "Produit m1 * m2 :\n" << m3 << std::endl;
    std::cout << "Déterminant de m1 : " << m1.determinant() << std::endl;

    return 0;
}


//-----------------------------------------------------------------------------------------------------------------------------------------


#include <iostream>
#include <vector>

class Polynome {
    std::vector<double> coeffs;

public:
    Polynome(std::initializer_list<double> init) : coeffs(init) {}

    // Addition de deux polynômes
    Polynome operator+(const Polynome& p) const {
        size_t max_deg = std::max(coeffs.size(), p.coeffs.size());
        std::vector<double> result(max_deg, 0);

        for (size_t i = 0; i < coeffs.size(); ++i)
            result[i] += coeffs[i];
        for (size_t i = 0; i < p.coeffs.size(); ++i)
            result[i] += p.coeffs[i];

        return Polynome(result);
    }

    // Affichage
    friend std::ostream& operator<<(std::ostream& os, const Polynome& p) {
        for (size_t i = 0; i < p.coeffs.size(); ++i) {
            if (i > 0) os << " + ";
            os << p.coeffs[i] << "x^" << i;
        }
        return os;
    }

private:
    Polynome(const std::vector<double>& coeffs) : coeffs(coeffs) {}
};

int main() {
    Polynome p1{1, 2, 3}; // 1 + 2x + 3x^2
    Polynome p2{0, 4, 5}; // 4x + 5x^2
    Polynome p3 = p1 + p2;

    std::cout << "Polynome p1 : " << p1 << std::endl;
    std::cout << "Polynome p2 : " << p2 << std::
