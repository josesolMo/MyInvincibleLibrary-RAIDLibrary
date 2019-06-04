
#include "Image.h"


/**
 * Contenedor de la informacion necesaria para obtener una imagen.
 *
 * @since 03/06/19
 */


///Constructores

Image::Image(string _nombre) {
    nombre = _nombre;
}


///Metodos

//-


///Getters & Setters

/**
 * Getter del nombre de Image.
 * @return nombre
 */
string Image::getNombre() {
    return nombre;
}

/**
 * Setter del nombre de Image.
 * @param _nombre
 */
void Image::setNombre(string _nombre) {
    nombre = _nombre;
}