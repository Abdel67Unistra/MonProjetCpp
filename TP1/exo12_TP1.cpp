#include<iostream>
#include<string>

void ajoute_entier(int lst[10]) {
    int val;
    std::string cond;
    std::cout << "Voulez-vous ajouter une valeur à la liste ? (oui/non)" << std::endl;
    std::cin >> cond;
    
    if (cond == "oui") {
        std::cout << "Quelle valeur voulez-vous ajouter :" << std::endl;
        std::cin >> val;
    } else {
        return;  // Sort de la fonction si la réponse n'est pas "oui"
    }
    
    // Trouver la première position vide (valeur 0) dans le tableau
    int i = 0;
    while (i < 10 && lst[i] != 0) {
        i += 1;
    }
    
    // Ajouter la valeur dans la première case vide si trouvée
    if (i < 10) {
        lst[i] = val;
    } else {
        std::cout << "La liste est pleine, impossible d'ajouter la valeur." << std::endl;
    }
}

void affiche(int lst[10]) {
    std::cout << "Contenu de la liste :" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << lst[i] << std::endl;
    }
}

void moyenne(int lst[10]) {
    int somme = 0;
    for (int i = 0; i < 10; i++) {
        somme += lst[i];
    }
    
    int moyenne = somme / 10;
    std::cout << "La moyenne est : " << moyenne << std::endl;
}

void Supprimer(int lst[10]){
    int val;
    std::string cond;
    std::cout << "Voulez-vous Supprimer une valeur à la liste ? (oui/non)" << std::endl;
    std::cin >> cond;
    if(cond == "oui"){
        std::cout<<"insserer la valeur à supprimer : "<<std::endl ;
        std::cin>>val ;

    }
    for(int i(0) ; i<10 ; i++){
        if(lst[i] == val){
            lst[i] = 0 ;
        }
    }
    affiche(lst);

}

int main() {
    int lst[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    ajoute_entier(lst);  // Ajout d'un entier à la liste
    //affiche(lst);        // Affiche la liste
    //moyenne(lst);        // Calcule et affiche la moyenne
    Supprimer(lst) ;
    
    return 0;
}
