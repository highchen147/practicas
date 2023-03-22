// 
//
// Ecuacion de Burgers en una dimensión
// Resuelta con volumenes finitos
// 
// 
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

// Declaración de funciones
double f_cond_inicial(double x);
double step_func(double x);
double gauss_impar(double x);
void salida(ofstream &of, double *u, double *x, double t, int N);
double u_prima(double u, double v);
double Flujo(double u, double v);

int main()
{
    // Parámetros temporales
    const double t_total = 80; // Tiempo total en segundos
    const double dt = 0.01; // Tamaño de paso temporal en segundos
    int Niter = floor(t_total/dt); // Número total de iteraciones
    const int num_outs = 500; // Número de gráficas de instantes temporales
    int out_cada = floor(Niter / num_outs); // Cada out_cada veces se 
                                            // imprimen los valores
    
    double tiempo = 0.0; // Variable de tiempo en la simulación

    // Parámetros espaciales
    int Nx = 500; // Número de puntos en el eje x
    double L = 100.0; // Largo del dominio en metros
    double dx = L/(Nx-1); // Tamaño de paso en el eje x

    // Variables y archivos de salida
    ofstream outfile; // Archivo donde se guarda la función solución u
    ofstream out_curves; // Archivo donde se guardan curvas de velocidad
    ofstream gplotmain; // Archivo de gnuplot para graficar la función
    // Nombres de los archivos de datos y de gráficas
    const char *nombre = "gaussiana";
    char name_datafile[20];
    char name_gplotmain[30];
    sprintf(name_datafile, "%s.dat", nombre);
    sprintf(name_gplotmain, "grafica-%s.gp", nombre);
    outfile.open(name_datafile, ios::out );
    gplotmain.open(name_gplotmain, ios::out);

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
    // Aplicar condición inicial a u
    for (int i = 0; i < Nx; i++)
    {
        u[i] = f_cond_inicial(x[i]);
    }
    // Condiciones de frontera
    double u_0 = 0.0;
    double u_L = 0.0;
    u[0] = u_0;
    u[Nx-1] = u_L;

    // Se imprimen los datos correspondientes al tiempo inicial
    // de la simulación
    salida(outfile, u, x, tiempo, Nx);
        
    // Comienza a correr el tiempo antes de entrar al ciclo principal
    tiempo += dt;
    
    // Ciclo principal
    for (int j = 0; j < Niter; j++)
    {
        for (int i = 1; i < Nx-1; i++)
        {
            u_nueva[i] = u[i] -(dt/dx)*
            (Flujo(u[i], u[i+1])-Flujo(u[i-1], u[i]));
        }
        
        // Condiciones de frontera
        u_nueva[0] = u_0;
        u_nueva[Nx-1] = u_L;

        // Actualizar u
        for (int i = 0; i < Nx; i++)
        {
            u[i] = u_nueva[i];
        }
        
        // Se imprime la solución de la iteración
        if (j % out_cada == 0){
            salida(outfile, u, x, tiempo, Nx);
            cout << 100*tiempo/t_total << endl;
        }
            

        // Actualizamos el tiempo
        tiempo += dt;        
    }
    
    // Se escribe el archivo .gp para generar la solución de
    // la evolución temporal
    gplotmain << "# Animación de evolución temporal de Burgers1DV F" << endl;
    gplotmain << "set xrange[0:" << L << "]" << endl;
    gplotmain << "set yrange[-1:7]" << endl;
    gplotmain << "print 'Presione Enter'" << endl;
    gplotmain << "pause -1" << endl;
    gplotmain << endl;
    gplotmain << "do for [i=0:" << num_outs - 1 << "] {" << endl;
    gplotmain << "plot '" << name_datafile << "' index i u 2:3 w l" << endl;
    gplotmain << "pause 0.05" << endl;
    gplotmain << "print i" << endl;
    gplotmain << "}";
  
}

double f_cond_inicial(double x)
{
    double b = 0.03;
    double mu = 50;
    double A = 3.5;
    return A*exp(-b*pow(x - mu,2));
}

double step_func(double x)
{
    double L = 100;
    if (x < L/2)
    {
        return 5.0;        
    }
    else
    {
        return 0.0;
    }
}

double gauss_impar(double x)
{
    double a = 0.4;
    double L = 100;
    return a*(x-L/2)*f_cond_inicial(x);
}

void salida(ofstream &of, double *u, double *x, double t, int N)
{
    for (int i = 0; i < N; i++)
    {
        of << t << "\t" << x[i] << "\t" << u[i] << endl;
    }
    of << endl << endl;
}

double u_prima(double u, double v)
{
    if (u >= v)
    {
        if (u + v > 0)
        {
            return u;
        }else
            return v;
    }
    else
    {
        if (u > 0)
        {
            return u;
        }
        else if (v < 0)
        {
            return v;
        }else
        {
            return 0;
        }
    }
}

double Flujo(double u, double v)
{
    return 0.5*pow(u_prima(u,v), 2);
}