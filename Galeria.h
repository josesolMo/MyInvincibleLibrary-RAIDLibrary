//
// Created by andreyzartty on 03/06/19.
//

#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_GALERIA_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_GALERIA_H

#import "List.h"
#include <iostream>

using namespace std;


class Galeria {

private:
    List* Imagenes;
    string* Nombre;
    string* Cliente;
public:
    string *getNombre();
    void setNombre(string *nombre);
    string *getCliente();
    void setCliente(string *cliente);
    void insertarImagen();


};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_GALERIA_H
