#include "ArbolB.h"
#include <iostream>


void ArbolB::insertar(Avion k) {
    if (raiz == nullptr) {
        raiz = new NodoB(true);
        raiz->claves.push_back(k);
    } else {
        if (raiz->claves.size() == 2 * orden - 1) {
            NodoB* s = new NodoB(false);
            s->hijos.push_back(raiz);
            dividirHijo(s, 0, raiz);
            insertarNoLleno(s, k);
            raiz = s;
        } else {
            insertarNoLleno(raiz, k);
        }
    }
}

void ArbolB::insertarNoLleno(NodoB* nodo, Avion k) {
    int i = nodo->claves.size() - 1;

    if (nodo->hoja) {
        nodo->claves.push_back(k);
        while (i >= 0 && nodo->claves[i].numero_de_registro > k.numero_de_registro) {
            nodo->claves[i + 1] = nodo->claves[i];
            i--;
        }
        nodo->claves[i + 1] = k;
    } else {
        while (i >= 0 && nodo->claves[i].numero_de_registro > k.numero_de_registro)
            i--;
        if (nodo->hijos[i + 1]->claves.size() == 2 * orden - 1) {
            dividirHijo(nodo, i + 1, nodo->hijos[i + 1]);
            if (nodo->claves[i + 1].numero_de_registro < k.numero_de_registro)
                i++;
        }
        insertarNoLleno(nodo->hijos[i + 1], k);
    }
}

void ArbolB::dividirHijo(NodoB* nodo, int i, NodoB* y) {
    NodoB* z = new NodoB(y->hoja);
    for (int j = 0; j < orden - 1; j++)
        z->claves.push_back(y->claves[j + orden]);

    if (!y->hoja) {
        for (int j = 0; j < orden; j++)
            z->hijos.push_back(y->hijos[j + orden]);
    }

    y->claves.resize(orden - 1);
    y->hijos.resize(orden);

    nodo->hijos.insert(nodo->hijos.begin() + i + 1, z);
    nodo->claves.insert(nodo->claves.begin() + i, y->claves[orden - 1]);
}

void ArbolB::recorrer() {
    if (raiz != nullptr) recorrer(raiz);
}

void ArbolB::recorrer(NodoB* nodo) {
    int i;
    for (i = 0; i < nodo->claves.size(); i++) {
        if (!nodo->hoja) recorrer(nodo->hijos[i]);
        cout << " " << nodo->claves[i].numero_de_registro;
    }
    if (!nodo->hoja) recorrer(nodo->hijos[i]);
}

NodoB* ArbolB::buscar(string numero_de_registro) {
    return (raiz == nullptr) ? nullptr : buscar(raiz, numero_de_registro);
}

NodoB* ArbolB::buscar(NodoB* nodo, string numero_de_registro) {
    int i = 0;
    while (i < nodo->claves.size() && numero_de_registro > nodo->claves[i].numero_de_registro)
        i++;
    if (i < nodo->claves.size() && nodo->claves[i].numero_de_registro == numero_de_registro)
        return nodo;
    return (nodo->hoja) ? nullptr : buscar(nodo->hijos[i], numero_de_registro);
}

void ArbolB::generarDot(NodoB* nodo, ofstream& archivo) {
    if (nodo != nullptr) {
        archivo << "  node" << nodo << " [label=\"";
        for (size_t i = 0; i < nodo->claves.size(); ++i) {
            archivo << "<f" << i << "> " << nodo->claves[i].numero_de_registro;
            if (i != nodo->claves.size() - 1) {
                archivo << " | ";
            }
        }
        archivo << "\"];\n";

        for (size_t i = 0; i <= nodo->claves.size(); ++i) {
            if (i < nodo->hijos.size() && nodo->hijos[i] != nullptr) {
                archivo << "  node" << nodo << ":f" << i << " -> node" << nodo->hijos[i] << ";\n";
                generarDot(nodo->hijos[i], archivo);
            }
        }
    }
}

void ArbolB::exportarArbolDot(ArbolB& arbol_b) {
    string nombreArchivo = "ArbolAviones.dot";
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo DOT." << endl;
        return;
    }
    archivo << "digraph ArbolB {\n";
    archivo << "  node [shape=record];\n";
    if (raiz != nullptr) {
        generarDot(raiz, archivo);
    }
    archivo << "}\n";
    archivo.close();
    cout << "Archivo DOT generado correctamente." << endl;

    string dotCommand = "dot -Tpng ArbolAviones.dot -o ArbolAviones.png";
    system(dotCommand.c_str());

    system("start ArbolAviones.png");
}

void ArbolB::eliminar(string numero_de_registro) {
    if (raiz == nullptr) {
        cout << "El arbol esta vacio, no se puede eliminar." << endl;
        return;
    }

    eliminar(raiz, numero_de_registro);
}

void ArbolB::eliminar(NodoB* nodo, string numero_de_registro) {
    int i = 0;
    while (i < nodo->claves.size() && numero_de_registro > nodo->claves[i].numero_de_registro)
        i++;

    if (i < nodo->claves.size() && nodo->claves[i].numero_de_registro == numero_de_registro) {
        if (nodo->hoja) {
            nodo->claves.erase(nodo->claves.begin() + i);
        } else {
            Avion sucesor = encontrarSucesor(nodo->hijos[i + 1]);
            eliminar(nodo->hijos[i + 1], sucesor.numero_de_registro);
            nodo->claves[i] = sucesor;
        }
    } else {
        if (nodo->hoja) {
            cout << "El avion con numero de registro " << numero_de_registro << " no existe en el arbol." << endl;
            return;
        }

        bool enUltimoHijo = (i == nodo->claves.size());
        if (nodo->hijos[i]->claves.size() < orden) {
            rellenar(nodo, i);
        }

        if (enUltimoHijo && i > nodo->claves.size()) {
            eliminar(nodo->hijos[i - 1], numero_de_registro);
        } else {
            eliminar(nodo->hijos[i], numero_de_registro);
        }
    }
}

Avion ArbolB::encontrarSucesor(NodoB* nodo) {
    if (nodo->hoja) {
        return nodo->claves[0];
    }
    return encontrarSucesor(nodo->hijos[0]);
}

void ArbolB::rellenar(NodoB* nodo, int indice) {
    if (indice != 0 && nodo->hijos[indice - 1]->claves.size() >= orden) {
        moverClaveAnterior(nodo, indice);
    } else if (indice != nodo->claves.size() && nodo->hijos[indice + 1]->claves.size() >= orden) {
        moverClaveSiguiente(nodo, indice);
    } else {
        fusionar(nodo, indice);
    }
}

void ArbolB::moverClaveAnterior(NodoB* nodo, int indice) {
    NodoB* hijo = nodo->hijos[indice];
    NodoB* hermano = nodo->hijos[indice - 1];

    hijo->claves.insert(hijo->claves.begin(), nodo->claves[indice - 1]);

    if (!hijo->hoja) {
        hijo->hijos.insert(hijo->hijos.begin(), hermano->hijos[hermano->claves.size()]);
        hermano->hijos.erase(hermano->hijos.begin() + hermano->hijos.size());
    }

    nodo->claves[indice - 1] = hermano->claves[hermano->claves.size()];
}

void ArbolB::moverClaveSiguiente(NodoB* nodo, int indice) {
    NodoB* hijo = nodo->hijos[indice];
    NodoB* hermano = nodo->hijos[indice + 1];

    hijo->claves.push_back(nodo->claves[indice]);

    if (!hijo->hoja) {
        hijo->hijos.push_back(hermano->hijos[0]);
        hermano->hijos.erase(hermano->hijos.begin());
    }

    nodo->claves[indice] = hermano->claves[0];
}

void ArbolB::fusionar(NodoB* nodo, int indice) {
    NodoB* hijo = nodo->hijos[indice];
    NodoB* hermano = nodo->hijos[indice + 1];

    hijo->claves.push_back(nodo->claves[indice]);

    for (int i = 0; i < hermano->claves.size(); ++i) {
        hijo->claves.push_back(hermano->claves[i]);
    }

    if (!hijo->hoja) {
        for (int i = 0; i <= hermano->hijos.size(); ++i) {
            hijo->hijos.push_back(hermano->hijos[i]);
        }
    }

    nodo->hijos.erase(nodo->hijos.begin() + indice + 1);
    nodo->claves.erase(nodo->claves.begin() + indice);
}

Avion* ArbolB::obtenerAvionPorNumeroRegistro(const std::string& numeroRegistro) {
    return buscarAvionPorNumero(raiz, numeroRegistro);
}

Avion* ArbolB::buscarAvionPorNumero(NodoB* nodo, const std::string& numeroRegistro) {
    if (nodo == nullptr) {
        return nullptr;
    }

    int i = 0;
    while (i < nodo->claves.size() && numeroRegistro > nodo->claves[i].numero_de_registro) {
        i++;
    }

    if (i < nodo->claves.size() && nodo->claves[i].numero_de_registro == numeroRegistro) {
        return &(nodo->claves[i]);
    }

    return (nodo->hoja) ? nullptr : buscarAvionPorNumero(nodo->hijos[i], numeroRegistro);
}