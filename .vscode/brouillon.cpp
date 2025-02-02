include<iostream>

void swap_ref(int a , int b )
{
    int &c = a ;
    a = b ;
    b = c ;
    std::cout<"a = "<<a<<" ,b = "<<b<<std::endl;

}



int main()
{
int a  = 1 ;
int b = 2 ;
swap_ref(a,b);  
return 0;


}