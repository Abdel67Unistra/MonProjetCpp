#include <iostream>
int fact(int N){
    if (N == 0){
        return 1 ;
    }
    else {
        return N*fact(N-1);
    }
}

int main() {
    int N , produit=1 ;

    std::cout << "Entrez un nombre N : ";
    std::cin >> N;
    produit = fact(N);
    std::cout<<produit<< std::endl ;

    return 0;
}

