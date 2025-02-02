#include<iostream>
#include <ctime>

int fact_recc(int n)
{
    if(n==0)
    {
        return 1 ;
    }
    else
    {
        return n*fact_recc(n-1) ;
    }
}

int fact_itt(int n)
{
    if(n ==0){return 1;}
    int tmp = 1 ;
    int i = 1 ;
    while(i <= n)
    {
        tmp = tmp*i ;
        i++ ;
    }
    return tmp ;
}


int main()
{
    /*
    int n =16 ; //Jusqu'à 16 donne la bonne réponse !! ensuite on a des reponse négatives .
    int l = fact_recc(n);
    std::cout<<l<<std::endl;
*/
/*
    int m =16  ; //Jusqu'à 16 donne la bonne réponse !! ensuite on a des reponse négatives .
    int j = fact_itt(m);
    std::cout<<j<<std::endl;

*/


time_t start_clock, end_clock;
start_clock = clock();


    int n =16 ; //Jusqu'à 16 donne la bonne réponse !! ensuite on a des reponse négatives .
    int l = fact_recc(n);


end_clock = clock();
std::cout << "temps d'exécution : " << difftime(end_clock, start_clock)<<"seconde en reccursive! " << std::endl;


//

time_t start_, end_;
start_ = clock();


int m =16  ; //Jusqu'à 16 donne la bonne réponse !! ensuite on a des reponse négatives .
    int j = fact_itt(m);

end_ = clock();
std::cout << "temps d'exécution : " << difftime(end_, start_)<<"seconde en itterative! " << std::endl;

/*

temps d'exécution : 3seconde en reccursive! 
temps d'exécution : 0seconde en itterative! 

*/






    return 0 ;
}

