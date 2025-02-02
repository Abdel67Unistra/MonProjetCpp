#include<iostream>

void trieSelection(int lst[9],int size){
    for(int i=0;i<size-1;i++){
        for (int j=i+1 ; j< size ; j++){
            if(lst[j]<lst[i]){
                int temp = lst[i] ;
                lst[i]= lst[j] ;
                lst[j] = temp ;
            }
        }
    }


}

int main(){
    int lst[9] ;
    std::cout<<"Votre liste est :"<<std::endl;
    for(int i = 0 ; i<9;i++){
        std::cout<<"Entrez un nombre : "<<std::endl ;
        std::cin>>lst[i] ;

    }
    for(int i=0 ; i<9 ; i++){
        std::cout<<lst[i]<<std::endl  ;

    }

    trieSelection(lst,9) ;
    std::cout<<"Votre liste trieé est :"<<std::endl;
    for(int i=0 ; i<9 ; i++){
        std::cout<<lst[i]<<std::endl  ;

    }


    return 0;
}