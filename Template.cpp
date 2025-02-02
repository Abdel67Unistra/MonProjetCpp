#include <iostream>
#include <cmath>
#include <stdexcept>

template<typename T, int d>
class anneauquad {
private:
    T a; // Coefficient pour 1
    T b; // Coefficient pour √d

public:
    // Constructeurs
    anneauquad(T a_init = 0, T b_init = 0) : a(a_init), b(b_init) {}

    // Addition
    anneauquad operator+(const anneauquad& other) const {
        if constexpr (d == other.d) { // Vérifie que les deux anneaux ont le même d
            return anneauquad(a + other.a, b + other.b);
        } else {
            throw std::invalid_argument("Opération entre anneaux de valeurs différentes de d invalide.");
        }
    }

    // Multiplication
    anneauquad operator*(const anneauquad& other) const {
        if constexpr (d == other.d) {
            T new_a = a * other.a + b * other.b * d;
            T new_b = a * other.b + b * other.a;
            return anneauquad(new_a, new_b);
        } else {
            throw std::invalid_argument("Opération entre anneaux de valeurs différentes de d invalide.");
        }
    }

    // Opérations externes avec double
    friend double operator*(double y, const anneauquad& x) {
        return y * (static_cast<double>(x.a) + std::sqrt(d) * static_cast<double>(x.b));
    }

    friend double operator+(double y, const anneauquad& x) {
        return y + (static_cast<double>(x.a) + std::sqrt(d) * static_cast<double>(x.b));
    }

    // Affichage
    void print() const {
        std::cout << a << " + " << b << "√" << d << std::endl;
    }
};

class rationnel {
private:
    int n; // Numérateur
    int d; // Dénominateur

public:
    // Constructeur
    rationnel(int num = 0, int den = 1) : n(num), d(den) {
        if (d == 0) throw std::invalid_argument("Dénominateur ne peut pas être nul.");
    }

    // Cast vers int
    operator int() const {
        return n / d;
    }

    // Cast vers double
    operator double() const {
        return static_cast<double>(n) / static_cast<double>(d);
    }

    // Affichage
    void print() const {
        std::cout << n << "/" << d;
    }
};



int main() {
    // Exemple avec anneauquad<int, 2>
    anneauquad<int, 2> an(1, 2); // 1 + 2√2
    anneauquad<int, 2> bn(1, 3); // 1 + 3√2

    // Addition et multiplication dans le même anneau
    auto cn = an + bn;
    cn.print(); // Affiche 2 + 5√2

    auto dn = an * bn;
    dn.print(); // Affiche -5 + 8√2

    // Exemple avec anneauquad<rationnel, 3>
    anneauquad<rationnel, 3> x(rationnel(1, 1), rationnel(1, 2)); // 1 + 1/2√3
    anneauquad<rationnel, 3> y(rationnel(2, 1), rationnel(1, 3)); // 2 + 1/3√3

    auto z = x + y;
    z.print(); // Affiche 3 + 5/6√3

    double p = 1.2;
    p = (p + z) + p * x;
    std::cout << "Valeur de p : " << p << std::endl;

    return 0;
}
