#include<iostream>

/*
Écrire un programme qui saisit deux tableaux de 10 entiers a et b. 
c est un tableau de 20 entiers. Le programme doit mettre dans c la fusion des tableaux a et b.
On copiera dans les 10 premières cases de c le tableau a, dans les dix dernières le tableau b. 
Le programme affiche ensuite le tableau c.

Cet exercice a pour but de vérifier les points techniques suivants :

  -  utilisation simple de tableaux ;
  -  un algorithme simple sur un tableau : fusion de deux tableaux.

*/

int main(){
    int A[10] , B[10], C[20] ;
    for(int i(0) ; i<10 ; i++){
        std::cout<<"Entrez la valeur "<<i<<" ème de A"<<std::endl ;
        std::cin>>A[i] ;
        std::cout<<"Entrez la valeur "<<i<<" ème de B"<<std::endl ;
        std::cin>>B[i] ;
    }
    for(int i(0) ; i<10 ; i++){
        C[i] = A[i] ;
    }
    for(int j(0) ; j<10 ; j++){
        C[10+j] = B[j] ;
    }
    for(int i(0) ; i<20 ; i++){
        std::cout<<C[i]<<std::endl ;
    }


    return 0 ;
}