#include<iostream>
#include <stdexcept> // Pour std::out_of_range et exceptions
//  throw std::invalid_argument("Les vecteurs n'ont pas la même taille.");

class Complexe
{
  double M_Re , M_Im;
  public :
    Complexe()
    {
        M_Re =0. ;
        M_Im = 0. ;
    }
    Complexe(double re ,double im)
    {
        M_Re =re ;
        M_Im = im ;
    }
        Complexe(Complexe const &a)
    {
        M_Re =a.M_Re ;
        M_Im = a.M_Im ;
    }


    void print()
    {
        if(M_Im >=0)
        {
        std::cout<<M_Re<<"+"<<M_Im<<"i"<<std::endl;
        }
        else
        {
            std::cout<<M_Re<<M_Im<<"i"<<std::endl;
        }

        
    }
    bool operator==(Complexe const &b)const
    {
        return (M_Re == b.M_Re) && (M_Im == b.M_Im);
    }
        bool operator!=(Complexe const &b)const
    {
        return (M_Re != b.M_Re) || (M_Im != b.M_Im);
    }

    void operator +=(Complexe const &b)
    {
        M_Re += b.M_Re ;
        M_Im += b.M_Im ;
    }
    void operator -=(Complexe const &b)
    {
        M_Re -= b.M_Re ;
        M_Im -= b.M_Im ;
    }

    void operator *=(Complexe const&b)
    {
        double reel = M_Re * b.M_Re - M_Im * b.M_Im;
        M_Im = M_Re * b.M_Im + M_Im * b.M_Re; // Correction ici pour l'imaginaire
        M_Re = reel;
    }

 
    friend Complexe operator+(const Complexe &a , const Complexe &b) ;
    friend Complexe operator-(const Complexe &a , const Complexe &b) ;
    friend Complexe operator*(const Complexe &a , const Complexe &b) ;



};


Complexe operator+(const Complexe &a , const Complexe &b) 
{
    Complexe c(a);
    c+=b ;
    return c;
}

Complexe operator-(const Complexe &a , const Complexe &b) 
{
    Complexe c(a);
    c-=b ;
    return c;
}

Complexe operator*(const Complexe &a , const Complexe &b) 
{
    Complexe c(a);
    c*=b ;
    return c;
}




int main()
{
    Complexe a(8,-9);
    //a.print();
    Complexe b(1,1) ;
    //a*=a;
    //a.print() ;
    Complexe d(b*b);
    d.print();
    return 1 ;
}