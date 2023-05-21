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