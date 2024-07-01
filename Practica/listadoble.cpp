#include "Listadoble.h"
#include <iostream>
#include <fstream>

Listadoble::Listadoble(string nombre) : inicio(nullptr), nombreLista(nombre) {}

Listadoble::~Listadoble() {
    while (!estaVacia()) {
        eliminar(inicio->avion.numero_de_registro);
    }
}

bool Listadoble::estaVacia() const {
    return inicio == nullptr;
}

void Listadoble::insertarAlFinal(Avion avion) {
    NodoAvion* nuevo = new NodoAvion();
    nuevo->avion = avion;
    nuevo->siguiente = nullptr;
    nuevo->anterior = nullptr;

    if (estaVacia()) {
        inicio = nuevo;
    } else {
        NodoAvion* actual = inicio;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
        nuevo->anterior = actual;
    }
}

void Listadoble::mostrar() const {
    if (estaVacia()) {
        cout << "La lista de " << nombreLista << " esta vacia." << endl;
        return;
    }

    NodoAvion* actual = inicio;
    while (actual != nullptr) {
        cout << "Numero de registro: " << actual->avion.numero_de_registro << ", Estado: " << actual->avion.estado << endl;
        actual = actual->siguiente;
    }
}

void Listadoble::moverAvion(Listadoble& otraLista, string numero_de_registro) {
    NodoAvion* actual = inicio;
    while (actual != nullptr) {
        if (actual->avion.numero_de_registro == numero_de_registro) {
            Avion avion = actual->avion;
            if (nombreLista == "Disponibles") {
                avion.estado = "Mantenimiento";
            } else if (nombreLista == "Mantenimiento") {
                avion.estado = "Disponible";
            }

            eliminar(numero_de_registro);
            otraLista.insertarAlFinal(avion);
            cout << "Avion movido exitosamente a la lista de " << otraLista.nombreLista << "." << endl;
            return;
        }
        actual = actual->siguiente;
    }

    cout << "No se encontro ningun avion con el numero de registro proporcionado en la lista de " << nombreLista << "." << endl;
}

void Listadoble::eliminar(string numero_de_registro) {
    if (estaVacia()) {
        cout << "La lista de " << nombreLista << " esta vacia, no se puede eliminar ningun avion." << endl;
        return;
    }

    NodoAvion* actual = inicio;
    while (actual != nullptr) {
        if (actual->avion.numero_de_registro == numero_de_registro) {
            if (actual == inicio) {
                inicio = actual->siguiente;
                if (inicio != nullptr) {
                    inicio->anterior = nullptr;
                }
                delete actual;
            } else {
                actual->anterior->siguiente = actual->siguiente;
                if (actual->siguiente != nullptr) {
                    actual->siguiente->anterior = actual->anterior;
                }
                delete actual;
            }
            //cout << "Avion eliminado exitosamente de la lista de " << nombreLista << "." << endl;
            return;
        }
        actual = actual->siguiente;
    }

    cout << "No se encontro ningun avion con el numero de registro proporcionado en la lista de " << nombreLista << "." << endl;
}

bool Listadoble::contieneAvion(const string& numero_de_registro) const {
    NodoAvion* actual = inicio;
    while (actual != nullptr) {
        if (actual->avion.numero_de_registro == numero_de_registro) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

void Listadoble::graficar(const std::string &archivo) {
    if (estaVacia()) {
        std::cout << "Lista sin elementos" << std::endl;
        return;
    }

    std::ofstream archivoSalida(archivo + ".dot");
    if (archivoSalida.is_open()) {
        archivoSalida << "digraph G {" << std::endl;
        archivoSalida << "rankdir = LR;" << std::endl;
        archivoSalida << "node [shape = record];" << std::endl;

        NodoAvion* actual = inicio;
        int i = 0;
        while (actual != nullptr) {
            archivoSalida << "node" << i << " [label = \"Numero de Registro: " << actual->avion.numero_de_registro << "\"];" << std::endl;
            actual = actual->siguiente;
            i++;
        }

        for (int j = 0; j < i; j++) {
            archivoSalida << "node" << j << " -> node" << (j + 1) % i << ";" << std::endl;
            archivoSalida << "node" << (j + 1) % i << " -> node" << j << ";" << std::endl;
        }

        archivoSalida << "}" << std::endl;
        archivoSalida.close();

        std::string comando = "dot -Tpng " + archivo + ".dot -o " + archivo + ".png";
        int resultado = system(comando.c_str());
        if (resultado == 0) {
            //std::cout << "Archivo PNG generado exitosamente: " << archivo << ".png" << std::endl;
            std::string comando2 = "start " + archivo + ".png";
            system(comando2.c_str());
        } else {
            std::cerr << "Error al generar el archivo PNG." << std::endl;
        }
    } else {
        std::cerr << "Error al abrir el archivo DOT." << std::endl;
    }
}

