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

int main()
{
    ofstream archivo;
    archivo.open("campana.dat", ios::out );
        
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            imprimir(archivo, i, j, step_neg(i,j));
        }
        
    }
    
}

void salida( ostream &of, double **u, double *x, double *y, double t, int Nx, int Ny )
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