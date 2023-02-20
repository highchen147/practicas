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
void salida_surf(ofstream &of, double *u, double *x, double t, int N);
double RK4( double y, double h, double (*derivada)( double));

int main()
{
    // Parámetros temporales
    const double t_total = 1.2;         // Tiempo total en segundos
    const double dt = 0.000001;            // Tamaño de paso temporal
    int Niter = floor(t_total/dt);      // Número total de iteraciones
    const int num_outs = 48;            // Número de gráficas de instantes temporales
    int out_cada = floor(Niter / num_outs);                  // Cada out_cada veces se imprimen los valores
    

    double tiempo = 0.0;                // Variable que almacena el tiempo en la simulación

    // Parámetros espaciales
    int Nx = 500;                       // Número de puntos en el eje x
    double L = 100.0;                   // Largo del dominio en metros
    double dx = L/(Nx-1);               // Tamaño de paso en el eje x

    // Variables y archivos de salida
    ofstream outfile;                   // Archivo donde se guarda la función solución u
    ofstream out_surf;                  // Archivo donde se guarda la función como superficie
    ofstream out_curves;                // Archivo donde se guardan curvas de velocidad
    ofstream gplotmain;                 // Archivo de gnuplot para graficar la función u(x,t)
    outfile.open("sol-burgers1D.dat", ios::out );
    out_surf.open("sol-burgers1Dsurf.dat", ios::out);
    out_curves.open("curves.gp", ios::out);
    gplotmain.open("burgers1DDF.gp", ios::out);
    bool superficie = false;

    // Arreglos
    double *u = new double[Nx];         // Función de velocidad en el tiempo actual: u{x, t}
    double *u_nueva = new double[Nx];   // Función de velocidad en el tiempo dt después u{x, t+dt}
    double *x = new double[Nx];         // Función de distancia sobre el eje x
    // double *y;

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
    if(not superficie) {
        salida(outfile, u, x, tiempo, Nx);
        // num_outs += 1;
        }
    else salida_surf(out_surf, u, x, tiempo, Nx);
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
            if (not superficie) {
                salida(outfile, u, x, tiempo, Nx);
                // num_outs += 1;
                }
            else salida_surf(out_surf, u, x, tiempo, Nx);
        
        
        // cout << round(1000*tiempo/t_total)/10 << " %" << endl;
        // Actualizamos el tiempo
        tiempo += dt;        
    }
    
    cout << "N_outs = " << num_outs << endl;
    
    // Se escribe el archivo .gp para generar la solución animada de la evolución temporal
    gplotmain << "# Animación de evolución temporal de Burgers1DDF" << endl;
    gplotmain << "set xrange[0:" << L << "]" << endl;
    gplotmain << "set yrange[-1:15]" << endl;
    gplotmain << endl;
    gplotmain << "do for [i=0:" << num_outs - 1 << "] {" << endl;
    gplotmain << "plot 'sol-burgers1D.dat' index i u 2:3 w lp" << endl;
    gplotmain << "pause -1" << endl;
    gplotmain << "print i" << endl;
    gplotmain << "}";
  
}

double f_cond_inicial(double x)
{
    double b = 0.05;
    double mu = 50;
    double A = 3.5;
    return A*exp(-b*pow(x - mu,2));
    // if (x < 50)
    // {
    //     return(0.0);
    // }else return(1.0);
}

void salida(ofstream &of, double *u, double *x, double t, int N)
{
    for (int i = 0; i < N; i++)
    {
        of << t << "\t" << x[i] << "\t" << u[i] << endl;
    }
    of << endl << endl;
}
void salida_surf(ofstream &of, double *u, double *x, double t, int N)
{
    for (int i = 0; i < N; i++)
    {
        of << t << "\t" << x[i] << "\t" << u[i] << endl;
    }
}

double velocidad(double y, const double t, double (*funcion)(double))
{
    return (*funcion)(y);
}

double RK4( double y, 
          double h, 
          double (*derivada)( double)
)
{
    double k0;
    double k1;
    double k2;
    double k3;
    double z;

    k0 = (*derivada)(y);
    z =  y + 0.5*k0*h;
    k1 = (*derivada)(z);
    z = y + 0.5*k1*h;
    k2 = (*derivada)(z);
    z = y + k2*h;
    k3 = (*derivada)(z);
    z = y + h * (k0 + 2*k1 + 2*k2 + k3)/6.0;
    return z;
}
