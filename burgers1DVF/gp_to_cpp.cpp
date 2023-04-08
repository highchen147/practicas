#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main()
{
    string input_filename = "gauss-fija-general.gp";   // Replace with the name of your input .dat file
    string output_filename = "output-gauss-fija-general.dat";   // Replace with the name of your output .dat file
    string name_ofstream = "grafica_general";
    
    ifstream input_file(input_filename);
    ofstream output_file(output_filename);
    
    if (input_file.is_open() && output_file.is_open())
    {
        string line;
        while (getline(input_file, line))
        {
        output_file << name_ofstream << " " << "<< \"" << line << "\" << endl;" <<'\n';
        }
        cout << "el script de gnuplot " << input_filename << " ha sido adaptado para ser escrito desde un programa cpp en: " << output_filename << endl;
    }
    else
    {
        cout << "Error: Failed to open input or output file!" << endl;
    }
    
    return 0;
}
