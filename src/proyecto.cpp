#include <iostream>
#include <fstream>
using namespace std;

struct Hechizo {
    int id_mago;
    string nombre;
    string descripcion;
    Hechizo* next;
};

Mago* crearMago(string datos[]) {
    Mago* nuevo = new Mago();
    nuevo->id = stoi(datos[0]);
    nuevo->name = datos[1];
    nuevo->last_name = datos[2];
    nuevo->gender = datos[3][0];
    nuevo->age = stoi(datos[4]);
    nuevo->id_father = stoi(datos[5]);
    nuevo->is_dead = datos[6] == "1";
    nuevo->type_magic = datos[7];
    nuevo->is_owner = datos[8] == "1";
    nuevo->left = nuevo->right = nullptr;
    nuevo->hechizos = nullptr;
    return nuevo;
}
void insertar(Mago*& raiz, Mago* nuevo) {
    if (!raiz) raiz = nuevo;
    else if (nuevo->id < raiz->id) insertar(raiz->left, nuevo);    
    else insertar(raiz->right, nuevo);
}

Mago* buscarPorId(Mago* raiz, int id) {
    if (!raiz) return nullptr;
    if (raiz->id == id) return raiz;  
    if (id < raiz->id) return buscarPorId(raiz->left, id);
    return buscarPorId(raiz->right, id);
}
