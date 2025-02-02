#include<iostream>


class Vecteur
{

    int M_size ;
    double *M_vect ;

    public :
        Vecteur(int n)
        {
            M_size = n ; 
            M_vect = new double[M_size] ;
            for(int i(0) ; i<M_size;i++)
            {
                M_vect[i] = 0 ;
            }
        }
        
        ~Vecteur()
        {
            delete M_vect[] ; 
        }

        double operator[](int i)
        {
            return M_vect[i] ;
        }


};



int main()
{
    Vecteur a(5) ;
    std::cout<<a[2]<<std::endl;


    return 1 ;
}