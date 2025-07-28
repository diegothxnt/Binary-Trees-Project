#include <iostream>
#include <fstream>
using namespace std;

struct Hechizo {
    int id_mago;
    string nombre;
    string descripcion;
    Hechizo* next;
};
struct Mago {
    int id;
    string name;
    string last_name; 
    char gender;
    int age;
    int id_father;
    bool is_dead;
    string type_magic;
    bool is_owner;
    Mago* left;
    Mago* right;
    Hechizo* hechizos;
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
void mostrarSucesion(Mago* raiz) {
    if (!raiz) return;
    if (!raiz->is_dead) {
        cout << raiz->name << " " << raiz->last_name;
        if (raiz->is_owner) cout << " (Dueno actual)";
        cout << endl;
    }
    mostrarSucesion(raiz->left); 
    mostrarSucesion(raiz->right);
}
Mago* buscarDueñoActual(Mago* raiz) {
    if (!raiz) return nullptr;
    if (raiz->is_owner) return raiz;
    Mago* izquierda = buscarDueñoActual(raiz->left); 
    if (izquierda) return izquierda;
    return buscarDueñoActual(raiz->right);
}
Mago* encontrarSucesor(Mago* raiz, Mago* actual) {  
    if (!raiz || !actual) return nullptr;

    
    for (Mago* temp = raiz; temp; ) {
        if (temp->id_father == actual->id && !temp->is_dead) {
            if (temp->type_magic == "elemental" || temp->type_magic == "unique") return temp;
        }
        if (actual->id < temp->id) temp = temp->left;
        else temp = temp->right;
    }

    
    for (Mago* temp = raiz; temp; ) {
        if (temp->id_father == actual->id && !temp->is_dead && temp->type_magic == "mixed") return temp;
        if (actual->id < temp->id) temp = temp->left;
        else temp = temp->right;
    }

    
    for (Mago* temp = raiz; temp; ) {
        if (temp->id_father == actual->id && !temp->is_dead && temp->gender == 'H') return temp;
        if (actual->id < temp->id) temp = temp->left;
        else temp = temp->right;
    }

    
    Mago* maestro = buscarPorId(raiz, actual->id_father);
    if (maestro) {
        for (Mago* temp = raiz; temp; ) {
            if (temp->id_father == maestro->id && temp->id != actual->id && !temp->is_dead) {
                return temp;
            }
            if (maestro->id < temp->id) temp = temp->left;
            else temp = temp->right;
        }
    }

    
    Mago* mujerMasJoven = nullptr;
    for (Mago* temp = raiz; temp; ) {
        if (temp->gender == 'M' && !temp->is_dead) {
            bool tieneDiscipulos = false;
            for (Mago* d = raiz; d; ) {
                if (d->id_father == temp->id && !d->is_dead) {
                    tieneDiscipulos = true;
                    break;
                }
                if (temp->id < d->id) d = d->left;
                else d = d->right;
            }
            if (tieneDiscipulos && (!mujerMasJoven || temp->age < mujerMasJoven->age)) {
                mujerMasJoven = temp;
            }
        }
        if (temp->id > 0) temp = temp->left;
        else temp = temp->right;
    }

    return mujerMasJoven;
}
void asignarNuevoDueño(Mago* raiz) {
    Mago* dueñoActual = buscarDueñoActual(raiz);
    if (!dueñoActual || !dueñoActual->is_dead) return;

    Mago* sucesor = encontrarSucesor(raiz, dueñoActual);
    if (sucesor) { 
        dueñoActual->is_owner = false;
        sucesor->is_owner = true;
        cout << "Nuevo dueno asignado: " << sucesor->name << " " << sucesor->last_name << endl;
    } else {
        cout << "No se pudo asignar un nuevo dueno automaticamente" << endl;
    }
}
void modificarMago(Mago* raiz) {
    int id;
    cout << "Ingrese ID del mago a modificar: ";
    cin >> id;

    Mago* mago = buscarPorId(raiz, id);
    if (!mago) { 
        cout << "Mago no encontrado" << endl;
        return;
    }

    cout << "Modificando a " << mago->name << " " << mago->last_name << endl;
    cout << "Nuevo nombre (actual: " << mago->name << "): ";
    cin >> mago->name;
    cout << "Nuevo apellido (actual: " << mago->last_name << "): ";
    cin >> mago->last_name;
    cout << "Nuevo genero (H/M) (actual: " << mago->gender << "): ";
    cin >> mago->gender;
    cout << "Nueva edad (actual: " << mago->age << "): ";
    cin >> mago->age;
    cout << "Esta muerto? (1/0) (actual: " << mago->is_dead << "): ";
    cin >> mago->is_dead;
    cout << "Tipo de magia (elemental/unique/mixed/no_magic) (actual: " << mago->type_magic << "): ";
    cin >> mago->type_magic;

    if (mago->is_dead && mago->is_owner) {
        mago->is_owner = false;
        asignarNuevoDueño(raiz);
    }
}
void cargarHechizos(Mago* raiz) {
    ifstream archivo("bin/hechizos.csv");
    if (!archivo) {
        cout << "No se pudo abrir el archivo hechizos.csv" << endl;  
        return;
    }

    string linea;
    getline(archivo, linea); 

    while (getline(archivo, linea)) {
        size_t pos1 = linea.find(',');
        size_t pos2 = linea.find(',', pos1+1);
        
        int id_mago = stoi(linea.substr(0, pos1));
        string nombre = linea.substr(pos1+1, pos2-pos1-1);
        string descripcion = linea.substr(pos2+1);

        Mago* mago = buscarPorId(raiz, id_mago);
        if (mago) {
            Hechizo* nuevo = new Hechizo();
            nuevo->id_mago = id_mago;
            nuevo->nombre = nombre;
            nuevo->descripcion = descripcion;
            nuevo->next = mago->hechizos;
            mago->hechizos = nuevo;
        }
    }
}
void mostrarHechizos(Mago* raiz) {
    int id;
    cout << "Ingrese ID del mago: ";
    cin >> id;

    Mago* mago = buscarPorId(raiz, id);
    if (!mago) {
        cout << "Mago no encontrado" << endl;
        return;
    }

    cout << "\nHechizos de " << mago->name << " " << mago->last_name << ":\n";
    cout << "Tipo de magia: " << mago->type_magic << "\n\n";

    if (!mago->hechizos) {
        cout << "Este mago no tiene hechizos registrados\n";
        return;
    }

    Hechizo* actual = mago->hechizos;
    while (actual) {
        cout << "Hechizo: " << actual->nombre << "\n";
        cout << "Descripcion: " << actual->descripcion << "\n\n";
        actual = actual->next;
    }
}
void cargarDatos(Mago*& raiz) {
    ifstream archivo("bin/magos.csv");
    if (!archivo) {
        cout << "No se pudo abrir el archivo magos.csv" << endl;
        return;
    }
    string linea;
    getline(archivo, linea); 

    while (getline(archivo, linea)) {
        string datos[9];
        int pos = 0;
        size_t inicio = 0;
        size_t fin = linea.find(',');

        while (fin != string::npos) {
            datos[pos++] = linea.substr(inicio, fin - inicio);
            inicio = fin + 1;
            fin = linea.find(',', inicio);
        }
        datos[pos] = linea.substr(inicio);

        Mago* nuevo = crearMago(datos); 
        insertar(raiz, nuevo);
    }
    cargarHechizos(raiz);
    cout << "Datos cargados correctamente" << endl;
}
int main() {
    Mago* raiz = nullptr;
    cargarDatos(raiz);

    int opcion;
    do {
        cout << "\nMenu del Hechizo del Mago\n";
        cout << "1. Mostrar linea de sucesion\n";
        cout << "2. Asignar nuevo dueno (simular muerte)\n";
        cout << "3. Modificar mago\n";
        cout << "4. Mostrar hechizos de un mago\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                cout << "\nLinea de sucesion actual:\n";
                mostrarSucesion(raiz);
                break;
            case 2: {
                Mago* dueño = buscarDueñoActual(raiz);
                if (dueño) {
                    cout << "Simulando muerte de " << dueño->name << endl;
                    dueño->is_dead = true;
                    asignarNuevoDueño(raiz);
                } else {
                    cout << "No hay dueno actual" << endl;
                }
                break;
            }
            case 3:
                modificarMago(raiz);
                break;
            case 4:
                mostrarHechizos(raiz);
                break;
            case 5:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida\n";
        }
    } while (opcion != 5);

    return 0;
}

