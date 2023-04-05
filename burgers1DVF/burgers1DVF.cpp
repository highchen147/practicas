// 
//
// Ecuacion de Burgers en una dimensión
// Resuelta con volumenes finitos
// 
// 
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

// Declaración de funciones
double gauss(double x);
double step_neg(double x);
double step_pos(double x);
double gauss_impar(double x);
void salida(ofstream &of, double *u, double *x, double t, int N);
void cond_frontera_periodica(double *u, double *u_nueva, int N, 
                             double dt, double dx);
double u_prima(double u, double v);
double uPrime(double u, double v);
double Flujo(double u, double v);

int main()
{
    // Parámetros temporales
    const double t_total = 10; // Tiempo total en segundos
    const double dt = 0.001; // Tamaño de paso temporal en segundos
    int Niter = floor(t_total/dt); // Número total de iteraciones
    const int num_outs = 400; // Número de gráficas de instantes temporales
    int out_cada = floor(Niter / num_outs); // Cada out_cada veces se 
                                            // imprimen los valores
    
    double tiempo = 0.0; // Variable de tiempo en la simulación

    // Parámetros espaciales
    int Nx = 500; // Número de puntos en el eje x
    double L = 100.0; // Largo del dominio en metros
    double dx = L/(Nx-1); // Tamaño de paso en el eje x

    // Variables y archivos de salida
    // Se almacena el nombre del marco numérico a utilizar
    // LF: Lex-Friedrichs
    // roe: Roe
    // gudonov: Gudonov
    string Marco;
    cout << "Ingrese el marco numérico a utilizar: gudonov, roe o LF" << endl;
    cin  >> Marco;
    cout << endl;
    while (Marco != "LF" && Marco != "godunov" && Marco != "roe") {
        cout << "Marco numérico inválido. Intente de nuevo con gudonov, roe o LF";
        cin >> Marco;
        cout << "\n" << endl;
    }

    // Se almacena el nombre de la función inicial a utilizar
    // step_neg: Función Heaviside valuada en L/2-x donde L es el largo del dominio en x
    // step_pos: Función Heaviside valuada en L/2+x
    // gauss: Función gaussiana centrada en L/2 con amplitud A
    // gauss_impar: Función gaussiana multiplicada por (x-L/2) con amplitud A
    string funcion_inicial;
    cout << "Escriba la condición inicial a utilizar, puede ser:"<< endl; 
    cout << "step_neg \nstep_pos \ngauss \ngauss_impar" << endl;
    cin  >> funcion_inicial;
    cout << endl;
    while (funcion_inicial != "step_neg" && 
           funcion_inicial != "step_pos" && 
           funcion_inicial != "gauss" && 
           funcion_inicial != "gauss_impar") 
    {
        cout << "Función inválida. Intente de nuevo con:" << endl;
        cout << "step_neg \nstep_pos \ngauss \ngauss_impar" << endl;
        cin >> funcion_inicial;
        cout << "\n" << endl;
    }

    // Se almacena el tipo de condiciones de frontera: fija o periódica
    string frontera;
    cout << "Escriba el tipo de condición de frontera que desea aplicar:" << endl;
    cout << "fija" << endl;
    cout << "periodica" << endl;
    cin >> frontera;
    cout << "\n" << endl;
    while (frontera != "fija" && 
           frontera != "periodica") 
    {
        cout << "Condición inválida. Intente de nuevo con:" << endl;
        cout << "fija \nperiodica" << endl;
        cin >> frontera;
        cout << "\n" << endl;
    }

    string nombreDatos = funcion_inicial + "-" + frontera + ".dat";
    string pathDatos = Marco + "/" + nombreDatos;
    string nombreGraf = funcion_inicial + "-" + frontera + ".gp";
    string pathGraf = Marco + "/" + nombreGraf;
    
    // Escribir archivos
    ofstream salidaDatos;
    salidaDatos.open(pathDatos, ios::out);
    ofstream salidaGraf;
    salidaGraf.open(pathGraf, ios::out);


    ofstream outfile; // Archivo donde se guarda la función solución u
    ofstream out_curves; // Archivo donde se guardan curvas de velocidad
    ofstream gplotmain; // Archivo de gnuplot para graficar la función
    // Nombres de los archivos de datos y de gráficas
    const char *nombre = "step-gudonov2";
    char name_datafile[strlen(nombre) + 4];
    char name_gplotmain[strlen(nombre) + 11];
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
    // Variables de control
    double umax = 0;
    double umin = 0;

    // Inicialización de arreglos
    for (int i = 0; i < Nx; i++)
    {
        x[i] = i*dx;
    }
    // Aplicar condición inicial a u
    for (int i = 0; i < Nx; i++)
    {
        if (funcion_inicial == "step_neg")
        {
            u[i] = step_neg(x[i]);
        }
        else if (funcion_inicial == "step_pos")
        {
            u[i] = step_pos(x[i]);
        }
        else if (funcion_inicial == "gauss")
        {
            u[i] = gauss(x[i]);
        }
        else if (funcion_inicial == "gauss_impar")
        {
            u[i] = gauss_impar(x[i]);
        }
        
        // Encontrar el máximo y mínimo valor de u
        if (u[i] > umax) umax = u[i];
        if (u[i] < umin) umin = u[i];
    }

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
            
            if (u_nueva[i] > umax) umax = u_nueva[i];
            if (u_nueva[i] < umin) umin = u_nueva[i];
        }
        
        
        // Condiciones de frontera
        // cond_frontera_periodica(u, u_nueva, Nx, dt, dx);
        u_nueva[0] = 0.0;
        u_nueva[Nx-1] = 0.0;

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
    // Parámetros de ploteo
    double margen = 0.25;
    double ymax = umax + (umax-umin)*margen;
    double ymin = umin - (umax-umin)*margen;
    // Se escribe el archivo .gp para generar la solución de
    // la evolución temporal
    gplotmain << "# Animación de evolución temporal de Burgers1DVF" << endl;
    gplotmain << "set xrange[0:" << L << "]" << endl;
    gplotmain << "set yrange[" << ymin << ":" << ymax <<"]" << endl;
    gplotmain << "print 'Presione Enter'" << endl;
    gplotmain << "pause -1" << endl;
    gplotmain << endl;
    gplotmain << "do for [i=0:" << num_outs - 1 << "] {" << endl;
    gplotmain << "plot '" << name_datafile << "' index i u 2:3 w l" << endl;
    gplotmain << "pause " << 0.02 << endl;
    gplotmain << "print i" << endl;
    gplotmain << "}";
  
}

/**
 * @brief Aplica condiciones de frontera a la función u pasada como puntero
 * 
 * @param u Puntero al arreglo con los valores que toma la función u
 * @param u_nueva Puntero al arreglo con los valores de la función u 
 * en un instante dt después
 * @param N Tamaño del arreglo u
 * @param dt Tamaño de paso en t
 * @param dx Tamaño de paso en x
 */
void cond_frontera_periodica(double *u, double *u_nueva, int N, 
                             double dt, double dx)
{
    u_nueva[0] = u[0] -(dt/dx)*
            (Flujo(u[0], u[1])-Flujo(u[N-1], u[0]));

    u_nueva[N-1] = u[N-1] -(dt/dx)*
            (Flujo(u[N-1], u[0])-Flujo(u[N-2], u[N-1]));
}

double gauss(double x)
{
    double b = 0.03;
    double mu = 50;
    double A = 3.5;
    return -A*exp(-b*pow(x - mu,2));
}

double step_neg(double x)
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

double step_pos(double x)
{
    return step_neg(-x);
}

double gauss_impar(double x)
{
    double a = 0.4;
    double L = 100;
    return a*(x-L/2)*gauss(x);
}

/**
 * @brief Envía los datos de la función u correspondientes a un instante
 * de tiempo
 * 
 * @param of Archivo de datos
 * @param u Puntero al arreglo que contiene los valores que toma u
 * @param x Puntero al arreglo del dominio espacial
 * @param t Instante temporal en cuestión
 * @param N Tamaño de los arreglos u y x
 */
void salida(ofstream &of, double *u, double *x, double t, int N)
{
    for (int i = 0; i < N; i++)
    {
        of << t << "\t" << x[i] << "\t" << u[i] << endl;
    }
    of << endl << endl;
}

/**
 * @brief Retorna la velocidad adecuada según el marco de Gudonov
 * 
 * @param u Velocidad a la izquierda de la interfaz
 * @param v Velocidad a la derecha de la interfaz
 * @return double: Velocidad a tomar en cuenta para el flujo
 */
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

/**
 * @brief Calcula el flujo según la ec. de Burgers
 * 
 * @param u Velocidad a la izquierda de la interfaz
 * @param v Velocidad a la derecha de la interfaz
 * @return double: Velocidad a usar, según marco de Gudonov 
 */
double Flujo(double u, double v)
{
    return 0.5*pow(uPrime(u,v), 2);
}

double uPrime(double u, double v)
{
    if (u + v > 0)
    {
        return u;
    }
    else if (u + v < 0)
    {
        return v;
    }else
        return 0;
    
}