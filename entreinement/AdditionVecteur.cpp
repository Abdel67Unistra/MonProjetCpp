#include<iostream>

/*
Programme ou on rentre un vecteur puis un second et on  les additionne .
*/
void fusion(int tab1[5], int tab2[5],int size5){
    int final[5];
    for(int i(0) ; i<5 ; i++){
        final[i] = tab1[i] + tab2[i] ;
        std::cout<<final[i]<<std::endl ;
    }



}


int main(){
int tab1[5] , tab2[5] ;
int i(0);
for(i=0 ; i<5;i++){
    std::cout<<"Entrez la valleur : "<<i<< " de tab1"<<std::endl;
    std::cin>>tab1[i] ;
    std::cout<<"Entrez la valleur : "<<i<< " de tab2"<<std::endl;
    std::cin>>tab2[i] ;
}
std::cout<<"Votre résultat de la fusion est :"<<std::endl;
fusion(tab1,tab2,5) ;


    return 0 ;
}