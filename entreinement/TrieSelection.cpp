#include <iostream>
using namespace std;

void trieCroissant(int lst[], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (lst[j] < lst[i]) {
                // Échange des éléments
                int temp = lst[i];
                lst[i] = lst[j];
                lst[j] = temp;
            }
        }
    }
}

int main() {
    int lst[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int size = 9;

    trieCroissant(lst, size);

    cout << "La liste triee en ordre croissant est: ";
    for (int i = 0; i < size; i++) {
        cout << lst[i] << " ";
    }
    cout << endl;

    return 0;
}
