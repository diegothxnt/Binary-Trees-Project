#include <iostream>
#include <fstream>
using namespace std;

struct Hechizo {
    int id_mago;
    string nombre;
    string descripcion;
    Hechizo* next;
};