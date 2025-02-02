#include<iostream>
#include <stdexcept> // Pour std::out_of_range et exceptions
#include<cmath>
class Complexe
{
    double M_Re ,M_Im ;
    public :
        Complexe()
        {
            M_Re = 0 ;
            M_Im = 0 ;

        }
        Complexe(double reel,double imaginaire)
        {
            M_Re = reel ;
            M_Im = imaginaire ;
        }
        Complexe(const Complexe &b)
        {
            M_Re = b.M_Re ;
            M_Im = b.M_Im ;
        }

        bool operator ==(const Complexe &b)const
        {
            return (M_Re == b.M_Re) && (M_Im == b.M_Im) ; 
        }

        bool operator !=(const Complexe &b)const
        {
            return (M_Re != b.M_Re) || (M_Im != b.M_Im) ; 
        }
        
        void operator +=(const Complexe &b)
        {
            M_Re += b.M_Re ;
            M_Im += b.M_Im ;
        }

        void operator -=(const Complexe &b)
        {
            M_Re -= b.M_Re ;
            M_Im -= b.M_Im ;
        }

        void operator *=(const Complexe &b)
        {
            M_Re = M_Re * b.M_Re - M_Im * b.M_Im ;
            M_Im = M_Re * b.M_Im + M_Im * b.M_Re ;
        }

        void operator /=(const Complexe &b)
        {
            double denom = std::pow(b.M_Re, 2) + std::pow(b.M_Im, 2);
            if (denom == 0)
            {
                throw std::invalid_argument("Division par zéro");
            }

            M_Re = (M_Re * b.M_Re + M_Im * b.M_Im)/(std::pow(b.M_Re,2)  + std::pow(b.M_Im,2)) ;
            M_Im = (M_Im * b.M_Re - M_Re * b.M_Im)/(std::pow(b.M_Re,2)  + std::pow(b.M_Im,2)) ;
        }

        friend Complexe operator +(const Complexe &a ,const Complexe &b);
        friend Complexe operator -(const Complexe &a ,const Complexe &b);
        friend Complexe operator *(const Complexe &a ,const Complexe &b);
        friend Complexe operator /(const Complexe &a ,const Complexe &b);
        // Surcharge de l'opérateur <<
        friend std::ostream& operator<<(std::ostream& os, const Complexe& c);



};

// Définition de l'opérateur <<
std::ostream& operator<<(std::ostream& os, const Complexe& c)
{
    os << c.M_Re;
    if (c.M_Im >= 0) {
        os << " + " << c.M_Im << "i";  // Affichage positif de la partie imaginaire
    } else {
        os << " - " << -c.M_Im << "i";  // Affichage négatif de la partie imaginaire
    }
    return os;  // Retour du flux pour permettre l'enchaînement d'opérations
}


Complexe operator +(const Complexe &a ,const Complexe &b)
{
    Complexe c(a) ;
    c+=b ;
    return c ;

}

Complexe operator -(const Complexe &a ,const Complexe &b)
{
    Complexe c(a) ;
    c-=b ;
    return c ;

}

Complexe operator *(const Complexe &a ,const Complexe &b)
{
    Complexe c(a) ;
    c*=b ;
    return c ;

}
Complexe operator /(const Complexe &a ,const Complexe &b)
{
    Complexe c(a) ;
    c/=b ;
    return c ;

}



