#include "ArbolBBusqueda.h"
#include <iostream>
#include <fstream>


void ArbolBinarioBusqueda::insertar(Piloto piloto) {
    insertar(raiz, piloto);
}

void ArbolBinarioBusqueda::insertar(NodoBST*& nodo, Piloto piloto) {
    if (nodo == nullptr) {
        nodo = new NodoBST(piloto);
    } else if (piloto.horas_de_vuelo < nodo->piloto.horas_de_vuelo) {
        insertar(nodo->izquierda, piloto);
    } else {
        insertar(nodo->derecha, piloto);
    }
}

void ArbolBinarioBusqueda::eliminar(const std::string& numero_de_id) {
    raiz = eliminarRecursivo(raiz, numero_de_id);
}

NodoBST* ArbolBinarioBusqueda::eliminarRecursivo(NodoBST* nodo, const std::string& numero_de_id) {
    if (nodo == nullptr) {
        return nullptr;
    }

    // Recorrer el árbol
    if (numero_de_id < nodo->piloto.numero_de_id) {
        nodo->izquierda = eliminarRecursivo(nodo->izquierda, numero_de_id);
    } else if (numero_de_id > nodo->piloto.numero_de_id) {
        nodo->derecha = eliminarRecursivo(nodo->derecha, numero_de_id);
    } else {
        // Nodo encontrado, realizar la eliminación
        NodoBST* temp = nodo;
        if (nodo->izquierda == nullptr) {
            nodo = nodo->derecha;
            delete temp;
        } else if (nodo->derecha == nullptr) {
            nodo = nodo->izquierda;
            delete temp;
        } else {
            // Caso: nodo tiene dos hijos
            temp = buscarMinimo(nodo->derecha);
            nodo->piloto = temp->piloto;
            nodo->derecha = eliminarRecursivo(nodo->derecha, temp->piloto.numero_de_id);
        }
    }

    // Continuar buscando y eliminando
    if (nodo != nullptr) {
        nodo->izquierda = eliminarRecursivo(nodo->izquierda, numero_de_id);
        nodo->derecha = eliminarRecursivo(nodo->derecha, numero_de_id);
    }

    return nodo;
}

NodoBST* ArbolBinarioBusqueda::buscarMinimo(NodoBST* nodo) {
    NodoBST* actual = nodo;
    while (actual->izquierda != nullptr) {
        actual = actual->izquierda;
    }
    return actual;
}


void ArbolBinarioBusqueda::recorridoPreorden() {
    recorridoPreorden(raiz);
    cout << endl;
}

void ArbolBinarioBusqueda::recorridoPreorden(NodoBST* nodo) {
    if (nodo != nullptr) {
        cout << " " << nodo->piloto.horas_de_vuelo;
        recorridoPreorden(nodo->izquierda);
        recorridoPreorden(nodo->derecha);
    }
}

void ArbolBinarioBusqueda::recorridoInorden() {
    recorridoInorden(raiz);
    cout << endl;
}

void ArbolBinarioBusqueda::recorridoInorden(NodoBST* nodo) {
    if (nodo != nullptr) {
        recorridoInorden(nodo->izquierda);
        cout << " " << nodo->piloto.horas_de_vuelo;
        recorridoInorden(nodo->derecha);
    }
}

void ArbolBinarioBusqueda::recorridoPostorden() {
    recorridoPostorden(raiz);
    cout << endl;
}

void ArbolBinarioBusqueda::recorridoPostorden(NodoBST* nodo) {
    if (nodo != nullptr) {
        recorridoPostorden(nodo->izquierda);
        recorridoPostorden(nodo->derecha);
        cout << " " << nodo->piloto.horas_de_vuelo;
    }
}

// Declaración de la función generarDot adaptada para BST
void ArbolBinarioBusqueda::generarDot(NodoBST* nodo, ofstream& archivo, bool isLeft) {
    if (nodo == nullptr) return;

    // Escribir el nodo actual
    archivo << "    \"" << nodo->piloto.horas_de_vuelo << "\";\n";

    // Si hay nodo izquierdo, conectar y seguir recursivamente
    if (nodo->izquierda != nullptr) {
        archivo << "    \"" << nodo->piloto.horas_de_vuelo << "\" -> \""
                << nodo->izquierda->piloto.horas_de_vuelo << "\";\n";
        generarDot(nodo->izquierda, archivo, true);
    }

    // Si hay nodo derecho, conectar y seguir recursivamente
    if (nodo->derecha != nullptr) {
        archivo << "    \"" << nodo->piloto.horas_de_vuelo << "\" -> \""
                << nodo->derecha->piloto.horas_de_vuelo << "\";\n";
        generarDot(nodo->derecha, archivo, false);
    }
}

// Función para graficar el árbol BST
void ArbolBinarioBusqueda::graficarBST(ArbolBinarioBusqueda& arbol) {
    string nombreArchivo = "ArbolPilotos.dot";
    ofstream archivo(nombreArchivo);
    archivo << "digraph G {\n";
    archivo << "    node [shape=circle];\n"; // Cambiar el shape a circle

    // Generar DOT desde la raíz del árbol
    generarDot(arbol.raiz, archivo, false);

    archivo << "}\n";
    archivo.close();

    // Generar la imagen PNG y abrir el archivo
    system("dot -Tpng ArbolPilotos.dot -o ArbolPilotos.png");
    system("start ArbolPilotos.png"); // Para Windows, abrirá la imagen en visor predeterminado
}
