// 
//
// Ecuacion de Burgers en dos dimensiones
// Resuelta con volumenes finitos
//
//

#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

// Declaración de funciones
double gauss(double x, double y);
double step_neg(double x, double y);
double step_pos(double x, double y);
double gauss_impar(double x, double y);
void salida(ofstream &of, double *u, double *x, double t, int N);
double FlujoBurgers(double u);                        
double Flujo(double u, double v, const string &Marco, double dx, double dt);
double uPrime(double u, double v);
double uProm(double u, double v);
void imprimir(ofstream &file, double xcor, double ycor, double f);
void datos_funciones();

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
    int Ny = 500; // Número de puntos en el eje y
    double Lx = 100.0; // Largo del dominio x en metros
    double Ly = 100.0; // Largo del dominio y en metros
    double dx = Lx/(Nx-1); // Tamaño de paso en el eje x
    double dy = Ly/(Ny-1); // Tamaño de paso en el eje y

    
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
        cout << ">> ";
        cin >> tipo_frontera;
        cout << "\n" << endl;
    }

    // Nombre del archivo con los datos
    string nombreDatos;
    nombreDatos = funcion_inicial + "-" + tipo_frontera + ".dat";
    // Se abre el archivo donde van los datos
    ofstream outfile;
    outfile.open(nombreDatos, ios::out);

    // Arreglos y constantes
    // Función de velocidad en el tiempo actual: u{x, y, t}
    double **u = new double*[Nx];
    // Función de velocidad en el tiempo dt después u{x, y, t+dt}
    double **u_nueva = new double*[Nx];
    // Puntos sobre el eje x
    double *x = new double[Nx];
    // Puntos sobre el eje y
    double *y = new double[Ny];

    // Inicialización de cada arreglo
    for (int i = 0; i < Nx; i++)
    {
        // Función u
        u[i] = new double[Ny];
        u_nueva[i] = new double[Ny];
        // Arreglos dimensionales
        x[i] = i*dx;
    }
    // Arreglo de dominio y
    for (int j = 0; j < Ny; j++)
    {
        y[j] = j*dx;
    }
    

    
}

void salida(ostream &of, double **u, double *x, double *y, double t, int Nx, int Ny )
{
  for( int i=0; i<Nx+1; i++ ){
    for( int j=0; j<Ny+1; j++ ){
        of << t << "\t" << x[i] << "\t" << y[j] << "\t" << u[i][j] << endl;
    }
    of << endl;
  }
  of << endl;
}

double gauss(double x, double y)
{
    double b = 0.02;
    double x0 = 50;
    double y0 = 50; 
    double A = 3.5;
    return A*exp(-b*(pow(x - x0,2) + pow(y-y0,2)));
}

double step_neg(double x, double y)
{
    double L = 100;
    double arg = x - L/2;
    if (arg < 0)
    {
        return 1.0;        
    }
    else
    {
        return 0.0;
    }
}

double step_pos(double x, double y)
{
    double L = 100;
    double arg = -(x - L/2);
    if (arg < 0)
    {
        return 1.0;        
    }
    else
    {
        return 0.0;
    }
}

void imprimir(ofstream &file, double xcor, double ycor, double f)
{
    file << xcor << "\t" << ycor << "\t" << f << endl;
}

void datos_funciones()
{
    ofstream archivo;
    archivo.open("campana.dat", ios::out );
        
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            imprimir(archivo, i, j, step_pos(i,j));
        }
        
    }
}