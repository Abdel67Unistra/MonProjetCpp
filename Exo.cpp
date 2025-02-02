#include<iostream>
#include <stdexcept> // Pour std::out_of_range et exceptions

class Vecteur
{
    int M_size ;
    double* M_vect;

    public : 
    Vecteur(int taille) 
    {
        M_size = taille ;
        M_vect = new double[M_size] ;
    }

    ~Vecteur() 
    {
        delete[] M_vect;
    }

    int get_Size()
    {
        return M_size ;
    }

     double operator[](int index) const
    {
        if(index <0 || index >= M_size)
        {
            throw std::out_of_range("Index hors limites.");
        }
        return M_vect[index]
        ;
    }
    double operator()(int index) const
    {
        return M_vect[index];
    }

    void print()
    {
        for(int i(0); i<M_size ;i++)
        {
            std::cout<<M_vect[i]<<std::endl;
        }
    }
    void inis()
    {
        for (int i = 0; i < M_size; i++)
        {
            M_vect[i] = i ;
        }
        
    }

   //Vecteur operator =(Vecteur v)
   Vecteur& operator=(const Vecteur& v)
    {
        if(this == &v)
        {
            return *this ;
        }

        if(v.M_size == M_size)
        {
        for(int i(0) ; i<M_size ;i++)
        {
        M_vect[i] = v.M_vect[i]  ;
        }
        
        }
        else
        {
            throw std::out_of_range("Pas le même index !");
        }
        return *this  ; 

    }

    void set_constant( double d )
    {
        for(int i(0) ; i< M_size ;i++)
        {
            M_vect[i] = d ;
        }
    }

void set_zero()
{
    for(int i(0) ; i<M_size;i++)
    {
        M_vect[i] = 0 ;
    }
}
double somme() const
{
    double somme = 0 ;
    for(int i(0) ;i<M_size;i++)
    {
        somme += M_vect[i] ;
    }
    return somme ;
}

Vecteur operator+(Vecteur b)
{
    if(M_size != b.M_size)
    {
        throw std::out_of_range("Pas le même index !");
    }
    else
    {
        for(int i(0) ; i<M_size;i++)
        {
            M_vect[i] += b.M_vect[i] ;
        }
    }
    return *this ;
}

Vecteur operator-(Vecteur b)
{
    if(M_size != b.M_size)
    {
        throw std::out_of_range("Pas le même index !");
    }
    else
    {
        for(int i(0) ; i<M_size;i++)
        {
            M_vect[i] -= b.M_vect[i] ;
        }
    }
    return *this ;
}

friend double inner_product( Vecteur const &v1, Vecteur const &v2 );

};

double inner_product( Vecteur const &v1, Vecteur const &v2 )
{
    double ps = 0 ;
    for(int i(0) ; i<v1.M_size;i++)
    {
        ps += v1.M_vect[i] * v2.M_vect[i];
    }
    return ps ;
}




int main()
{

Vecteur a = Vecteur(5);
a.inis() ;
//a.print();
//std::cout<<a[0]<<std::endl; 
Vecteur b = a ;
//b.print();
b.set_constant(0) ;
//b.print();
std::cout<<inner_product(a,b)<<std::endl;
    return 0 ;
}