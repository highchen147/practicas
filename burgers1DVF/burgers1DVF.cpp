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

    // Se imprimen los datos correspondientes al tiempo inicial
    // de la simulación
    tiempo = 0.0;
    salida(outfile, u, x, tiempo, Nx);
        
    // Comienza a correr el tiempo antes de entrar al ciclo principal
    tiempo += dt;
    
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
    cout << endl;    

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
    gplotmain << "plot '" << pathDatos << "' index i u 2:3 w l" << endl;
    gplotmain << "pause " << 0.02 << endl;
    gplotmain << "print i" << endl;
    gplotmain << "}";
    
    }

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
 * @param tipo Tipo de condición: periódica o fija
 * @param marco Marco numérico utilizado: roe, godunov o LF
 * 
 */
void condicion_frontera(double *u, double *u_nueva, int N, 
                        double dt, double dx, const string &tipo, 
                        const string &marco)
{
    if (tipo == "fija")
    {
        u_nueva[0] = u[0];
        u_nueva[N-1] = u[N-1];
    }
    else
    {    
        u_nueva[0] = u[0] -(dt/dx)*
            (Flujo(u[0], u[1], marco, dx, dt)-
             Flujo(u[N-1], u[0], marco, dx, dt));

        u_nueva[N-1] = u[N-1] -(dt/dx)*
            (Flujo(u[N-1], u[0], marco, dx, dt)-
             Flujo(u[N-2], u[N-1], marco, dx, dt));
    }
}

double gauss(double x)
{
    double b = 0.02;
    double mu = 50;
    double A = 3.5;
    return A*exp(-b*pow(x - mu,2));
}

double step_neg(double x)
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

double gauss_impar(double x)
{
    double c = 0.4;
    double L = 100;
    return c*(x-L/2)*gauss(x);
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
 * @brief Flujo general de la ecuación de Burgers, F(u) = 1/2(u^2)
 * 
 * @param u Velocidad en x
 * @return double: Flujo en función de la velocidad u
 */
double FlujoBurgers(double u)
{
    return 0.5*pow(u, 2);
}

/**
 * @brief Calcula el flujo por interfaz según el marco utilizado
 * 
 * @param u Velocidad a la izquierda de la interfaz
 * @param v Velocidad a la derecha de la interfaz
 * @param marco Tipo de marco numérico usado: godunov, roe 
 * o LF (Lax-Friedrichs)
 * @param dx Tamaño de paso en x
 * @param dt Tamaño de paso temporal
 * @return double: Flujo promedio en la i-ésima interfaz 
 */
double Flujo(double u, 
             double v, 
             const string &marco, 
             double dx, 
             double dt)
{
    // Marco de Gudonov
    if (marco == "godunov")
    {
        return FlujoBurgers(uPrime(u, v));
    }
    // Marco de Lax-Friedrichs
    else if (marco == "LF")
    {
        double FlujoPromedio = 0.5*(FlujoBurgers(u)+FlujoBurgers(v));
        return FlujoPromedio-0.5*dx/dt*(v-u);
    }
    // Marco de Roe
    else if (marco == "roe")
    {
        double FlujoPromedio = 0.5*(FlujoBurgers(u)+FlujoBurgers(v));
        return (FlujoPromedio-0.5*abs(uProm(u,v))*(v-u));
    }
    else if (marco == "roe-fix")
    {
        double epsilon = max(0.0, (v-u)/2);
        double FlujoPromedio = 0.5*(FlujoBurgers(u)+FlujoBurgers(v));
        double u_prom = uProm(u,v);
        if (u_prom >= epsilon)
        {
            return (FlujoPromedio-0.5*abs(uProm(u,v))*(v-u));
        }
        else
        {
            return (FlujoPromedio-0.5*abs(epsilon)*(v-u));   
        }
        
    }
    // Flujo por default, toma la velocidad de la izquierda
    else 
        return FlujoBurgers(u);
}

/**
 * @brief Velocidad a usar para el flujo del marco de Godunov
 * 
 * @param u Velocidad a la izquierda
 * @param v Velocidad a la derecha
 * @return double: velocidad elegida
 */
double uPrime(double u, double v)
{

    
    if (u > v)
    {
        if ((u+v)/2 > 0)
        {
            return u;
        }
        else return v;
        
    }
    else if (v > u )
    {
        if (u > 0)
        {
            return u;
        }
        else if (v < 0)
        {
            return v;
        }
        else if ((v > 0) && (u < 0))
        {
            return 0;
        }
        else return 0;
    }
    else return u;
    
    
    
}

/**
 * @brief Velocidad promedio entre celdas para el flujo del 
 * marco de Roe.
 * 
 * @param u Velocidad a la izquierda. También denotada u_{i}
 * @param v Velocidad a la derecha, o u_{i+1}
 * @return double: Velocidad a usar
 */
double uProm(double u, double v)
{
    if (u != v)
    {
        return 0.5*(u + v);
    }
    else
        return u;
}