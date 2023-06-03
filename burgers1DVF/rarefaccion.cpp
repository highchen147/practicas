// 
//
// Ecuacion de Burgers en una dimensión
// Resuelta con volumenes finitos
// 
// 
#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <algorithm>
using namespace std;

void salida(ofstream &of, double *u, double *x, double t, int N);
double step_pos(double x);

int main()
{
    // Parámetros temporales
    const double t_total = 30; // Tiempo total en segundos
    const double dt = 0.001; // Tamaño de paso temporal en segundos
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
    // Función de velocidad en el tiempo actual: u{x, t} = u_i
    double *u = new double[Nx]; 
    // Función de velocidad en el tiempo dt después u{x, t+dt} = u_i+1
    double *u_nueva = new double[Nx]; 
    // Puntos sobre el eje x
    double *x = new double[Nx];

    // Inicialización de arreglos
    for (int i = 0; i < Nx; i++)
    {
        x[i] = i*dx;
    }

    for (int i = 0; i < Nx; i++)
    {
        u[i] = step_pos(x[i]);
    }

    ofstream outfile;
    outfile.open("rarefaccion.dat", ios::out);
    // Se imprimen los datos correspondientes al tiempo inicial
    // de la simulación
    tiempo = 0.0;
    salida(outfile, u, x, tiempo, Nx);

    // Comienza a correr el tiempo antes de entrar al ciclo principal
    tiempo += dt;

    for (int j = 0; j < Niter; j++)
    {
        double u_L = step_pos(0);
        double u_R = step_pos(100);

        for (int i = 0; i < Nx; i++)
        {
            double x_p = x[i] - L/2;
            if (x_p < u_L*tiempo)
            {
                u[i] = u_L;
            }
            else if ((x_p >= u_L*tiempo) && (x_p <= u_R*tiempo))
            {
                u[i] = x_p/tiempo;
            } else
            {
                u[i] = u_R;
            }
        }

        if (j % out_cada == 0)
        {
            salida(outfile, u, x, tiempo, Nx);
            // cout << "\r" << " " << round(100*tiempo/t_total*100)/100 << "%";
            // cout.flush();
        }
        // Actualizamos el tiempo
        tiempo += dt;  
        
    }
}

void salida(ofstream &of, double *u, double *x, double t, int N)
{
    for (int i = 0; i < N; i++)
    {
        of << t << "\t" << x[i] << "\t" << u[i] << endl;
    }
    of << endl << endl;
}

double step_pos(double x)
{
    double L = 100;
    double arg = -(x - L/2);
    if (arg < 0)
    {
        return 1.0;        
    }
    else
    {
        return -1.0;
    }
}