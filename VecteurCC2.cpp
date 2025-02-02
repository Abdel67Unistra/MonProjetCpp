#include<iostream>
#include <stdexcept> // Pour std::out_of_range et exceptions

class Vecteur
{
    int M_size ;
    double *M_vect ;

    public :

        Vecteur(int size)
        {
            M_size = size ;
            M_vect = new double[M_size] ;

            for(int i(0) ;i<M_size ;i++)
            {
                M_vect[i] = 0 ;
            }
        }

        Vecteur(const Vecteur &b)
        {
            M_size = b.M_size ;
            M_vect = new double[M_size] ;
            for(int i(0) ;i<M_size ;i++)
            {
                M_vect[i] = b.M_vect[i] ;
            }

        }
        

        double operator[](int i)const
        {
                if (i < 0 || i >= M_size)
                {
                    throw std::out_of_range("Indice hors limites");
                }


            return M_vect[i] ;
        }

        double & operator()(int i)
        {
            if (i < 0 || i >= M_size) 
                {
                    throw std::out_of_range("Indice hors limites");
                }

            return M_vect[i] ;
        }

         ~Vecteur()
         {
            delete [] M_vect ;
         }

        void operator =(const Vecteur &b)
        {
            if(M_size != b.M_size)
            {
                throw std::invalid_argument("La taille n'est pas la même !");
            }

            for(int i(0) ;i<M_size;i++)
            {
                M_vect[i] = b.M_vect[i] ;
            }
        }


        void set_constant( double d )
            {
                for(int i(0) ;i<M_size ;i++)
                    {
                        M_vect[i] = d ;
                    }
            }

        void set_zero()
        {       
        for(int i(0) ;i<M_size ;i++)
            {
                M_vect[i] = 0 ;
            }
       
        }

        double somme() const
        {
            double som = 0 ;
            for(int i(0) ;i<M_size ;i++)
            {
                som += M_vect[i] ;
            }
            return som ;
        }

        void operator +=(const Vecteur &b)
        {
    
            if (M_size != b.M_size) 
            {
                throw std::invalid_argument("Les tailles des vecteurs doivent être égales !");
            }

            for(int i(0) ;i<M_size ;i++)
            {
                M_vect[i] += b.M_vect[i] ;
            }
        }

        void operator -=(const Vecteur &b)
        {
            if (M_size != b.M_size) 
            {
                throw std::invalid_argument("Les tailles des vecteurs doivent être égales !");
            }

            for(int i(0) ;i<M_size ;i++)
            {
                M_vect[i] -= b.M_vect[i] ;
            }
        }
    void print() const
    {
        std::cout << "[";
        for (int i = 0; i < M_size; i++) 
            {
                std::cout << M_vect[i];
            if (i < M_size - 1)
            {
                std::cout << ", ";
            }
            }
        std::cout << "]" << std::endl;
}

        friend Vecteur operator +(const Vecteur &a , const Vecteur &b) ;
        friend Vecteur operator -(const Vecteur &a , const Vecteur &b) ;
        friend double inner_product( Vecteur const &v1, Vecteur const &v2 ) ;

};

Vecteur operator +(const Vecteur &a , const Vecteur &b)
{
    Vecteur c(a) ;
    c+=b ;
    return c ;

}
Vecteur operator -(const Vecteur &a , const Vecteur &b)
{
    Vecteur c(a) ;
    c-=b ;
    return c ;

}
double inner_product( Vecteur const &v1, Vecteur const &v2 )
{
    double ps(0) ;
    for(int i(0) ;i<v1.M_size ;i++)
    {
        ps += v1.M_vect[i] * v2.M_vect[i] ;
    } 

    return ps ;
}


int main()
{

    Vecteur a(7) ;
    a.set_constant(7);
    Vecteur c = (a-a) ;
    c.print();


    return 0 ;
}