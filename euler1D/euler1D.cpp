#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

double gamma = 1.5;
double rho_prom(double rho_L, double rho_R);
double u_prom(double u_L, double u_R, double rho_L, double rho_R);

int main()
{

}

/**
 * @brief Calcula media geométrica entre densidades vecinas
 * 
 * @param rho_L Densidad a la izquierda
 * @param rho_R Densidad a la derecha
 * @return double 
 */
double rho_prom(double rho_L, double rho_R)
{
    return sqrt(rho_L*rho_R);
}

/**
 * @brief Calcula media ponderada de velocidades 
 * respecto a la raiz de densidades vecinas
 * 
 * @param u_L velocidad a la izquierda
 * @param u_R velocidad a la derecha
 * @param rho_L densidad a la izquierda
 * @param rho_R densidad a la derecha
 * @return double 
 */
double u_prom(double u_L, double u_R, double rho_L, double rho_R)
{
    return (sqrt(rho_L)*u_L+sqrt(rho_R)*u_R)/
    (sqrt(rho_L) + sqrt(rho_R));
}

double h_prom(double p_L, double p_R, double u_L, double u_R, double rho_L, double rho_R)
{
    // Se calcula la entalpía a la izquierda y a la derecha
    double h_L = gamma/(gamma-1)*(p_L/rho_L) + 0.5*u_L*u_L;
    double h_R = gamma/(gamma-1)*(p_R/rho_R) + 0.5*u_R*u_R;
    // Se devuelve la misma media ponderada que con u_prom pero ahora con las entalpías
    return (sqrt(rho_L)*h_L+sqrt(rho_R)*h_R)/
    (sqrt(rho_L) + sqrt(rho_R));
}