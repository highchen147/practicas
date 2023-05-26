//  ESTE CÓDIGO NO FUNCIONA, ES PARA INCLUIR EN EL INFORME
#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <algorithm>
using namespace std;

// Declaración de funciones
double gauss(double x);
double step_neg(double x);
double step_pos(double x);
double gauss_impar(double x);
void salida(ofstream &of, double *u, double *x, double t, int N);
void condicion_frontera(double *u, double *u_nueva, int N, 
                        double dt, double dx, const string &tipo, 
                        const string &marco);
double FlujoBurgers(double u);                        
double Flujo(double u, double v, const string &Marco, double dx, double dt);
double uPrime(double u, double v);
double uProm(double u, double v);

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

    // Variables y archivos de salida
    // Se almacena el nombre de la función inicial a utilizar
    // step_neg: Función Heaviside valuada en L/2-x donde L es el largo del dominio en x
    // step_pos: Función Heaviside valuada en L/2+x
    // gauss: Función gaussiana centrada en L/2 con amplitud A
    // gauss_impar: Función gaussiana multiplicada por (x-L/2) con amplitud A
    string funcion_inicial;
    cout << "Escriba la condición inicial a utilizar, puede ser:"<< endl; 
    cout << "step_neg \nstep_pos \ngauss \ngauss_impar \ngauss_neg" << endl;
    cout << ">> ";
    cin  >> funcion_inicial;
    cout << endl;
    while (funcion_inicial != "step_neg" && 
           funcion_inicial != "step_pos" && 
           funcion_inicial != "gauss" && 
           funcion_inicial != "gauss_impar" &&
           funcion_inicial != "gauss_neg") 
    {
        cout << "Función inválida. Intente de nuevo con:" << endl;
        cout << "step_neg \nstep_pos \ngauss \ngauss_impar" << endl;
        cout << ">> ";
        cin >> funcion_inicial;
        cout << "\n" << endl;
    }

    // Se almacena el tipo de condiciones de frontera: fija o periódica
    string tipo_frontera;
    cout << "Escriba el tipo de condición de frontera que desea aplicar:" << endl;
    cout << "fija" << endl;
    cout << "periodica" << endl;
    cout << ">> ";
    cin >> tipo_frontera;
    cout << "\n" << endl;
    while (tipo_frontera != "fija" && 
           tipo_frontera != "periodica") 
    {
        cout << "Condición inválida. Intente de nuevo con:" << endl;
        cout << "fija \nperiodica" << endl;
        cin >> tipo_frontera;
        cout << "\n" << endl;
    }

    // Se escribe arreglo que almacena tipos de marcos utilizados.
    string marcos[] = {"godunov", "roe", "LF", "roe-fix"};
    int num_marcos = sizeof(marcos) / sizeof(marcos[0]);

    // Ciclo principal que resuelve con todos los marcos implementados
    // la ecuación de Burgers
    for (int i = 0; i < num_marcos; i++)
    {
    string marco = marcos[i];
    // nombre del archivo .dat con los resultados numéricos
    string nombreDatos = funcion_inicial + "-" + tipo_frontera + ".dat";
    // ruta de los datos numéricos
    string pathDatos = marco + "/" + nombreDatos;
    // nombre del archivo .gp que grafica los datos
    string nombreGraf = funcion_inicial + "-" + tipo_frontera + ".gp";
    // ruta del archivo de la gráfica
    string pathGraf = marco + "/" + nombreGraf;
    
    // Escribir archivos
    // ofstream salidaDatos;
    // salidaDatos.open(pathDatos, ios::out);
    // ofstream salidaGraf;
    // salidaGraf.open(pathGraf, ios::out);
    ofstream outfile; // Archivo donde se guarda la función solución u
    ofstream out_curves; // Archivo donde se guardan curvas de velocidad
    ofstream gplotmain; // Archivo de gnuplot para graficar la función
    // Nombres de los archivos de datos y de gráficas
    outfile.open(pathDatos, ios::out );
    gplotmain.open(pathGraf, ios::out);

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
        else if (funcion_inicial == "gauss_neg")
        {
            u[i] = -gauss(x[i]);
        }
        
        // Encontrar el máximo y mínimo valor de u
        if (u[i] > umax) umax = u[i];
        if (u[i] < umin) umin = u[i];
    }

    ////////////////////////////////////////////////////////////////////////

// Ciclo principal de integración
for (int j = 0; j < Niter; j++)
{
for (int i = 1; i < Nx-1; i++)
{
    u_nueva[i] = u[i] -(dt/dx)*
    (Flujo(u[i], u[i+1], marco, dx, dt)-
     Flujo(u[i-1], u[i], marco, dx, dt));
    
    if (u_nueva[i] > umax) umax = u_nueva[i];
    if (u_nueva[i] < umin) umin = u_nueva[i];
}


// Condiciones de frontera
condicion_frontera(u, u_nueva, Nx, dt, dx, tipo_frontera, marco);

// Actualizar u
for (int i = 0; i < Nx; i++)
{
    u[i] = u_nueva[i];
}

// Se imprime la solución de la iteración
if (j % out_cada == 0)
{
    salida(outfile, u, x, tiempo, Nx);
    cout << "\r" << marco << " " << round(100*tiempo/t_total*100)/100 << "%";
    cout.flush();
}
    

// Actualizamos el tiempo
tiempo += dt;        
}