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
    double t_total = 0.8;               // Tiempo total en segundos
    double dt = 0.001;                   // Tamaño de paso temporal
    int Niter = floor(t_total/dt);      // Número total de iteraciones
    double tiempo = 0.0;                // Variable que almacena el tiempo en la simulación
    double vel_x;

    // Parámetros espaciales
    int Nx = 500;                       // Número de puntos en el eje x
    double L = 100.0;                   // Largo del dominio en metros
    double dx = L/(Nx-1);               // Tamaño de paso en el eje x

    // Variables y archivos de salida
    int out_cada = 25;                  // Cada out_cada veces se imprimen los valores
    ofstream outfile;                   // Archivo donde se guarda la función solución u
    ofstream out_surf;                  // Archivo donde se guarda la función como superficie
    ofstream out_curves;                // Archivo donde se guardan curvas de velocidad
    outfile.open("sol-burgers1D.dat", ios::out );
    out_surf.open("sol-burgers1Dsurf.dat", ios::out);
    out_curves.open("curves-vel.dat", ios::out);
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
    if(not superficie) salida(outfile, u, x, tiempo, Nx);
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
            if (not superficie) {salida(outfile, u, x, tiempo, Nx);}
            else salida_surf(out_surf, u, x, tiempo, Nx);
        
        // Actualizamos el tiempo
        tiempo += dt;
    }
    
    // Ciclo para producir curvas correspondientes a la evolución independiente
    // de la velocidad
    int num_curvas = 20;
    t_total = 6e-5;
    dt = t_total/100;
    for (int i = 0; i <= num_curvas; i++)
    {
        tiempo = 0.0;
        // x(0) = condicion inicial para x
        double min_x = 40;
        double max_x = 60;
        double intervalo = (max_x-min_x)/(num_curvas-1);
        vel_x = f_cond_inicial(min_x + i*intervalo);
        out_curves << vel_x + min_x + i*intervalo << "\t" << tiempo << endl;
        for (int j = 0; j < 10; j++)
        {
            // double h = 1;
            vel_x += RK4(vel_x, dt, f_cond_inicial);
            // vel_x = f_cond_inicial(vel_x)*dt;
            tiempo += dt;
            out_curves << vel_x + min_x + i*intervalo << "\t" << tiempo << endl;
        }
        out_curves << endl << endl;
    }
    
    
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
