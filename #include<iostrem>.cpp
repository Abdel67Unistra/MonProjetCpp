#include<iostrem>

/*
Écrire une fonction de signature void circ_permut(int n, int *from, int *to) qui prend en entrée un entier n 
et un tableau from de taille n et qui renvoie dans to le tableau obtenu en permutant les éléments de from de manière circulaire. 
Par exemple, si from contient les éléments 1, 2, 3, 4, 5 alors to contiendra les éléments 5, 1, 2, 3, 4.
*/

/*
Écrire une autre fonction void circ_permut(int n, int *fromto) qui prend en entrée un entier n et un tableau fromto de taille n 
et qui renvoie dans le même tableau fromto le tableau obtenu en permutant les éléments comme ci-dessus.

Pour ces deux fonctions, on supposera n>0 et from, to et fromto sont des pointeurs vers les premiers éléments de tableaux d'au moins n entiers.
*/


void circ_permut(int n, int *from, int *to)
{
to = new int[n]
to[0] = from[n-1]
for(int i = 0 ;i<n-1;i++)
{
    to[i+1] = from[i]  ; 

}



}



int main()
    {
        int n = 4 ;
        int*from = {1,2,3,4};
        int *to;
        circ_permut(n,from,to);
        for(int i(0);i<4;i++)
        {
            std::cout<<to[i]<<std::endl;
        }

        return 0 ;

    }
