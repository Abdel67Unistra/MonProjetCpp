#include<iostream>
#include<cmath>
using namespace std ;
#include <stdexcept> // Pour std::out_of_range et exceptions
#include <string>

class Polynome 
{
    double *coeff ;
    int deg ;

    public : 
        Polynome(int n)
        {
            if(n >= 0)
            {
                deg = n ;
                coeff = new double[n+1] ;
            }
        }

        Polynome()
        {
            deg = 0 ;
            coeff = NULL ;
        }

        Polynome(const Polynome &b)
        {
            if(b.deg>=0)
            {
                deg = b.deg ;
                coeff = new double[deg + 1] ;
                for(int i(0) ; i<= deg ;i++)
                {
                    coeff[i] = b.coeff[i] ;
                }
            }
        }

        void Initialisation()
        {
            for(int i(0) ;i<=deg;i++)
            {
                coeff[i] = i ;
            }
        }
        void print()
        {
            for(int i(0) ;i<=deg;i++)
            {
                cout<<coeff[i]<<endl;
            }
        }
        double get(int n)
        {
            if(n> deg || deg < 0)
            {
                throw std::invalid_argument("Idince qui n'entre pas dans le tableau");
            }
            return coeff[n] ;
        }

        void set(double a , int n)
        {
            if(n> deg || deg < 0)
            {
                throw std::invalid_argument("Idince qui n'entre pas dans le tableau");
            }
            coeff[n] = a ; ;
        }

         ~Polynome()
         {
            if(coeff != NULL)
            {
                cout<<"Destructeur"<<endl;
                delete[] coeff ;

            }
            
         }

        double evaluer(double x)
        {

            double result = 0.0 ;
            for(int i(0) ;i<=deg ;i++)
            {
                result += coeff[i] * pow(x,i);
            }
            return result ;

        }

        void afficher()
        {
            string affichage = " ";

            for(int i(0) ;i<= deg ; i++)
            {
                if(i !=0 && coeff[i]>=0 && coeff[i]!=0)
                {
                    affichage += " + " + to_string(coeff[i]) + " * x^" + to_string(i);
                }
                if(i !=0 && coeff[i]<=0 && coeff[i] != 0)
                {
                    affichage += " - " + to_string(-coeff[i]) + " * x^" + to_string(i);
                }
                if(i==0 && coeff[i] != 0)
                {
                    affichage += to_string(coeff[i])+" * x^" + to_string(i);
                }
            }
            if (affichage.empty())  // Si aucun terme n'a été ajouté (polynôme nul)
                {
                    affichage = "0";
                }

            cout<<affichage<<endl;
        }     

        Polynome Derivee()
        {
            if(deg < 1)
            {
                throw std::invalid_argument("Idince qui n'entre pas dans le tableau");
            }
            Polynome Der = Polynome(deg-1);
            for(int i(1);i<=deg;i++)
                {
                    Der.set(i*coeff[i]   , i-1) ;
                }
            return Der ;
            
        }

        Polynome Integrale()
        {
            Polynome Der = Polynome(deg +1) ;
            double j(0.0) ;
            for(int i(1) ; i<=deg +1 ;i++)
            {
                Der.set((1.0 / i) * coeff[i - 1], i);
       
            }
            Der.set(0,0) ;

            return Der ;
        }

        double operator[](int i)
        {
            if(i >deg)
            {
                throw std::invalid_argument("Idince qui n'entre pas dans le tableau");
            }
            return coeff[i] ;
        }

        double operator()(double x)
        {
            double result = 0 ;
            for(int i(0) ;i<=deg ;i++)
            {
                result += coeff[i] * pow(x,i);
            }
            return result ;
        }



        Polynome operator +(const Polynome &b) ;
        Polynome operator -(const Polynome &b) ;
        Polynome operator *(const Polynome &b) ;
        friend ostream & operator << ( ostream & out ,const Polynome & p) ;
        friend istream & operator >>( istream & in , Polynome & p) ;
        bool operator ==(const Polynome &b) ;
        bool operator !=(const Polynome &b) ;

};

Polynome Polynome::operator +(const Polynome &b)
{
    int max_deg = max(deg,b.deg) ;
    Polynome c(max_deg);
    if(deg <= max_deg)
    {
        for(int i(0) ; i<=deg;i++)
        {
            c.set(coeff[i] + b.coeff[i],i) ;

        }
        for(int j(deg+1);j<=b.deg;j++)
        {
            c.set(b.coeff[j],j) ;
        }
    }

    if(deg > max_deg)
    {
        for(int i(0) ; i<=b.deg;i++)
        {
            c.set(coeff[i] + b.coeff[i] , i) ;

        }
        for(int j(b.deg+1);j<=deg;j++)
        {
            c.set(coeff[j] , j) ;
        }
    }
    return c ;




}

Polynome Polynome::operator *(const Polynome &b)
{
    Polynome c(deg + b.deg);
    double val(0);
    for(int i(0);i<=deg + b.deg;i++)
    {
        val = 0 ;
        for(int j(0) ; j<= i ;j++)
        {
            val += coeff[j] * b.coeff[i-j] ;
        }
        c.set(val, i) ; 
    }
    return c ;
    


}

ostream & operator << ( ostream & out ,const Polynome & p)
{
    for(int i(0) ;i<= p.deg;i++)
    {
        out << to_string(p.coeff[i]) + "x^" + to_string(i) ;
        if(i!= p.deg ){ out <<" + ";}
    }
    return out ;
}

istream & operator >>( istream & in , Polynome & p)
{
    for(int i(0) ;i<=p.deg ;i++)
    {
        cout <<" coeff: "<<i<<endl;
        in >> p.coeff[i];
    }
    return in ;
}

Polynome Polynome::operator -(const Polynome &b)
{
    int max_deg = max(deg,b.deg) ;
    Polynome c(max_deg);
    if(deg <= max_deg)
    {
        for(int i(0) ; i<=deg;i++)
        {
            c.set(coeff[i] - b.coeff[i],i) ;

        }
        for(int j(deg+1);j<=b.deg;j++)
        {
            c.set(-b.coeff[j],j) ;
        }
    }

    if(deg > max_deg)
    {
        for(int i(0) ; i<=b.deg;i++)
        {
            c.set(coeff[i] - b.coeff[i] , i) ;

        }
        for(int j(b.deg+1);j<=deg;j++)
        {
            c.set(coeff[j] , j) ;
        }
    }
    return c ;



}

bool Polynome::operator ==(const Polynome &b)
{
    if(deg != b.deg)
    {
        return false ;
    }
    for(int i(0) ; i<deg+1 ;i++)
    {
        if(coeff[i] != b.coeff[i])
        {
            return false ;
        }
    }
    return true ;


}

bool Polynome::operator !=(const Polynome &b)
{
    if(deg != b.deg)
    {
        return true ;
    }
    int var = 0 ;
    for(int i(0) ; i<deg+1 ;i++)
    {
        if(coeff[i] == b.coeff[i])
        {
            var++ ;
        }
    }
    if(var == deg+1)
    {
        return false ;
    }
    
    return true ;

}


int main()
{
    Polynome a(2);
    a.Initialisation();
    Polynome b(a) ;
    a.set(1,0) ;
    //a.afficher();
    

    bool d = (b!=a) ;
    cout<<d<<endl;
    cout<<ww<<endl;
    


    return 0 ;
}