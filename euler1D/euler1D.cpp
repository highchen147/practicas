#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;

// Sobrecarga de * para multiplicación de escalar por un vector


// Sobrecarga de + para suma de vectores 


double rho_prom(double rho_L, double rho_R);
double u_prom(double u_L, double u_R, double rho_L, double rho_R);
double h_prom(double p_L, double p_R, double u_L, double u_R, double rho_L, double rho_R);
double a_prom(double p_L, double p_R, double rho_L, double rho_R);
vector<double> operator+(const vector<double>& a, const vector<double>& b);
vector<double> operator*(const vector<double>& v, double scalar);

const double Gamma = 1.4;

int main()
{
    // Parámetros temporales
    const double t_total = 30; // Tiempo total en segundos
    const double dt = 0.01; // Tamaño de paso temporal en segundos
    int Niter = floor(t_total/dt); // Número total de iteraciones
    const int num_outs = 1000; // Número de gráficas de instantes temporales
    int out_cada = floor(Niter / num_outs); // Cada out_cada veces se 
                                            // imprimen los valores
    
    double tiempo = 0.0; // Variable de tiempo en la simulación
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

/**
 * @brief Calcula media ponderada de la entalpía
 * respecto a la raíz de densidades vecinas
 * 
 * @param p_L presión a la izquierda
 * @param p_R presión a la derecha
 * @param u_L velocidad a la izquierda
 * @param u_R velocidad a la derecha
 * @param rho_L densidad a la izquierda
 * @param rho_R densidad a la derecha
 * @return double 
 */
double h_prom(double p_L, double p_R, double u_L, double u_R, double rho_L, double rho_R)
{
    // Se calcula la entalpía a la izquierda y a la derecha
    double h_L = Gamma/(Gamma-1.0)*(p_L/rho_L) + 0.5*u_L*u_L;
    double h_R = Gamma/(Gamma-1.0)*(p_R/rho_R) + 0.5*u_R*u_R;
    // Se devuelve la misma media ponderada que con u_prom pero ahora con las entalpías
    return (sqrt(rho_L)*h_L+sqrt(rho_R)*h_R)/
    (sqrt(rho_L) + sqrt(rho_R));
}

/**
 * @brief Calcula la media ponderada de a, correspondiente a velocidad del sonido en casos particulares.
 * También respecto a las densidades vecinas.
 * 
 * @param p_L 
 * @param p_R 
 * @param rho_L 
 * @param rho_R 
 * @return double 
 */
double a_prom(double p_L, double p_R, double rho_L, double rho_R)
{
    return sqrt(Gamma*(p_L/sqrt(rho_L)+p_R/sqrt(rho_R))/(sqrt(rho_L)+sqrt(rho_R)));
}

vector<double> operator+(const vector<double>& a, const vector<double>& b) {
    // Asegurarse de que ambos vectores tengan el mismo tamaño
    if (a.size() != b.size()) {
        throw runtime_error("Los vectores deben tener el mismo tamaño.");
    }
    
    vector<double> result(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        result[i] = a[i] + b[i];
    }
    return result;
}

vector<double> operator*(const vector<double>& v, double scalar) {
    vector<double> result(v.size());
    for (size_t i = 0; i < v.size(); i++) {
        result[i] = v[i] * scalar;
    }
    return result;
}