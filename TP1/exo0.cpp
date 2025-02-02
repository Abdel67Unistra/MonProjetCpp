#include<iostream>

void affiche_entier_while(){
    int a = 8 ;
    int b = 23 ;
    int i = 0 ;
    while(i<b){
        std::cout<<i<<std::endl;
        i++ ;
    }
}

void affiche_entier(){
    int a = 8 ;
    int b = 23 ;
    for(int i(0) ; i<b ; i++){
        std::cout<<i<<std::endl;
    }
}

void moyenne(){
    int ptr(0) ;
    int lst[5];
    for(int i(0) ; i<5; i++){
        std::cout<<"Entrez un nbr"<<std::endl ;
        std::cin>>lst[i] ;
    }
    for(int i(0) ; i<5; i++){
        ptr += lst[i];
    }
    ptr = ptr / 5 ;
    std::cout<<ptr<<std::endl; 
}

void gagne(){
    int val ;
    std::cout<<"Entrez une valeur : ";
    std::cin>>val;
    if (val >=  56 && val  <= 78){
        std::cout<<"gagne"<<std::endl ;
    } else {
        std::cout<<"perdu"<<std::endl ;
    }
}

void AetB(){
    int A, B, ptr ;
    std::cout<<"Entrez A : ";
    std::cin>>A ;
    std::cout<<"Entrez B : ";
    std::cin>>B ;
    ptr = A ;
    A = B;
    B = ptr;
    std::cout<<"A = "<<A<<" B = "<<B<<std::endl;
}

void perimetre(int L , int l){
    int P = 2 * (L + l);
    int S = L * l;
    std::cout<<"Le périmetre est " << P << " et la surface est de " << S <<std::endl;
}

void PlusGrandEntierTab(){
    int tab[5];
    int ptr;
    int indice ;
    for(int i(0); i<5; i++){
        std::cout<<"Entrez un nbr :"<<std::endl ;
        std::cin>>tab[i] ;
        if(i == 0){
            ptr = tab[i];
            indice = i;
        }
        if (ptr < tab[i]){
            ptr = tab[i];
            indice = i;
        }
    }
    std::cout<<"Le plus grand élément est : " << ptr << " d'indice " << indice << std::endl;
}

void Occurence() {
    int tab[5];
    int ptr;
    int occurence = 0;

    for(int i(0); i<5; i++){
        std::cout<<"Entrez un nbr :"<<std::endl ;
        std::cin>>tab[i] ;
        if(i == 0){
            ptr = tab[i];
        }
        if (ptr < tab[i]){
            ptr = tab[i];
        }
    }

    for(int i(0); i<5; i++){
        if(ptr == tab[i]){
            occurence += 1;
        }
    }

    std::cout << "Le nombre d'occurences du plus grand élément est : " << occurence << std::endl;
}




int main(){
    //perimetre(8,5);
    //AetB();
    //gagne();
    //moyenne();
    //affiche_entier();
    //affiche_entier_while();
    //PlusGrandEntierTab();
    Occurence();
    return 0;
}
