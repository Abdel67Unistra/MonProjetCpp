#include<iostream>

/*
Écrire un programme qui saisit deux tableaux de 10 entiers a et b qui doivent être triés dans l'ordre croissant.
Le programme devra tout d'abord vérifier que les deux tableaux sont triés. Le tableau c est un tableau de 20 entiers.
Le programme doit mettre dans c la fusion des tableaux a et b.
Le tableau c devra contenir les éléments de a et ceux de b et devra être trié.
Le programme affiche ensuite le tableau c.


*/

void trieSelection(int lst[9]; int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (lst[j] < lst[i])
            {
                int temp = lst[i];
                lst[i] = lst[j];
                lst[j] = temp;
            }
        }
    }
}

void affiche(int A)
{
    for (int i(0); i < 10; i++)
    {
        std::cout << A[i] << std::endl;
    }
}

int main()
{
    int A[10], B[10], C[20];
    for (int i(0); i < 10; i++)
    {
        A[i] = 10 - i;
        B[i] = 10 - i;
    }
    trieSelection(A);
    trieSelection(B);
    affiche(A);
    affiche(B);

    return 0;
}