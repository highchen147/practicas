#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    string nombre;
    ofstream archivo;
    archivo.open("hola.dat", ios_base::app);
    // archivo.seekp(0, ios);
    // archivo << endl;
    archivo << "mundo" << endl;
    archivo.close();
}