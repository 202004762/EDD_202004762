#include "TablaHash.h"


TablaHash::TablaHash(int tamanoInicial)
    : M(tamanoInicial) {
    tabla = new NodoPiloto*[M];
    for (int i = 0; i < M; ++i) {
        tabla[i] = nullptr;
    }
}

TablaHash::~TablaHash() {
    for (int i = 0; i < M; ++i) {
        NodoPiloto* actual = tabla[i];
        while (actual) {
            NodoPiloto* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }
    delete[] tabla;
}

int TablaHash::funcionHash(const std::string& llave) {
    int suma = static_cast<int>(llave[0]);
    for (size_t i = 1; i < llave.length(); ++i) {
        suma += (llave[i] - '0');
    }
    return suma % M;
}

void TablaHash::insertar(const Piloto& piloto) {
    int indice = funcionHash(piloto.numero_de_id);
    NodoPiloto* nuevoNodo = new NodoPiloto(piloto);
    nuevoNodo->siguiente = tabla[indice];
    tabla[indice] = nuevoNodo;
}

void TablaHash::eliminar(const std::string& numero_de_id) {
    int indice = funcionHash(numero_de_id);
    NodoPiloto* actual = tabla[indice];
    NodoPiloto* anterior = nullptr;

    while (actual && actual->piloto.numero_de_id != numero_de_id) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual) {
        if (anterior) {
            anterior->siguiente = actual->siguiente;
        } else {
            tabla[indice] = actual->siguiente;
        }
        delete actual;
    }
}


Piloto* TablaHash::buscar(const std::string& numero_de_id) {
    int indice = funcionHash(numero_de_id);
    NodoPiloto* actual = tabla[indice];
    while (actual) {
        if (actual->piloto.numero_de_id == numero_de_id) {
            return &(actual->piloto);
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

void TablaHash::mostrar() {
    for (int i = 0; i < M; ++i) {
        std::cout << "Indice " << i << ": ";
        NodoPiloto* actual = tabla[i];
        while (actual) {
            std::cout << actual->piloto.numero_de_id << " ";
            actual = actual->siguiente;
        }
        std::cout << std::endl;
    }
}

void TablaHash::imprimirValorAscii(const std::string& numero_de_id) {
    std::cout << "Valor ASCII de '" << numero_de_id << "': ";
    for (char ch : numero_de_id) {
        std::cout << static_cast<int>(ch) << " ";
    }
    std::cout << std::endl;
}

void TablaHash::generarArchivoDOT() const {
    std::ofstream archivo("Hash.dot");
    archivo << "digraph G {\n";
    archivo << "rankdir=TB;\n";
    archivo << "node [shape=box];\n";

    for (int i = 0; i < M; ++i) {
        archivo << "indice" << i << " [label=\"indice " << i << "\"];\n";

        NodoPiloto* actual = tabla[i];
        int j = 0;
        while (actual) {
            archivo << "piloto" << i << "_" << j << " [label=\"" << actual->piloto.numero_de_id << "\"];\n";
            if (j > 0) {
                archivo << "piloto" << i << "_" << (j - 1) << " -> piloto" << i << "_" << j << ";\n";
            } else {
                archivo << "indice" << i << " -> piloto" << i << "_" << j << ";\n";
            }
            actual = actual->siguiente;
            ++j;
        }
    }

    archivo << "}\n";
    archivo.close();

    // Generar imagen PNG y abrir en visor predeterminado
    system("dot -Tpng Hash.dot -o Hash.png");
    system("start Hash.png"); // Para Windows, abrirá la imagen en el visor predeterminado
}