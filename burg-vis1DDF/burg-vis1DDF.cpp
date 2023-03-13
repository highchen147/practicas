// 
//
// Ecuacion de Burgers viscosa en una dimensión
// Resuelta con diferencias finitas 
// 
// Para eliminar todos los archivos de solución
// rm sol-burg-vis1DDF-nu-*
// Para eliminar todos los archivos de gráfica
// rm burg-vis1DDF-nu-*

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
    // Tiempo total en segundos
    const double t_total = 8;
    // Tamaño de paso temporal
    const double dt = 0.001;
    // Número total de iteraciones
    int Niter = floor(t_total/dt);
    // Número de gráficas de instantes temporales
    const int num_outs = 400;
    // Cada out_cada veces se imprimen los valores
    int out_cada = floor(Niter / num_outs);
    // Variable de tiempo en la simulación
    double tiempo = 0.0;

    // Parámetros espaciales
    int Nx = 500;       // Número de puntos en el eje x
    double L = 100.0;   // Largo del dominio en metros
    double dx = L/(Nx-1);   // Tamaño de paso en el eje x

    // Arreglos y constantes
    // Parámetro de viscosidad
    const double nu = 0.3;
    // Función de velocidad en el tiempo actual: u{x, t}
    double *u = new double[Nx];
    // Función de velocidad en el tiempo dt después u{x, t+dt}
    double *u_nueva = new double[Nx];
    // Función de distancia sobre el eje x
    double *x = new double[Nx];

    // Variables y archivos de salida
    // Archivo donde se guarda la función solución u
    ofstream outfile;
    // Archivo de gnuplot para graficar la función u(x,t)
    ofstream gplotmain;
    // Nombres de los archivos de datos y de gráficas
    char name_datafile[31];
    char name_gplotmain[28];
    sprintf(name_datafile, "sol-burg-vis1DDF-nu-%.2f.dat", nu);
    sprintf(name_gplotmain, "burg-vis1DDF-nu-%.2f.gp", nu);
    // Se crean los archivos de datos y gráficas
    outfile.open(name_datafile, ios::out );
    gplotmain.open(name_gplotmain, ios::out);


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
            u_nueva[i] = u[i] + 0.5*(dt/dx)*(pow(u[i], 2)-pow(u[i+1], 2))
            + nu*(dt/dx)*(u[i+1]-2*u[i]+u[i-1])/dx;
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
    
    cout << "N_outs = " << num_outs << endl;
    
    // Se escribe el archivo .gp para generar la solución animada de la evolución temporal
    gplotmain << "# Animación de evolución temporal de Burgers viscosa" << endl;
    gplotmain << "set xrange[0:" << L << "]" << endl;
    gplotmain << "set yrange[-1:15]" << endl;
    gplotmain << endl;
    gplotmain << "pause -1" << endl;
    gplotmain << "do for [i=0:" << num_outs - 1 << "] {" << endl;
    gplotmain << "plot '" << name_datafile << "' index i u 2:3 w l" << endl;
    gplotmain << "pause " << t_total/num_outs << endl;
    gplotmain << "print i" << endl;
    gplotmain << "}";
  
}

double f_cond_inicial(double x)
{
    double b = 0.05;
    double mu = 50;
    double A = 3.5;
    return A*exp(-b*pow(x - mu,2));
}

void salida(ofstream &of, double *u, double *x, double t, int N)
{
    for (int i = 0; i < N; i++)
    {
        of << t << "\t" << x[i] << "\t" << u[i] << endl;
    }
    of << endl << endl;
}
