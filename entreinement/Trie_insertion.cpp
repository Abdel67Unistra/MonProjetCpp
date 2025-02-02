#include<iostream>

void trie_insertion(int lst[10],int len){
    for(int i(1) ; i<10 ; i++){
       int ptr = lst[i] ;
       int j = i-1 ;
       while(j>=0 && lst[j] > ptr){
        lst[j+1] = lst[j] ;
        j -=1 ;
       }
       lst[j+1] = ptr ;


    }


}



int main(){
    int lst[10];
    for(int i(0); i<10 ; i++){
        lst[i] = 10-i ;

    }
    trie_insertion(lst,10) ;

    std::cout<<"Votre liste trieé est :"<<std::endl;
    for(int i=0 ; i<9 ; i++){
        std::cout<<lst[i]<<std::endl  ;

    }


    return 0;
}