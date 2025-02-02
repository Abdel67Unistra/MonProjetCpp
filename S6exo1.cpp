#include <iostream>
#include<cmath>
double moyenne(double *tab , int n)
{
    double mean = 0;
    for(int i = 0; i < n; i++) {
        mean += tab[i];
    }
    mean /= n;
    return mean;
}

void regression(double *X, double *Y, int n, double &alpha, double &beta)
{
    double X_bar = 0;
    double Y_bar = 0;

    // Calcul des moyennes
    for(int i = 0; i < n; i++) {
        X_bar += X[i];
        Y_bar += Y[i];
    }
    X_bar /= n;
    Y_bar /= n;

    // Calcul du numérateur et du dénominateur
    double num = 0;
    double den = 0;
    for(int i = 0; i < n; i++) {
        num += (X[i] - X_bar) * (Y[i] - Y_bar);
        den += (X[i] - X_bar) * (X[i] - X_bar);
    }

    // Calcul des coefficients
    alpha = num / den;
    beta = Y_bar - alpha * X_bar;
}

double f1(double x)
{
    return 2 * x - 4;
}

double f2(double x)
{
    return std::exp(-x) ;
}

double eval(double x, double alpha, double beta)
{
    return alpha * x + beta;
}

int main() {
    double x[100];
    double y[100];
    for(int i = 0; i < 100; i++) {
        x[i] = -4 + i / 25.0; // Correction ici
        y[i] = f1(x[i]);
    }

    double alpha;
    double beta;
    regression(x, y, 100, alpha, beta);
    std::cout << f1(2) << " et " << eval(2, alpha, beta) << std::endl;


    return 0;
}
