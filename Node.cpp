
#include "Node.h"


/**
 * Representacion de un Nodo.
 *
 * @since 02/06/19
 */


///Constructores


/**
 * Costructor de Node.
 * @param _imagen
 */
Node::Node(Imagen* _imagen) {
    imagen = _imagen;
}


///Metodos


//-


///Getters & Setters


/**
 * Getter de imagen de Node.
 * @return Imagen
 */
Imagen* Node::getImagen() {
    return imagen;
}

/**
 * Setter de imagen de Node.
 * @param _imagen
 */
void Node::setImagen(Imagen* _imagen) {
    imagen = _imagen;
}

/**
 * Getter del next de Node.
 * @return Node
 */
Node* Node::getNext() {
    return next;
}

/**
 * Getter del next de Node.
 * @param _next
 */
void Node::setNext(Node* _next) {
    next = _next;
}
