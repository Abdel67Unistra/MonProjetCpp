#include<iostream>

int main(){

    int a = 1;
    int b = 2 ;
    int c =3 ; 
    int *p_1  ;
    int *p_2 ;
    std::cout<<"int a ; int b ; int c ; int *p_1  ; int *p_2 ;  "<<std::endl;
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    p_1 = &a ;
    p_2 = &c ;
    std::cout<<"  p_1 = &a ; p_2 = &c ; "<<std::endl;
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    *p_1 = *p_2  ;
    std::cout<<" *p_1 = *p_2  ;  "<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    (*p_2) ++;
    std::cout<<" (*p_2) ++; " <<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;


    p_1 = p_2 ;
    std::cout<<" p_1 = p_2 ; "<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    p_2 = &b ;
    std::cout<<" p_2 = &b ; " <<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    *p_2 = *p_1 -2* *p_2 ;
    std::cout<< "  *p_2 = *p_1 -2* *p_2 ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    (*p_2) -- ;
    std::cout<<"(*p_2) -- ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    *p_1 = *p_2 - c ;
    std::cout<<"*p_1 = *p_2 - c ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;
    
    a = (2 + *p_2) * *p_1 ;
    std::cout<<"a = (2 + *p_2) * *p_1 ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    p_2 = &c ;
    std::cout<<"p_2 = &c ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    *p_2 = *p_1 / *p_2 ;
    std::cout<<"*p_2 = *p_1 / *p_2 ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    *p_1 = a+b ;
    std::cout<<"*p_1 = a+b ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    a+= *p_1 ;
    std::cout<<"a+= *p_1 ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    b = *p_1 + *p_2 ;
    std::cout<<"b = *p_1 + *p_2 ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    *p_1 = 2 * a ;
    std::cout<<"*p_1 = 2 * a ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    a = *p_2 ;

    std::cout<<" a = *p_2 ; "<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;


    *p_1 = 1 - c ;
    std::cout<<"*p_1 = 1 - c ;"<<std::endl;
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;

    *p_2 += *p_1 + a ;
    std::cout<<"*p_2 += *p_1 + a ;"<<std::endl;
     std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<p_1<<std::endl;
    std::cout<<p_2<<std::endl;




return 0;





}