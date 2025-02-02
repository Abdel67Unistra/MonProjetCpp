#include<iostream>
#include<cmath>
#define SIZE 10

void init_tab(int *tab)
{
    for(int i(0) ; i<SIZE ; i++)
    {
        tab[i] = 0 ;
    }

}

void set_tab(int *tab, int i, int v)
{
    tab[i] = v ;
}


int get_size(int *tab)
{
    int i = SIZE -1 ;
    while(tab[i] == 0  && i != 0 )
    {
        i = i-1 ;

    }
    return i ;
}

void compress_tab(int *tab)
{
    int pos = 0; // Position où placer le prochain élément non nul

    for (int i = 0; i < SIZE; i++) 
    {
        if (tab[i] != 0) 
        {
            tab[pos] = tab[i]; // Déplacer l'élément non nul vers la gauche
            pos++;
        }
    }

    // Remplir le reste du tableau avec des zéros
    while (pos < SIZE) 
    {
        tab[pos] = 0;
        pos++;
    }
}

void print_tab(int *tab)
{
    for(int i(0);i<SIZE;i++)
    {
        std::cout<<tab[i] ;
    }
}

int main()
{

    int *tab = new int[10];
    init_tab(tab);
    /*
    for(int i(0); i<SIZE ;i++)
    {
        std::cout<<tab[i]<<std::endl;
    }

    */
    tab[0] = 1;
    tab[1] = 1;
    tab[2] = 2;
    tab[3] = 0;
    tab[4] = 3;
    tab[5] = 4;
    tab[6] = 0;
    tab[7] = 5;
    tab[8] = 0;
    tab[9]=0;
    int d = get_size(tab);
    //std::cout<<d<<std::endl;
    compress_tab(tab) ;

    for(int i(0); i<SIZE ;i++)
    {
        std::cout<<tab[i]<<std::endl;
    }
    delete[] tab ;
    return 0;



}