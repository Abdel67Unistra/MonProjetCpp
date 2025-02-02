#include<iostream>
#include <stdexcept> // Pour std::out_of_range et exceptions

class Pile
{

 int M_size ;
 int * M_elem ;
 int M_sommet ;

 public :
    Pile(int size)
    {
        M_size = size ; 
        M_elem = new int[M_size +50] ;
        M_sommet = -1 ;
    }

    Pile(const Pile &b)
    {
        M_size = b.M_size ; 
        M_elem = new int[M_size +50] ;
        M_sommet = -1 ;
        for(int i(0) ;i<M_size ;i++)
        {
            M_elem[i] = b.M_elem[i] ;
            M_sommet ++ ;
        }

    }

    void init()
    {
        for(int i(0) ;i<M_size-10;i++)
        {

            M_elem[i] = i+1 ;
            M_sommet ++ ;
        }
    }

    void print()
    {
        for(int i(0) ;i<M_size ;i++)
        {
            std::cout<<M_elem[i]<<std::endl;
        }
    }

    void Push(int val)
    {
        if(M_sommet >= M_size)
        {
            throw std::out_of_range("Index hors limites.");
        }
        M_sommet++ ;
        M_elem[M_sommet] = val ; 

    }

    void pop()
    {
        if(M_sommet==-1)
        {
            throw std::out_of_range("La pile est vide .");
        }
        M_elem[M_sommet] = 0 ;
        M_sommet-- ;
    }

    int Top()
    {
        return M_elem[M_sommet] ;
    }

    ~Pile() 
    {
        delete[] M_elem; // Libération de la mémoire
    }


    friend Pile operator +(const Pile &a ,const Pile &b) ;



} ;
Pile operator +(const Pile &a ,const Pile &b)
    {

        Pile c(a.M_size + b.M_size) ;
        int j(0);
        for(int i(0) ;i< c.M_size ;i++)
        {
            if(i<a.M_sommet)
            {
                c.M_elem[i] = a.M_elem[i] ;
            }
            else if (j<b.M_sommet)
            {
                c.M_elem[i] = b.M_elem[j] ;
                j++ ;
            }
        }
    
        return c ;
    }





int main()
{

    Pile a(100) ;
    a.init() ;
    //a.print() ;
    Pile c(a+a) ;
    c.print() ;
    return 0 ;
}