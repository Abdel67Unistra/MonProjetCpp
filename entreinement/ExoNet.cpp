#include<iostream>

/*
Écrire un programme qui demande à l'utilisateur de saisir 10 entiers stockés dans un tableau ainsi qu'un entier V.
Le programme doit rechercher si V se trouve dans le tableau et doit supprimer la première occurrence de V en décalant 
d'une case vers la gauche les éléments suivants et en rajoutant un 0 à la fin du tableau. Le programme doit ensuite afficher
le table finalement.

Cet exercice a pour but de vérifier les points techniques suivants :

-utilisation simple de tableaux ;
-un algorithme simple sur un tableau : suppression d'un élément avec décalage des suivants.


*/

int main(){
    int tab[10] ,  i=0 , v ;
    std::cout<<"Entrez une par une les valeur du tableau ! "<<std::endl;
    for(i=0;i<10;i++){
        std::cout<<"Entrez la valeur "<<i<< "du tableau"<<std::endl;
        std::cin>>tab[i] ;
        
    }
    std::cout<<"Entrez une valeur V "<<std::endl;
    std::cin>>v ;
    i=0 ;
    while((tab[i]!= v) && ( i<=10)){
        i += 1 ;
        }
    
    int ptr = 0 ;
    for(int a = i ; a<10;a++){
        tab[a] = tab[a+1];
        ptr = a ;

    }
    if((tab[ptr] != v) && (ptr == 9 )){
    tab[9] = 0 ;
    }

    
    /*
    Reste à afficher la nouvelle liste !
    */ 

   for(int i(0) ; i<10 ; i++){
    std::cout<< tab[i] <<std::endl;

   }


    return 0;
}

