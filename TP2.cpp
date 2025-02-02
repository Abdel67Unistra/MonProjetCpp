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


double* linspace(double a, double b, int N) {
    double *x = new double[N];
    double h = (b - a) / (N - 1);

    for (int i = 0; i < N; i++) {
        x[i] = a + i * h;
    }

    return x;
}

double* cos_1(double xmin , double xmax , int N){
    double* x = linspace(xmin,xmax,N) ;
    double* y = new double(N);
    for(int i(0); i<N ; i++){
        y[i] = std::cos(x[i]);
    }
    return y ;
} 

double* fonc(double xmin , double xmax , int N, double (*pointeurfonction)(double)){
    double* x = linspace(xmin,xmax,N) ;
    double* y = new double(N);
    for(int i(0); i<N ; i++){
        y[i] = pointeurfonction(x[i]);
        std::cout<<y[i]<<std::endl;
    }
    return y ;
} 

double f2(double x){
    return x+1 ;
}





void SuiteSyracuse(int a){
    int u0 = a; 
    while(u0 !=1){
        if(u0 %2 ==0){
            u0 = u0 / 2 ;
        }
        else{
            u0 = 3* u0 +1 ;
        }
        std::cout<<u0<<std::endl;
    }

}

int* SuiteSyracuse2(int a,int* size) {
    // Utilisation d'un tableau statique de taille initiale
    int* lst = new int[100]; // Ajuste la taille selon les besoins (ici 100 pour l'exemple)
    int u0 = a;
    int i = 0;

    // Remplissage de la suite jusqu'à atteindre 1
    while (u0 != 1) {
        lst[i] = u0; // Enregistre le terme actuel
        i++;
        if (u0 % 2 == 0) {
            u0 = u0 / 2; // Si u0 est pair
        } else {
            u0 = 3 * u0 + 1; // Si u0 est impair
        }
    }

    lst[i] = 1; // Ajoute le dernier terme qui est 1
    *size = i+1 ;
    return lst; // Retourne le tableau
}

/*
int *Nmax(*int Suite){

}
*/

// Fonction pour trouver le N maximal de la suite de Syracuse pour M donné
int CherchionsN(int M) {
    int maxsize = 1;  // Initialiser la taille maximale à 1
    int maxtab = 0;   // Initialiser la valeur correspondant à cette taille maximale

    for (int i = 2; i < M - 1; i++) {
        int size;
        int* Y = SuiteSyracuse2(i, &size);  // Calcul de la suite pour i
        if (size > maxsize) {  // Si la taille de la suite est supérieure à maxsize
            maxsize = size;
            maxtab = i;  // Mémoriser la valeur de i correspondant à cette taille
        }
        delete[] Y;  // Libérer la mémoire
    }


    return maxtab;  // Retourner la valeur qui a généré la plus grande suite
}



void tableau(int tab[] , int n){
    for(int i(0);i<n;i++){
        std::cout<<"Entrez un nbr : "<<std::endl;
        std::cin>>tab[i];
        
    }
}

int *Selection_sort(int *tab,int n )
{

    for(int i(0);i<4;i++)
    {
        int j = i+1;
        for(j= i+1;j<5;j++)
        {
            if(tab[i]>tab[j])
            {
               int ptr = tab[i];
               tab[i] = tab[j];
                tab[j] = ptr ;
            }

        }
        

    }

return tab ;
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

            /*
            int N = 10;  
            double a = 0.0;  
            double b = 1.0; 

            double *valeurs = linspace(a, b, N);
            for(int i(0) ; i<N;i++){
                std::cout<<valeurs[i]<<std::endl;
            }

            double *yeur = cos_1(a,b,N);
            for(int i(0) ; i<N ;i++){
                std::cout<<yeur[i]<<std::endl ;
            }





            delete[] valeurs;
            delete[] yeur  ;

            */
            /*

             double (*pointeurfonction)(double);
             pointeurfonction = f2 ;
            fonc(1,100,100,pointeurfonction);
            

            */
           /*
            int u0 ;
            std::cout<<"Entrez u0"<<std::endl;
            std::cin>>u0; 
            SuiteSyracuse(u0);
            */

            //CherchonsN(5);
            /*
            int *size ;

            int *N =  SuiteSyracuse2(100,size);

            for(int i(0) ; i<*size;i++){

           std::cout<< N[i]<<std::endl;
            */

        /*
           int A = CherchionsN(19);
           std::cout<<A<<std::endl;

        */

        int tab[5] = {5,4,3,2,1};
        int *tab2 = Selection_sort(tab,5);
        for(int i(0);i<5;i++)
        {
            std::cout<<tab2[i]<<std::endl;

        }


            return 0;
 
}

