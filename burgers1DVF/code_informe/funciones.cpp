// ESTE CÓDIGO ES SOLO PARA INCLUIR CÓDIGO FUENTE EN EL INFORME
#include <cmath>
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
    // Marco de Godunov
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
    // Marco de Roe corregido
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