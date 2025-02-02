#include <iostream>

int main() {
    int N , i=0 ,somme=0 ;

    std::cout << "Entrez un nombre N : ";
    std::cin >> N;

    std::cout << "Vous avez saisi N = " << N << std::endl;

    while (i<N){
        i++ ;
        somme = somme + i ;
    }
    std::cout << somme <<std::endl;

    return 0;
}

