#include<iostream>
#include<cmath>

int sum(int* arr, int size)
{
    int somme = 0;
    for(int i(0) ; i<size;i++)
    {
        somme += arr[i];
    }
    return somme ;
}

int produit(int* arr, int size)
{
    int produit = 1 ;
    for(int i(0) ; i<size ;i++)
    {
        produit *= arr[i] ;

    }
    return produit ;
}

int max_value(int* arr, int size)
{
    int max = arr[0];
    for(int i(0) ; i<size ;i++)
    {
        if(arr[i]> max)
        {
            max = arr[i] ;
        }
    }
    return max ;
}


int main()
{
int (*pointeur)(int*arr ,int size) ;
pointeur = max_value ;
int lst[10] = {1,2,3,4,5,6,7,8,9,10} ;
int a =pointeur(lst ,10) ;
std::cout<<a<<std::endl;
    return 0;
}