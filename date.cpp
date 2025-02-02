#include <iostream>
#include <cmath>
#include <ctime>

class Complexe 
{
     double M_RE ,M_Im  ;
     public :

   Complexe()
   {
      M_RE = 0 ;
      M_Im = 0 ;
   }

   double get_RE()
   {
      return M_RE;
   }
   double get_Im()
   {
      return M_Im ;
   }
     Complexe(double re , double im )
     {
        M_RE = re ;
        M_Im = im ;
     }

     void print() const
     {
      
      if(M_Im < 0){
        std::cout<<M_RE<<M_Im<< "i"<<std::endl;
      }
      else
      {
         std::cout<<M_RE<<"+"<<M_Im<< "i"<<std::endl;
      }
      
     }

   bool operator==(const Complexe& autre) const {
        return (M_RE == autre.M_RE) && (M_Im == autre.M_Im);
    }
   bool operator!=(const Complexe& autre) const
   {
      return (M_RE != autre.M_RE) || (M_Im != autre.M_Im);
   }
    Complexe operator+=(const Complexe& autre)
    {
      M_RE += autre.M_RE ;
      M_Im += autre.M_Im ;
      return *this ;
    }

    Complexe operator-=(const Complexe& autre)
    {
      M_RE -= autre.M_RE ;
      M_Im -= autre.M_Im ;
      return *this ;
    }

       Complexe operator*=(const Complexe& autre)
    {
      M_RE = M_RE * autre.M_RE - M_Im * autre.M_Im;
      M_Im = M_RE * autre.M_Im + M_Im * autre.M_RE ;
      return *this ;
    }
      Complexe operator/=(const Complexe& autre)
    {
      M_RE = (M_RE * autre.M_RE + M_Im * autre.M_Im)/(pow(autre.M_RE,2) + pow(autre.M_Im,2)) ;
      M_Im = (M_Im * autre.M_RE - M_RE * autre.M_Im)/(pow(autre.M_RE,2) + pow(autre.M_Im,2)) ;
      return *this ;
    }

    // Surcharge de l'opérateur << comme une fonction amie
    friend std::ostream& operator<<(std::ostream& os, const Complexe& c);


   
   // Méthodes amies pour surcharger les opérateurs binaires
    friend Complexe operator+(Complexe un, const Complexe &deux);
    friend Complexe operator-(Complexe un, const Complexe &deux);
    friend Complexe operator*(Complexe un, const Complexe &deux);
    friend Complexe operator/(Complexe un, const Complexe &deux);






};


Complexe operator+(Complexe un , const Complexe & deux)
{
   un += deux ;
   return un ;

}

Complexe operator-(Complexe un , const Complexe & deux)
{
   un -= deux ;
   return un ;

}
Complexe operator*(Complexe un , const Complexe & deux)
{
   un *= deux ;
   return un ;

}
Complexe operator/(Complexe un , const Complexe & deux)
{
   un /= deux ;
   return un ;

}





std::ostream& operator<<(std::ostream& os, const Complexe& c) {
    os << c.M_RE;
    if (c.M_Im >= 0) {
        os << " + " << c.M_Im << "i";
    } else {
        os << " - " << abs(c.M_Im) << "i";
    }
    return os; // Retourner le flux pour permettre l'enchaînement
}



int main()
{
   Complexe D(1, 2);
   Complexe C(2, 3);

   /*D.print();
   C.print();


   std::cout << (D == C ? "true" : "false") << std::endl;
   std::cout << (D != C ? "true" : "false") << std::endl;
   */
  Complexe H = D/=C ;

   std::cout<<D/D ;



  return 0 ;
}



