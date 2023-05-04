#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;


double rho_inicial(double x);
double p_inicial(double x);
double u_inicial(double x);
void calc_componentes_Q(double *q1, double *q2, double *q3, double *rho, double *p, double *u, int Nx);
void calc_componentes_F(double *F1, double *F2, double *F3, double *rho, double *p, double *u, int Nx);
double rho_prom(double rho_L, double rho_R);
double u_prom(double u_L, double u_R, double rho_L, double rho_R);
double h_prom(double p_L, double p_R, double u_L, double u_R, double rho_L, double rho_R);
double a_prom(double p_L, double p_R, double rho_L, double rho_R);
vector<double> suma_k(double p_L, double p_R, double u_L, double u_R, double rho_L, double rho_R);
vector<double> operator+(const vector<double>& a, const vector<double>& b);
vector<double> operator*(const vector<double>& v, double scalar);
vector<double>& operator+=(vector<double>& v1, const vector<double>& v2);

const double Gamma = 4;

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

    // Parámetros espaciales
    int Nx = 500; // Número de puntos en el eje x
    double L = 100.0; // Largo del dominio en metros
    double dx = L/(Nx-1); // Tamaño de paso en el eje x
     
    // Arreglos
    // Cantidades físicas
    double *rho = new double[Nx]; // Densidad
    double *u = new double[Nx]; // Velocidad
    double *p = new double[Nx]; // Presión
    // Componentes del vector Q
    double *q1 = new double[Nx];
    double *q2 = new double[Nx];
    double *q3 = new double[Nx];
    // Componentes del vector F
    double *F1 = new double[Nx];
    double *F2 = new double[Nx];
    double *F3 = new double[Nx];
    // Puntos sobre el eje x
    double *x = new double[Nx];

    // Inicialización de arreglos
    // Dominio espacial
    for (int i = 0; i < Nx; i++)
    {
        x[i] = i*dx;
    }
    // Densidad
    for (int i = 0; i < Nx; i++)
    {
        rho[i] = rho_inicial(x[i]);
    }
    // Presión
    for (int i = 0; i < Nx; i++)
    {
        p[i] = p_inicial(x[i]);
    }
    // Velocidad
    for (int i = 0; i < Nx; i++)
    {
        u[i] = u_inicial(x[i]);
    }

    // Se calculan las componentes del vector Q de acuerdo a su definición 
    calc_componentes_Q(q1, q1, q3, rho, p, u, Nx);

    // Se calculan las componentes del vector F, que representa el flujo
    calc_componentes_F(F1, F2, F3, rho, p, u, Nx);

    vector<double> res = suma_k(0, 4, 0, 0, 1, 1);
    
    for (double i : res){
        cout << i << endl;
    }
    cout << endl;
}

/**
 * @brief Función inicial de la velocidad
 * 
 * @param x Posición en x
 * @return double 
 */
double u_inicial(double x)
{
    return 0.0;
}

/**
 * @brief Función inicial de la presión
 * 
 * @param x Posición en x
 * @return double 
 */
double p_inicial(double x)
{
    double atm = (1.01325e5);
    double L = 100;
    if (x > L/2)
    {
        return 2*atm;
    }
    else
    {
        return atm;    
    }
}

/**
 * @brief Función inicial de la densidad
 * 
 * @param x Posición en x
 * @return double 
 */
double rho_inicial(double x)
{
    // Densidad del aire en kg/m^3
    double d_aire = 1.29;
    return 1.0*d_aire;
}

/**
 * @brief Asignar valores a las componentes del vector Q
 * 
 * @param q1 Componente 1 de Q
 * @param q2 Componente 2 de Q
 * @param q3 Componente 3 de Q
 * @param rho Densidad
 * @param p Presión
 * @param u Velocidad
 * @param Nx Tamaño de los arreglos que almacenan las funciones
 */
void calc_componentes_Q(double *q1, double *q2, double *q3, double *rho, double *p, double *u, int Nx)
{
    for (int i = 0; i < Nx; i++)
    {
        q1[i] = rho[i];
        q2[i] = rho[i]*u[i];
        q3[i] = p[i]/(Gamma-1) + 0.5*rho[i]*pow(u[i], 2);
    }
    
}

/**
 * @brief Asignar valores a las componentes del vector F (flujo)
 * 
 * @param F1 Componente 1 de F
 * @param F2 Componente 2 de F
 * @param F3 Componente 3 de F
 * @param rho Densidad
 * @param p Presión
 * @param u Velocidad
 * @param Nx Tamaño de los arreglos que almacenan las funciones
 */
void calc_componentes_F(double *F1, double *F2, double *F3, double *rho, double *p, double *u, int Nx)
{
    for (int i = 0; i < Nx; i++)
    {
        F1[i] = rho[i]*u[i];
        F2[i] = p[i] + rho[i]*pow(u[i], 2);
        F3[i] = u[i]*(p[i]/(Gamma-1) + 0.5*rho[i]*pow(u[i], 2) + p[i]); 
    }
    
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

vector<double> suma_k(double p_L, double p_R, double u_L, double u_R, double rho_L, double rho_R)
{
    double u = u_prom(u_L, u_R, rho_L, rho_R);
    double rho = rho_prom(rho_L, rho_R);
    double h = h_prom(p_L, p_R, u_L, u_R, rho_L, rho_R);
    double a = a_prom(p_L, p_R, rho_L, rho_R);
    double dp = p_R - p_L;
    double du = u_R - u_L;
    double drho = rho_R - rho_L;
    // Fuerzas de las ondas, alfa
    double alfa_1 = 0.5*(dp-rho*a*du)/pow(a, 2);
    double alfa_2 = (pow(a, 2)*drho-dp)/pow(a, 2);
    double alfa_3 = 0.5*(dp+rho*a*du)/pow(a, 2);
    vector<double> alfa = {alfa_1, alfa_2, alfa_3};
    // Autovalores de las ondas
    vector<double> lambda = {u-a, u, u+a};
    // Autovectores
    vector<double> e_1 = {1, u-a, h-u*a};
    vector<double> e_2 = {1, u, 0.5*pow(u,2)};
    vector<double> e_3 = {1, u+a, h+u*a};
    // vector de vectores
    vector<vector<double>> e_vec = {e_1, e_2, e_3};
    // Se declara el vector resultante
    vector<double> resultado(3, 0);

    // Se realiza la suma
    for (int i = 0; i < 3; i++)
    {
        resultado += e_vec[i]*(alfa[i]*abs(lambda[i]));
    }
    return resultado;
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

vector<double>& operator+=(vector<double>& v1, const vector<double>& v2) {
    if (v1.size() != v2.size()) {
        throw invalid_argument("Los vectores deben tener el mismo tamaño.");
    }

    for (size_t i = 0; i < v1.size(); i++) {
        v1[i] += v2[i];
    }

    return v1;
}