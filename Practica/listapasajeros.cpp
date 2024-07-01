#include "ListaPasajeros.h"
#include <iostream>

using namespace std;


ListaPasajeros::ListaPasajeros() : cabeza(nullptr), cola(nullptr) {}

ListaPasajeros::~ListaPasajeros() {
    while (cabeza != nullptr) {
        NodoPasajero* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }
}

void ListaPasajeros::insertarOrdenado(const Pasajero& pasajero) {
    NodoPasajero* nuevo = new NodoPasajero{pasajero, nullptr, nullptr};
    if (cabeza == nullptr) {
        cabeza = cola = nuevo;
    } else {
        NodoPasajero* actual = cabeza;
        NodoPasajero* anterior = nullptr;
        while (actual != nullptr && actual->pasajero < pasajero) {
            anterior = actual;
            actual = actual->siguiente;
        }
        if (anterior == nullptr) {
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
            cabeza = nuevo;
        } else {
            nuevo->siguiente = actual;
            nuevo->anterior = anterior;
            anterior->siguiente = nuevo;
            if (actual != nullptr) {
                actual->anterior = nuevo;
            } else {
                cola = nuevo;
            }
        }
    }
}

void ListaPasajeros::mostrar() {
    NodoPasajero* actual = cabeza;
    while (actual != nullptr) {
        actual->pasajero.mostrar();
        actual = actual->siguiente;
    }
}

bool ListaPasajeros::contienePasajero(const string& numeroPasaporte) const {
    NodoPasajero* actual = cabeza;
    while (actual != nullptr) {
        if (actual->pasajero.numero_de_pasaporte == numeroPasaporte) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

void ListaPasajeros::mostrarPasajero(const string& numeroPasaporte) const {
    NodoPasajero* actual = cabeza;
    while (actual != nullptr) {
        if (actual->pasajero.numero_de_pasaporte == numeroPasaporte) {
            //cout << "Pasajero encontrado:" << endl;
            actual->pasajero.mostrar();
            return;
        }
        actual = actual->siguiente;
    }
    //cout << "No se encontro al pasajero con el numero de pasaporte proporcionado." << endl;
}

void ListaPasajeros::graficar() {
    FILE* archivo = fopen("ListaPasajeros.dot", "w");
    if (archivo != nullptr) {
        fprintf(archivo, "digraph G {\n");
        fprintf(archivo, "node [shape=record];\n");
        fprintf(archivo, "rankdir = LR;\n");
        fprintf(archivo, "label = \"Lista de Pasajeros\";\n");
        NodoPasajero* temp = cabeza;
        int i = 0;
        while (temp != nullptr) {
            fprintf(archivo, "node%d [label = \"Nombre: %s | Nacionalidad: %s | Pasaporte: %s | Vuelo: %s | Asiento: %d | Destino: %s | Origen: %s | Equipaje: %d\"];\n",
                    i,
                    temp->pasajero.nombre.c_str(),
                    temp->pasajero.nacionalidad.c_str(),
                    temp->pasajero.numero_de_pasaporte.c_str(),
                    temp->pasajero.vuelo.c_str(),
                    temp->pasajero.asiento,
                    temp->pasajero.destino.c_str(),
                    temp->pasajero.origen.c_str(),
                    temp->pasajero.equipaje_facturado);
            if (temp->siguiente != nullptr) {
                fprintf(archivo, "node%d -> node%d;\n", i, i + 1);
            }
            if (temp->anterior != nullptr) {
                fprintf(archivo, "node%d -> node%d;\n", i, i - 1);
            }
            temp = temp->siguiente;
            i++;
        }
        fprintf(archivo, "}\n");
        fclose(archivo);
        system("dot -Tpng ListaPasajeros.dot -o ListaPasajeros.png");
        system("start ListaPasajeros.png");
    } else {
        std::cout << "Error al abrir el archivo" << std::endl;
    }
}
