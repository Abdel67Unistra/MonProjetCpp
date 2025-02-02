#include<iostream>
#include<cmath>
double distance(double xa , double ya , double xb , double yb){
    return sqrt(pow(xb-xa,2) + pow(yb-ya,2)) ;



}


bool isPrime(int n){
    if(n<2){
        return false ;
    }
    for(int i(2); i<n;i++){
        if(n% i ==0){
            return false ;
        }    
    }
    return true ;
}

int somme_tab(int tab[4],int N){
    int somme(0);
    for(int i(0);i  <N ;i++){
       somme += tab[i];

    }
    return somme ;

}

double f_1(double x){
    bool test ;
    test = (x-1)*(2-x) > 0 ;
    if(test == false){
        x= 2 ;
        test = true ;
    }
    return sqrt((x-1)*(2-x));
    

}

int swap_pointeur(int *pta ,int *ptb ){
    int d = *pta;
    *pta = *ptb ;
    *ptb = d ;
    std::cout<<"a = "<<*pta <<", b = "<< *ptb<<std::endl;
    return 0;
}

int swap_reference(int& a,int& b){
    int temp = a ;
    a = b ;
    b = temp ;
    std::cout<<"a = "<<a<<", b = "<< b<<std::endl;
    return 0;

     



}
bool tab_V(int tab[],int n ,int& v){
    bool s = false ;
    for(int i(0) ; i<n;i++){
        if(tab[i] == v){
            v = i ;
            std::cout<<v<<std::endl;
            s = true ;

        }

    }
    return s ;


}

double linspace(double a , double b , int N ){
    int x[N] ;
    double h = (b-a)/N ;
    for(int i(0) ;i<=N ; i++){
            x[i] = a + i*h;

        }
    return x ;


}


int main(){

            //double d1 = distance( 2.2,1.5,3.6,4.2);
            //std::cout<<"La distance est de "<<d1<<std::endl;

            // Vérification de la primalité du nombre 7
            //bool a = isPrime(9);
            //std::cout<< a << std::endl;  // Afficher le résultat de isPrime
            //int tab[4] = {1, 2, 3, 4}; // Déclaration et initialisation correcte
            //int somme = somme_tab(tab, 4);
            //std::cout << somme << std::endl;
            //std::cout<<f_1(1.5)<<std::endl;
            //int a = 1;
            //int b = 2 ;
            //swap_pointeur(&a,&b);
            //swap_reference(a,b);
            //int tab[4] = {1,2,3,4};
            //int n = 3 ;
            //tab_V(tab,4,n);
            linspace(1,10,10);



    return 0;


 
}