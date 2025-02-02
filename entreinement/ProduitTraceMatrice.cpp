#include<iostream>
/*
Produit d'une matrice par trace matricielle 
*/
int ProduitMatriceTrace(int lst1[3][3],int lst2[3][3]){
    int i(3);
    int produit = 1 ;
    for(i = 0 ; i<3 ; i++){
        for(int j = 0 ; j<3; j++){
            if(i==j){
                produit *= lst1[i][j] * lst2[i][j] ;
            }
        }
    }
    std::cout<<produit<<std::endl;
    return 0 ;
        }
        


int main(){
    int lst1[3][3] ;
    int lst2[3][3];
    for(int i = 0 ; i<3 ;i++){
        for(int j = 0 ; j<3; j++){
        std::cout<<"Entrez le nombre : [ "<<i<<" ][ "<< j<<" ] de votre matrice 1" <<std::endl ;
        std::cin>>lst1[i][j] ;

        std::cout<<"Entrez le nombre : [ "<<i<<" ][ "<< j<<" ] de votre matrice 2" <<std::endl ;
        std::cin>>lst2[i][j] ;

        }
    }
    ProduitMatriceTrace(lst1 , lst2) ;
    
 

    return 0;
}
