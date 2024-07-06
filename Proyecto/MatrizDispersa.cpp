#include "MatrizDispersa.h"


MatrizDispersa::MatrizDispersa() : filaHeader(nullptr), columnaHeader(nullptr) {}

MatrizDispersa::~MatrizDispersa() {
    // Liberar memoria de los nodos
    NodoMatriz* filaActual = filaHeader;
    while (filaActual) {
        NodoMatriz* filaSiguiente = filaActual->siguienteFila;
        NodoMatriz* columnaActual = filaActual;
        while (columnaActual) {
            NodoMatriz* columnaSiguiente = columnaActual->siguienteColumna;
            delete columnaActual;
            columnaActual = columnaSiguiente;
        }
        filaActual = filaSiguiente;
    }
}

NodoMatriz* MatrizDispersa::obtenerFilaHeader(const std::string& vuelo) {
    NodoMatriz* actual = filaHeader;
    while (actual) {
        if (actual->vuelo == vuelo) {
            return actual;
        }
        actual = actual->siguienteFila;
    }
    return nullptr;
}

NodoMatriz* MatrizDispersa::obtenerColumnaHeader(const std::string& ciudadDestino) {
    NodoMatriz* actual = columnaHeader;
    while (actual) {
        if (actual->ciudadDestino == ciudadDestino) {
            return actual;
        }
        actual = actual->siguienteColumna;
    }
    return nullptr;
}

void MatrizDispersa::insertar(const std::string& pilotoID, const std::string& vuelo, const std::string& ciudadDestino) {
    NodoMatriz* nuevo = new NodoMatriz(pilotoID, vuelo, ciudadDestino);

    // Insertar en la fila
    NodoMatriz* filaActual = obtenerFilaHeader(vuelo);
    if (!filaActual) {
        // Nueva fila
        filaActual = new NodoMatriz("", vuelo, "");
        filaActual->siguienteFila = filaHeader;
        filaHeader = filaActual;
    }

    // Insertar en la columna
    NodoMatriz* columnaActual = obtenerColumnaHeader(ciudadDestino);
    if (!columnaActual) {
        // Nueva columna
        columnaActual = new NodoMatriz("", "", ciudadDestino);
        columnaActual->siguienteColumna = columnaHeader;
        columnaHeader = columnaActual;
    }

    // Insertar nodo piloto en la fila
    NodoMatriz* tempFila = filaActual;
    while (tempFila->siguienteColumna && tempFila->siguienteColumna->ciudadDestino != ciudadDestino) {
        tempFila = tempFila->siguienteColumna;
    }
    nuevo->siguienteColumna = tempFila->siguienteColumna;
    tempFila->siguienteColumna = nuevo;

    // Insertar nodo piloto en la columna
    NodoMatriz* tempColumna = columnaActual;
    while (tempColumna->siguienteFila && tempColumna->siguienteFila->vuelo != vuelo) {
        tempColumna = tempColumna->siguienteFila;
    }
    nuevo->siguienteFila = tempColumna->siguienteFila;
    tempColumna->siguienteFila = nuevo;

    // Depuración
    std::cout << "Nodo insertado: Vuelo=" << vuelo << ", PilotoID=" << pilotoID << ", Ciudad=" << ciudadDestino << std::endl;
}

void MatrizDispersa::eliminarPiloto(const std::string& pilotoID) {
    NodoMatriz* filaActual = filaHeader;
    NodoMatriz* filaAnterior = nullptr;

    while (filaActual != nullptr) {
        NodoMatriz* nodoActual = filaActual->siguienteColumna;
        NodoMatriz* nodoAnterior = filaActual;

        while (nodoActual != nullptr) {
            if (nodoActual->pilotoID == pilotoID) {
                nodoAnterior->siguienteColumna = nodoActual->siguienteColumna;
                std::string vuelo = nodoActual->vuelo;
                std::string ciudadDestino = nodoActual->ciudadDestino;
                delete nodoActual;

                // Verificar si la fila queda vacía
                if (filaActual->siguienteColumna == nullptr && filaAnterior != nullptr) {
                    filaAnterior->siguienteFila = filaActual->siguienteFila;
                    delete filaActual;
                }

                // Verificar si la columna queda vacía
                NodoMatriz* columnaActual = columnaHeader;
                NodoMatriz* columnaAnterior = nullptr;
                while (columnaActual != nullptr) {
                    if (columnaActual->ciudadDestino == ciudadDestino) {
                        NodoMatriz* nodoColumnaActual = columnaActual->siguienteFila;
                        NodoMatriz* nodoColumnaAnterior = columnaActual;
                        bool columnaVacia = true;
                        while (nodoColumnaActual != nullptr) {
                            if (nodoColumnaActual->ciudadDestino == ciudadDestino) {
                                columnaVacia = false;
                                break;
                            }
                            nodoColumnaAnterior = nodoColumnaActual;
                            nodoColumnaActual = nodoColumnaActual->siguienteFila;
                        }
                        if (columnaVacia) {
                            if (columnaAnterior != nullptr) {
                                columnaAnterior->siguienteColumna = columnaActual->siguienteColumna;
                            } else {
                                columnaHeader = columnaActual->siguienteColumna;
                            }
                            delete columnaActual;
                        }
                        break;
                    }
                    columnaAnterior = columnaActual;
                    columnaActual = columnaActual->siguienteColumna;
                }

                return; // Salir después de eliminar el nodo del piloto y hacer las verificaciones necesarias
            }
            nodoAnterior = nodoActual;
            nodoActual = nodoActual->siguienteColumna;
        }
        filaAnterior = filaActual;
        filaActual = filaActual->siguienteFila;
    }
}

void MatrizDispersa::mostrar() {
    NodoMatriz* filaActual = filaHeader;
    while (filaActual) {
        NodoMatriz* nodoActual = filaActual->siguienteColumna; // Saltamos el header de la fila
        while (nodoActual) {
            std::cout << "Vuelo: " << nodoActual->vuelo << ", Piloto ID: " << nodoActual->pilotoID << ", Ciudad: " << nodoActual->ciudadDestino << std::endl;
            nodoActual = nodoActual->siguienteColumna;
        }
        filaActual = filaActual->siguienteFila;
    }
}

void MatrizDispersa::graficarMatriz() {
    std::ofstream file;
    std::string filename = "MatrizDispersa.dot";
    file.open(filename);
    file << "digraph G {" << std::endl;
    file << "node [shape=box];" << std::endl;
    file << "rankdir=LR;" << std::endl; // Dirección de izquierda a derecha

    // Crear las cabeceras de las columnas (ciudades destino)
    NodoMatriz* tmpCol = columnaHeader;
    file << "{ rank=same; \"Matriz\"";
    while (tmpCol != nullptr) {
        file << " -> C" << tmpCol->ciudadDestino;
        tmpCol = tmpCol->siguienteColumna;
    }
    file << " [style=invis]; }" << std::endl;

    tmpCol = columnaHeader;
    while (tmpCol != nullptr) {
        file << "C" << tmpCol->ciudadDestino << " [label=\"" << tmpCol->ciudadDestino << "\"];" << std::endl;
        tmpCol = tmpCol->siguienteColumna;
    }

    // Crear las cabeceras de las filas (vuelos)
    NodoMatriz* tmpFil = filaHeader;
    while (tmpFil != nullptr) {
        file << "F" << tmpFil->vuelo << " [label=\"" << tmpFil->vuelo << "\"];" << std::endl;
        tmpFil = tmpFil->siguienteFila;
    }

    // Crear nodos para las intersecciones (pilotos) y conectarlos
    NodoMatriz* fila = filaHeader;
    while (fila != nullptr) {
        NodoMatriz* columna = fila->siguienteColumna;
        while (columna != nullptr) {
            if (!columna->pilotoID.empty()) {
                file << "P" << fila->vuelo << "_" << columna->ciudadDestino << " [label=\"" << columna->pilotoID << "\"];" << std::endl;
                file << "F" << fila->vuelo << " -> P" << fila->vuelo << "_" << columna->ciudadDestino << " [dir=none];" << std::endl;
                file << "P" << fila->vuelo << "_" << columna->ciudadDestino << " -> C" << columna->ciudadDestino << " [dir=none];" << std::endl;
            }
            columna = columna->siguienteColumna;
        }
        fila = fila->siguienteFila;
    }

    // Conectar las cabeceras de las filas
    tmpFil = filaHeader;
    while (tmpFil != nullptr) {
        if (tmpFil->siguienteFila != nullptr) {
            file << "F" << tmpFil->vuelo << " -> F" << tmpFil->siguienteFila->vuelo << " [style=invis];" << std::endl;
        }
        tmpFil = tmpFil->siguienteFila;
    }

    // Conectar las cabeceras de las columnas
    tmpCol = columnaHeader;
    while (tmpCol != nullptr) {
        if (tmpCol->siguienteColumna != nullptr) {
            file << "C" << tmpCol->ciudadDestino << " -> C" << tmpCol->siguienteColumna->ciudadDestino << " [style=invis];" << std::endl;
        }
        tmpCol = tmpCol->siguienteColumna;
    }

    file << "}" << std::endl;
    file.close();

    std::string command = "dot -Tpng " + filename + " -o MatrizDispersa.png";
    system(command.c_str());
    system("start MatrizDispersa.png");
}
