#include<iostream>
#include<cmath>

int Fibo_recc(int n) 
{
    if(n==0 || n ==1)
    {
        return 1 ;
    }
    else
    {
        return Fibo_recc(n-1) + Fibo_recc(n-2) ;
    }
}

int Fibo_itt(int n)
{


    int Fn ;
    int i = 1;
    int a = 0 ;
    int b  = 1;
    while(i<=n)
    {
        Fn = a+b ;
        a = b ;  //FN-2
        b = Fn; //FN-1
        i++;
    }

    return Fn;
}



int main()
{
//72seconde en reccursive! 
//3seconde en itterative!   pour n = 20

time_t start_clock, end_clock;
start_clock = clock();

 int a = 30 ;
 int b = Fibo_recc(a) ;
 std::cout<<b<<std::endl;

end_clock = clock();
std::cout << "temps d'exécution : " << difftime(end_clock, start_clock)<<"seconde en reccursive! " << std::endl;


time_t start_, end_;
start_ = clock();


 int c = 30 ;
 int d = Fibo_itt(c) ;
 std::cout<<d<<std::endl;


end_ = clock();
std::cout << "temps d'exécution : " << difftime(end_, start_)<<"seconde en itterative! " << std::endl;





    return 0 ;
}
