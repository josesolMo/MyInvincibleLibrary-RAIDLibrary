
#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_GALERIA_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_GALERIA_H

#include "List.h"
#include <iostream>

using namespace std;


/**
 * Header de Galeria.
 *
 * @since 03/06/19
 */


class Galeria {

private:
    List* imagenes;
    string nombre;
    string cliente;

public:
    ///Constructor
    Galeria(string _nombre, string _cliente);

    ///Metodos
    void insertarImagen();

    ///Getters & Setters
    string getNombre();
    void setNombre(string _nombre);
    string getCliente();
    void setCliente(string _cliente);

};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_GALERIA_H
