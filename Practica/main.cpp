#include <iostream>
#include <fstream>
#include <json.hpp>
#include "cola.h"
#include "listadoble.h"
#include "pila.h"
#include "ListaPasajeros.h"

using namespace std;
using json = nlohmann::json;


void cargarAviones(Listadoble& listaDisponibles, Listadoble& listaMantenimiento) {
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
        avion.fabricante = avionJson["fabricante"];
        avion.ano_fabricacion = avionJson["ano_fabricacion"];
        avion.capacidad = avionJson["capacidad"];
        avion.peso_max_despegue = avionJson["peso_max_despegue"];
        avion.aerolinea = avionJson["aerolinea"];
        avion.estado = avionJson["estado"];

        if (avion.estado == "Disponible") {
            listaDisponibles.insertarAlFinal(avion);
        } else if (avion.estado == "Mantenimiento") {
            listaMantenimiento.insertarAlFinal(avion);
        } else {
            cerr << "Error: Estado desconocido para el avion." << endl;
        }

        //cout << "Avion cargado: " << avion.modelo << ", Estado: " << avion.estado << endl;
    }

}

void cargarPasajeros(Cola& colaPasajeros) {
    string nombreArchivo;
    cout << "Ingrese el nombre del archivo JSON: ";
    cin >> nombreArchivo;

    ifstream inputFile(nombreArchivo);
    if (!inputFile.is_open()) {
        cerr << "Error abriendo el archivo de pasajeros." << endl;
        return;
    }

    json pasajerosJson;
    inputFile >> pasajerosJson;

    for (const auto& pasajeroJson : pasajerosJson) {
        Pasajero pasajero;
        pasajero.nombre = pasajeroJson["nombre"];
        pasajero.nacionalidad = pasajeroJson["nacionalidad"];
        pasajero.numero_de_pasaporte = pasajeroJson["numero_de_pasaporte"];
        pasajero.vuelo = pasajeroJson["vuelo"];
        pasajero.asiento = pasajeroJson["asiento"];
        pasajero.destino = pasajeroJson["destino"];
        pasajero.origen = pasajeroJson["origen"];
        pasajero.equipaje_facturado = pasajeroJson["equipaje_facturado"];

        colaPasajeros.encolar(pasajero);
/*
        cout << "-----------------------------" << endl;
        cout << "Nombre: " << pasajero.nombre << endl;
        cout << "Nacionalidad: " << pasajero.nacionalidad << endl;
        cout << "Numero de Pasaporte: " << pasajero.numero_de_pasaporte << endl;
        cout << "Vuelo: " << pasajero.vuelo << endl;
        cout << "Asiento: " << pasajero.asiento << endl;
        cout << "Destino: " << pasajero.destino << endl;
        cout << "Origen: " << pasajero.origen << endl;
        cout << "Equipaje Facturado: " << pasajero.equipaje_facturado << endl;
        cout << "-----------------------------" << endl;
*/
    }
}

void consultarPasajero(Cola& colaPasajeros, ListaPasajeros& listaPasajeros) {
    string numeroPasaporte;
    cout << "Ingrese el numero de pasaporte del pasajero: ";
    cin >> numeroPasaporte;

    Cola colaAuxiliar;
    bool encontrado = false;

    while (!colaPasajeros.estaVacia()) {
        Pasajero pasajero = colaPasajeros.obtenerFrente();
        colaPasajeros.desencolar();
        colaAuxiliar.encolar(pasajero);

        if (pasajero.numero_de_pasaporte == numeroPasaporte) {
            encontrado = true;
            cout << "-----------------------------" << endl;
            cout << "Pasajero encontrado:" << endl;
            pasajero.mostrar();
            cout << "-----------------------------" << endl;
        }
    }

    while (!colaAuxiliar.estaVacia()) {
        colaPasajeros.encolar(colaAuxiliar.obtenerFrente());
        colaAuxiliar.desencolar();
    }

    if (listaPasajeros.contienePasajero(numeroPasaporte)) {
        encontrado = true;
        cout << "-----------------------------" << endl;
        cout << "Pasajero encontrado:" << endl;
        listaPasajeros.mostrarPasajero(numeroPasaporte);
        cout << "-----------------------------" << endl;
    }

    if (!encontrado) {
        cout << "No se encontro ningun pasajero con el numero de pasaporte proporcionado." << endl;
    }
}



void cargarMovimientos(Listadoble& listaDisponibles, Listadoble& listaMantenimiento, Cola& colaPasajeros, Pila& pilaEquipajes, ListaPasajeros& listaPasajeros) {
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
        linea.erase(0, linea.find_first_not_of(" \t\n\r"));
        linea.erase(linea.find_last_not_of(" \t\n\r") + 1);
        if (linea == "IngresoEquipajes;") {
            cout << "Comando de ingreso de equipajes detectado." << endl;
            while (!colaPasajeros.estaVacia()) {
                Pasajero pasajero = colaPasajeros.obtenerFrente();
                colaPasajeros.desencolar();
                if (pasajero.equipaje_facturado > 0) {
                    pilaEquipajes.push(pasajero.equipaje_facturado);
                }
                listaPasajeros.insertarOrdenado(pasajero);
            }
            // Mostrar pila de equipajes
            //cout << "Pila de equipajes (de abajo hacia arriba): ";
            //pilaEquipajes.mostrar();

            // Mostrar lista de pasajeros
            //cout << "Lista de pasajeros:" << endl;
            //listaPasajeros.mostrar();
        } else if (linea.find("MantenimientoAviones,Ingreso,") == 0) {
            cout << "Comando de ingreso de aviones detectado." << endl;
            size_t inicio = 29;
            size_t fin = linea.find(";", inicio);
            string numeroRegistro = linea.substr(inicio, fin - inicio);

            //cout << "Numero de registro a procesar: " << numeroRegistro << endl;

            if (listaDisponibles.contieneAvion(numeroRegistro)) {
                cout << "El avion con numero de registro " << numeroRegistro << " esta disponible y sera movido a mantenimiento." << endl;
                listaDisponibles.moverAvion(listaMantenimiento, numeroRegistro);
            } else if (listaMantenimiento.contieneAvion(numeroRegistro)) {
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

            if (listaMantenimiento.contieneAvion(numeroRegistro)) {
                listaMantenimiento.moverAvion(listaDisponibles, numeroRegistro);
            } else {
                cout << "El avion con numero de registro " << numeroRegistro << " no se encontro en la lista de mantenimiento." << endl;
            }

        } else {
            cout << "Comando no valido: " << linea << endl;
        }
    }

    inputFile.close();
}


/*void cargarMovimientos2(Listadoble& listaDisponibles, Listadoble& listaMantenimiento, Cola& colaPasajeros, Pila& pilaEquipajes, ListaPasajeros& listaPasajeros) {
    cin.ignore();
    cout << "Ingrese los comandos de movimiento, uno por linea (para terminar, ingrese 'fin'): " << endl;

    string linea;
    while (getline(cin, linea)) {
        if (linea == "fin") {
            break;
        }

        if (linea == "IngresoEquipajes;") {
            cout << "Comando de ingreso de equipajes detectado." << endl;
            while (!colaPasajeros.estaVacia()) {
                Pasajero pasajero = colaPasajeros.obtenerFrente();
                colaPasajeros.desencolar();
                if (pasajero.equipaje_facturado > 0) {
                    pilaEquipajes.push(pasajero.equipaje_facturado);
                }
                listaPasajeros.insertarOrdenado(pasajero);
            }
        } else if (linea.find("MantenimientoAviones,Ingreso,") == 0) {
            string numeroRegistro = linea.substr(29);
            listaDisponibles.moverAvion(listaMantenimiento, numeroRegistro);
        } else if (linea.find("MantenimientoAviones,Salida,") == 0) {
            string numeroRegistro = linea.substr(28);
            listaMantenimiento.moverAvion(listaDisponibles, numeroRegistro);
        } else {
            cout << "Comando no valido: " << linea << endl;
        }
    }
}*/

int main() {
    int opcion;
    Cola colaPasajeros;
    Listadoble listaDisponibles("Disponibles");
    Listadoble listaMantenimiento("Mantenimiento");
    Pila pilaEquipajes;
    ListaPasajeros listaPasajeros;

    do {
        cout << "---------- MENU ----------" << endl;
        cout << "|1. Carga de aviones     |" << endl;
        cout << "|2. Carga de pasajeros   |" << endl;
        cout << "|3. Carga de movimientos |" << endl;
        cout << "|4. Consultar pasajero   |" << endl;
        cout << "|5. Visualizar reportes  |" << endl;
        cout << "|6. Salir                |" << endl;
        cout << "--------------------------" << endl;
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                cargarAviones(listaDisponibles, listaMantenimiento);
                //cout << "Lista de disponibles:" << endl;
                //listaDisponibles.mostrar();
                //cout << "Lista de mantenimiento:" << endl;
                //listaMantenimiento.mostrar();
            break;
            case 2:
                cargarPasajeros(colaPasajeros);
            break;
            case 3:
                cargarMovimientos(listaDisponibles, listaMantenimiento, colaPasajeros, pilaEquipajes, listaPasajeros);
                //cout << "Lista de disponibles:" << endl;
                //listaDisponibles.mostrar();
                //cout << "Lista de mantenimiento:" << endl;
                //listaMantenimiento.mostrar();
            break;
            case 4:
                consultarPasajero(colaPasajeros, listaPasajeros);
            break;
            case 5:
                listaDisponibles.graficar("AvionesDisponibles");
                listaMantenimiento.graficar("AvionesMantenimiento");
                colaPasajeros.graficar();
                listaPasajeros.graficar();
                pilaEquipajes.graficar();
                break;
            case 6:
                cout << "Saliendo del programa..." << endl;
            break;
            default:
                cout << "Opcion invalida. Por favor, elija una opcion valida." << endl;
        }
    } while(opcion != 6);

    return 0;
}
