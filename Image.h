
#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_IMAGEN_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_IMAGEN_H

#include <iostream>

using namespace std;


/**
 * Header de Image.
 *
 * @since 02/06/19
 */


class Image {

private:
    string nombre;
    string binArray;

public:
    ///Constructores
    Image(string _nombre);

    ///Metodos
    //-

    ///Getters & Setters
    string getNombre();
    void setNombre(string _nombre);

};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_IMAGEN_H
