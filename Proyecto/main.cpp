#include <iostream>
#include <fstream>
#include <json.hpp>
#include "Avion.h"
#include "Piloto.h"
#include "ArbolB.h"
#include "ListaCircularDoble.h"
#include "ArbolBBusqueda.h"
#include "TablaHash.h"
#include "Grafo.h"

using namespace std;
using json = nlohmann::json;


ArbolBinarioBusqueda arbolPilotos;
ArbolB arbolDisponibles(3);
ListaCircularDoble listaMantenimiento;
TablaHash tablaHashPilotos(18);
Grafo grafo;

void cargarAviones() {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo JSON: ";
    cin >> nombreArchivo;

    ifstream inputFile(nombreArchivo);
    if (!inputFile.is_open()) {
        cerr << "Error abriendo el archivo de aviones." << endl;
        return;
    }

    json avionesJson;
    inputFile >> avionesJson;

    for (const auto& avionJson : avionesJson) {
        Avion avion;
        avion.vuelo = avionJson["vuelo"];
        avion.numero_de_registro = avionJson["numero_de_registro"];
        avion.modelo = avionJson["modelo"];
        avion.capacidad = avionJson["capacidad"];
        avion.aerolinea = avionJson["aerolinea"];
        avion.ciudad_destino = avionJson["ciudad_destino"];
        avion.estado = avionJson["estado"];

        if (avion.estado == "Disponible") {
            arbolDisponibles.insertar(avion);
        } else if (avion.estado == "Mantenimiento") {
            listaMantenimiento.insertar(avion);
        }
    }

    cout << "Aviones cargados correctamente." << endl;
    inputFile.close();

}

void cargarPilotos() {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo JSON: ";
    cin >> nombreArchivo;

    ifstream inputFile(nombreArchivo);
    if (!inputFile.is_open()) {
        cerr << "Error abriendo el archivo de pilotos." << endl;
        return;
    }

    json pilotosJson;
    inputFile >> pilotosJson;

    for (const auto& pilotoJson : pilotosJson) {
        Piloto piloto;
        piloto.nombre = pilotoJson["nombre"];
        piloto.nacionalidad = pilotoJson["nacionalidad"];
        piloto.numero_de_id = pilotoJson["numero_de_id"];
        piloto.vuelo = pilotoJson["vuelo"];
        piloto.horas_de_vuelo = pilotoJson["horas_de_vuelo"];
        piloto.tipo_de_licencia = pilotoJson["tipo_de_licencia"];

        arbolPilotos.insertar(piloto);
        tablaHashPilotos.insertar(piloto);
        //tablaHashPilotos.imprimirValorAscii(piloto.numero_de_id);
    }

    cout << "Pilotos cargados correctamente." << endl;
    inputFile.close();

}

void cargarRutas() {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo de rutas: ";
    cin >> nombreArchivo;

    grafo.cargarDesdeArchivo(nombreArchivo);
    cout << "Rutas cargadas correctamente." << endl;

}

void cargarMovimientos() {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo de movimientos: ";
    cin >> nombreArchivo;

    ifstream inputFile(nombreArchivo);
    if (!inputFile.is_open()) {
        cerr << "Error abriendo el archivo de movimientos." << endl;
        return;
    }

    string linea;
    while (getline(inputFile, linea)) {
        if (linea.find("DarDeBaja(") != string::npos) {
            size_t inicio = linea.find('(') + 1;
            size_t fin = linea.find(')');
            string id = linea.substr(inicio, fin - inicio);
            //cout << "Eliminando piloto con ID: " << id << endl; // Depuración
            arbolPilotos.eliminar(id);
            tablaHashPilotos.eliminar(id);
        } else if (linea.find("MantenimientoAviones,Ingreso,") == 0) {
            cout << "Comando de ingreso de aviones detectado." << endl;
            size_t inicio = 29;
            size_t fin = linea.find(";", inicio);
            string numeroRegistro = linea.substr(inicio, fin - inicio);

            //cout << "Numero de registro a procesar: " << numeroRegistro << endl;
            Avion* avionAMoverPtr = arbolDisponibles.obtenerAvionPorNumeroRegistro(numeroRegistro);

            if (arbolDisponibles.buscar(numeroRegistro)) {
                Avion avionAMover = *avionAMoverPtr;
                //cout << "El avion con numero de registro " << numeroRegistro << " esta disponible y sera movido a mantenimiento." << endl;
                arbolDisponibles.eliminar(numeroRegistro);
                listaMantenimiento.insertar(avionAMover);
            } else if (listaMantenimiento.buscar(numeroRegistro)) {
                cout << "El avion con numero de registro " << numeroRegistro << " ya esta en mantenimiento." << endl;
            } else {
                cout << "El avion con numero de registro " << numeroRegistro << " no se encontro en la lista de disponibles." << endl;
            }
        } else if (linea.find("MantenimientoAviones,Salida,") == 0) {
            cout << "Comando de salida de aviones detectado." << endl;
            size_t inicio = 28;
            size_t fin = linea.find(";", inicio);
            string numeroRegistro = linea.substr(inicio, fin - inicio);

            //cout << "Numero de registro a procesar: " << numeroRegistro << endl;

            if (listaMantenimiento.buscar(numeroRegistro)) {
                Avion avionAMover;
                if (listaMantenimiento.obtenerAvionPorNumeroRegistro(numeroRegistro, avionAMover)) {
                    listaMantenimiento.eliminar(numeroRegistro);
                    arbolDisponibles.insertar(avionAMover);
                    //cout << "El avion con numero de registro " << numeroRegistro << " movido de mantenimiento a disponibles." << endl;
                } else {
                    cout << "Error: No se pudo obtener el avion " << numeroRegistro << " de la lista de mantenimiento." << endl;
                }
            } else {
                cout << "El avion con numero de registro " << numeroRegistro << " no se encontro en la lista de mantenimiento." << endl;
            }
        } else {
            //cout << "Comando no valido: " << linea << endl;
        }
    }

    inputFile.close(); // Cerrar el archivo fuera del bucle while
    cout << "Movimientos cargados y procesados correctamente." << endl;

}

void recomendarRuta() {
    string origen, destino;
    cout << "Ingrese la ciudad origen: ";
    cin >> origen;
    cout << "Ingrese la ciudad destino: ";
    cin >> destino;

    grafo.dijkstra(origen, destino);

}



void mostrarAvionesDisponibles() {
    cout << "Aviones Disponibles en el Arbol B:" << endl;
    arbolDisponibles.recorrer();
    cout << endl;

}

void mostrarAvionesMantenimiento() {
    cout << "Aviones en Mantenimiento en la Lista Circular Doble:" << endl;
    listaMantenimiento.mostrar();

}

void mostrarTablaHash() {
    cout << "Tabla Hash de Pilotos:" << endl;
    tablaHashPilotos.mostrar();

}

void mostrarRutas() {
    grafo.mostrarRutas();

}



int ordenArbol() {
    int opcion;

    do {
        cout << "-------SELECCIONE RECORRIDO-------" << endl;
        cout << "|1. Preorden                     |" << endl;
        cout << "|2. Inorder                      |" << endl;
        cout << "|3. Postorden                    |" << endl;
        cout << "|4. Regresar                     |" << endl;
        cout << "----------------------------------" << endl;
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                cout << "PREORDEN" << endl;
                arbolPilotos.recorridoPreorden();
            break;
            case 2:
                cout << "INORDER" << endl;
                arbolPilotos.recorridoInorden();
            break;
            case 3:
                cout << "POSTORDEN" << endl;
                arbolPilotos.recorridoPostorden();
            break;
            case 4:
                cout << "Regresando al menu principal..." << endl;
            break;
            default:
                cout << "Opcion invalida. Por favor, elija una opcion valida." << endl;
        }
    } while(opcion != 4);

    return 0;

}

int main() {
    int opcion;

    do {
        cout << "-------------- MENU --------------" << endl;
        cout << "|1. Carga de aviones             |" << endl;
        cout << "|2. Carga de pilotos             |" << endl;
        cout << "|3. Carga de rutas               |" << endl;
        cout << "|4. Carga de movimientos         |" << endl;
        cout << "|5. Consulta de horas de vuelo   |" << endl;
        cout << "|6. Recomendar ruta              |" << endl;
        cout << "|7. Visualizar reportes          |" << endl;
        cout << "|8. Salir                        |" << endl;
        cout << "----------------------------------" << endl;
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                cargarAviones();
                //mostrarAvionesDisponibles();
                //mostrarAvionesMantenimiento();
            break;
            case 2:
                cargarPilotos();
                //mostrarTablaHash();
            break;
            case 3:
                cargarRutas();
                //mostrarRutas();
            break;
            case 4:
                cargarMovimientos();
            break;
            case 5:
                ordenArbol();
            break;
            case 6:
                recomendarRuta();
            break;
            case 7:
                arbolPilotos.graficarBST(arbolPilotos);
                grafo.generarArchivoDOT();
                tablaHashPilotos.generarArchivoDOT();
                listaMantenimiento.generarArchivoDOT("ListaAviones");
                arbolDisponibles.exportarArbolDot(arbolDisponibles);
            break;
            case 8:
                cout << "Saliendo del programa..." << endl;
            break;
            default:
                cout << "Opcion invalida. Por favor, elija una opcion valida." << endl;
        }
    } while(opcion != 8);

    return 0;

}