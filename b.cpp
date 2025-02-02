#include<iostream>

class Vecteur
{
    int M_size ;
    double *M_vect ;
    public :
        Vecteur(int size)
        {
            M_size = size ;
            M_vect = new double[M_size] ;
            this->set_zero();
        }

        Vecteur(const Vecteur &b)
        {
            M_size = b.M_size ;
            M_vect = new double[M_size] ;
            for(int i(0) ; i<M_size ;i++ )
            {
                M_vect[i] = b.M_vect[i] ;
            }

        }


        void set_zero()
        {
            for(int i(0) ;i<M_size ;i++)
            {
                M_vect[i] = 0 ;
            }
        }
    
        void init()
        {
                    {
            for(int i(0) ;i<M_size ;i++)
            {
                M_vect[i] = i ;
            }
        }
    
        }

        double operator [](int i)
        {
            return M_vect[i] ;
        }

        double &operator ()(int i)
        {
            return M_vect[i];
        }
        void print()
        {
            for(int i(0) ;i<M_size ;i++)
            {
               std::cout<< M_vect[i]<<std::endl ;
            }
      }

       ~Vecteur() 
        {
            delete[] M_vect;
        }


        void operator =(Vecteur b)
        {
            M_size = b.M_size ;
            M_vect = new double[M_size] ;
            for(int i(0) ;i<M_size ;i++)
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

        double somme() const
        {
            double som = 0. ;
            for(int i(0);i<M_size ;i++)
            {
                som += M_vect[i] ;
            }
            return som;
        }


        void operator +=(const Vecteur &b)
        {
            for(int i(0) ;i<M_size;i++)
            {
                M_vect[i] += b.M_vect[i] ;
            }
        }

        void operator -=(const Vecteur &b)
        {
            for(int i(0) ;i<M_size;i++)
            {
                M_vect[i] -= b.M_vect[i] ;
            }
        }


        friend Vecteur operator +(const Vecteur &a ,const Vecteur &b ) ;
        friend Vecteur operator -(const Vecteur &a ,const Vecteur &b ) ;
        friend double inner_product( Vecteur const &v1, Vecteur const &v2 ) ;
};

Vecteur operator +(const Vecteur &a ,const Vecteur &b ) 
{
    Vecteur c(a.M_size) ;
    c+=a ;
    c+=b ;
    return c ;
}



Vecteur operator -(const Vecteur &a ,const Vecteur &b ) 
{
    Vecteur c(a.M_size) ;
    c+=a ;
    c-=b ;
    return c ;
}


double inner_product( Vecteur const &v1, Vecteur const &v2 )
    {
        double ps =0 ;
        for(int i(0); i<v1.M_size ;i++)
        {
            ps += v1.M_vect[i] * v2.M_vect[i] ;

        }
        return ps ;

    }

int main()
{
    Vecteur a(4) ;
    //a.init() ;
    a(2) =9 ;


    Vecteur b(4) ;
    b= a ;

    Vecteur c = (a-b) ;
    double ps = inner_product(a,a) ;
    std::cout<<ps<<std::endl;



    return 0 ;
}