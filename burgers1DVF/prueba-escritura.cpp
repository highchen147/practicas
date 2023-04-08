#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    string nombre;
    ofstream archivo;
    archivo.open("hola.dat", ios::in | ios::ate);
    archivo.seekp(0);
    // archivo << endl;
    archivo << "mundo" << endl;
    archivo.close();
}