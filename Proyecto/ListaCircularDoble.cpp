#include "ListaCircularDoble.h"
#include <iostream>
#include <fstream>


void ListaCircularDoble::insertar(Avion avion) {
    Nodo* nuevo = new Nodo(avion);
    if (cabeza == nullptr) {
        cabeza = nuevo;
        cabeza->siguiente = cabeza;
        cabeza->anterior = cabeza;
    } else {
        Nodo* ultimo = cabeza->anterior;
        nuevo->siguiente = cabeza;
        cabeza->anterior = nuevo;
        nuevo->anterior = ultimo;
        ultimo->siguiente = nuevo;
    }
}

void ListaCircularDoble::mostrar() {
    if (cabeza == nullptr) {
        cout << "La lista esta vacia." << endl;
        return;
    }
    Nodo* temp = cabeza;
    do {
        cout << " " << temp->avion.numero_de_registro;
        temp = temp->siguiente;
    } while (temp != cabeza);
    cout << endl;
}

bool ListaCircularDoble::buscar(string numero_de_registro) {
    if (cabeza == nullptr) return false;
    Nodo* temp = cabeza;
    do {
        if (temp->avion.numero_de_registro == numero_de_registro)
            return true;
        temp = temp->siguiente;
    } while (temp != cabeza);
    return false;
}

void ListaCircularDoble::eliminar(string numero_de_registro) {
    if (cabeza == nullptr) return;
    Nodo* temp = cabeza;
    do {
        if (temp->avion.numero_de_registro == numero_de_registro) {
            if (temp == cabeza) {
                Nodo* ultimo = cabeza->anterior;
                cabeza = cabeza->siguiente;
                ultimo->siguiente = cabeza;
                cabeza->anterior = ultimo;
            } else {
                Nodo* anterior = temp->anterior;
                Nodo* siguiente = temp->siguiente;
                anterior->siguiente = siguiente;
                siguiente->anterior = anterior;
            }
            delete temp;
            return;
        }
        temp = temp->siguiente;
    } while (temp != cabeza);
}

bool ListaCircularDoble::obtenerAvionPorNumeroRegistro(string numero_de_registro, Avion& avion) {
    if (cabeza == nullptr) return false;
    Nodo* temp = cabeza;
    do {
        if (temp->avion.numero_de_registro == numero_de_registro) {
            avion = temp->avion;
            return true;
        }
        temp = temp->siguiente;
    } while (temp != cabeza);
    return false;
}

bool ListaCircularDoble::obtenerAvionPorVuelo(string numero_de_vuelo, Avion& avion) {
    if (cabeza == nullptr) return false;
    Nodo* temp = cabeza;
    do {
        if (temp->avion.vuelo == numero_de_vuelo) {
            avion = temp->avion;
            return true;
        }
        temp = temp->siguiente;
    } while (temp != cabeza);
    return false;
}

void ListaCircularDoble::generarArchivoDOT(const std::string& archivo) const {
    if (cabeza == nullptr) {
        //std::cout << "Lista vacia. No se puede generar el archivo DOT." << std::endl;
        return;
    }

    std::ofstream archivoSalida(archivo + ".dot");
    if (!archivoSalida.is_open()) {
        std::cerr << "Error al abrir el archivo DOT." << std::endl;
        return;
    }

    archivoSalida << "digraph G {" << std::endl;
    archivoSalida << "rankdir = LR;" << std::endl;
    archivoSalida << "node [shape = record];" << std::endl;

    Nodo* temp = cabeza;
    int i = 0;
    do {
        archivoSalida << "node" << i << " [label = \"Numero de Registro: " << temp->avion.numero_de_registro << "\"];" << std::endl;
        temp = temp->siguiente;
        ++i;
    } while (temp != cabeza);

    for (int j = 0; j < i; ++j) {
        archivoSalida << "node" << j << " -> node" << (j + 1) % i << ";" << std::endl;
        archivoSalida << "node" << (j + 1) % i << " -> node" << j << ";" << std::endl;
    }

    archivoSalida << "}" << std::endl;
    archivoSalida.close();

    std::string comando = "dot -Tpng " + archivo + ".dot -o " + archivo + ".png";
    int resultado = system(comando.c_str());
    if (resultado == 0) {
        std::string comando2 = "start " + archivo + ".png";
        system(comando2.c_str());
    } else {
        std::cerr << "Error al generar el archivo PNG." << std::endl;
    }
}