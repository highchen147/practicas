// 
//
// Ecuacion de Burgers en una dimensión
// Resuelta con diferencias finitas 
// 
// 
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

double f_cond_inicial(double x);
void salida(ofstream &of, double *u, double *x, double t, int N);

int main()
{
    // Parámetros temporales
    double t_total = 100;               // Tiempo total en segundos
    double dt = 0.5;                    // Tamaño de paso temporal
    int Niter = floor(t_total/dt);      // Número total de iteraciones
    double tiempo = 0.0;                // Variable que almacena el tiempo en la simulación

    // Parámetros espaciales
    int Nx = 500;                       // Número de puntos en el eje x
    double L = 100.0;                   // Largo del dominio en metros
    double dx = L/(Nx-1);               // Tamaño de paso en el eje x

    // Variables y archivos de salida
    int out_cada = 1;                   // Cada out_cada veces se imprimen los valores
    ofstream outfile;                   // Archivo donde se guarda la función solución u
    outfile.open("sol-burgers1D.dat", ios::out );

    // Arreglos
    double *u = new double[Nx];         // Función de velocidad en el tiempo actual: u{x, t}
    double *u_nueva = new double[Nx];   // Función de velocidad en el tiempo dt después u{x, t+dt}
    double *x = new double[Nx];         // Función de distancia sobre el eje x

    // Inicialización de arreglos
    for (int i = 0; i < Nx; i++)
    {
        x[i] = i*dx;
    }
    // Aplicar condición inicial a u
    for (int i = 0; i < Nx; i++)
    {
        u[i] = f_cond_inicial(x[i]);
    }
    // Condiciones de frontera
    u[0] = 0.0;
    u[Nx-1] = 0.0;

    // Se imprimen los datos correspondientes al tiempo inicial de la simulación
    salida(outfile, u, x, tiempo, Nx);
    // Comienza a correr el tiempo antes de entrar al ciclo principal
    tiempo += dt;
    
    // Ciclo principal
    for (int j = 0; j < Niter; j++)
    {
        for (int i = 1; i < Nx-1; i++)
        {
            u_nueva[i] = u[i]*(1 + (dt/dx)*(u[i]-u[i+1]));
        }
        
        // Condiciones de frontera
        u_nueva[0] = 0.0;
        u_nueva[Nx-1] = 0.0;

        // Actualizar u
        for (int i = 0; i < Nx; i++)
        {
            u[i] = u_nueva[i];
        }
        
        // Se imprime la solución de la iteración
        if (j % out_cada == 0)
            salida(outfile, u, x, tiempo, Nx);
        
        // Actualizamos el tiempo
        tiempo += dt;
        
        
    }
    
    
}

double f_cond_inicial(double x)
{
    double a = 0.05;
    double mu = 50;
    double s = 1;
    return s*exp(-a*pow(x - mu,2));
}

void salida(ofstream &of, double *u, double *x, double t, int N)
{
    for (int i = 0; i < N; i++)
    {
        of << t << "\t" << x[i] << "\t" << u[i] << endl;
    }
    of << endl << endl;
}