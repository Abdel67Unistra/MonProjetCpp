#include<iostream>
#include<string>
using namespace std ;

class Vehicule
{
    static int compteur ;
    string marque ;
    int annee ;

    public :
        Vehicule(std::string m , int a)
        {
            marque = m ;
            annee = a ;
            compteur++ ;
        }
    
        virtual void afficher()
        {
            std::cout<< marque <<" et "<< annee <<std::endl ; 
        }

        ~Vehicule()
        {
            std::cout<<"le véhicule est détruit !"<<std::endl ;
        }
        static void nbr_vehicule()
        {
            std::cout<< "Le nombre de vehicule cree est : "<<compteur<<std::endl;
        }


};
int Vehicule::compteur = 0 ;


class Voiture : public Vehicule
{
    int nombrePortes ;
    
    public :
        Voiture(string m ,int a ,int nbrPortes):Vehicule( m ,a)
        {
            nombrePortes = nbrPortes ;
        }

        void afficher() override final
        {
            Vehicule::afficher() ;
            std::cout<<"le nbr de porte est :"<<nombrePortes<<std::endl;
        }


};




int main()
{
    Voiture voit("toyota ", 2025,4) ; 
    //voit.afficher() ;

    Vehicule *P = &voit ; 
    //P->afficher() ;
    Vehicule::nbr_vehicule();

    return 0 ;
}