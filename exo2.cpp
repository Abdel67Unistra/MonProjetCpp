#include<iostream>
using namespace std ;

class Rationnel 
{
    int num ;
    int den ;
    public :
        Rationnel(int n,int d)
        {
            num = n ;
            den = d ;
        }
        void print()
        {
            cout<<num<<"/"<<den<<endl ;
        }

        Rationnel operator =(Rationnel b)
        {
            this ->num = b.num ;
            den = b.den ;
            return *this ;
        }

        Rationnel operator*(Rationnel b) ;

        friend Rationnel operator /(Rationnel a,Rationnel b) ;
        operator int() ;

};

Rationnel Rationnel::operator*(Rationnel b)
{
    Rationnel c(this->num * b.num,this-> den * b.den) ;
    return c ;

}
Rationnel operator /(Rationnel a ,Rationnel b)
{
    Rationnel c(a.num * b.den , a.den * b.num) ;
    return c ;
}

Rationnel::operator int()
{
    return num/den ; 

}



int main()
{
    Rationnel a(1,2);
    Rationnel b(5,5); 
    Rationnel c = (a/a);
    c.print();
    std::cout<<int(c)<<std::endl;
    return 0 ;
}