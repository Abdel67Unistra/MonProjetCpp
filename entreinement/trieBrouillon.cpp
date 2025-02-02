#include<iostream>


void trie(int lst[10]){
    for(int i=0 ; i<10 ; i++){ 
        for(int j=0 ; j< 10 ; j++){
            if (lst[i] <= lst[j]){
                int temp = lst[i] ;
                lst[i] = lst[j] ;
                lst[j] = temp ;
            }

        }
    }
    for(int k = 0 ; k< 10 ; k++){
        std::cout<<lst[k]<<std::endl;

    }

}


int main(){
    int lst[10] , i ;
    for(i=0;i<10;i++){
        std::cout<<"Entrez le nombre "<< i <<" ème svp : "<< std::endl;
        std::cin>>lst[i] ;
        

    }
    trie(lst) ;

    return 0 ;
}