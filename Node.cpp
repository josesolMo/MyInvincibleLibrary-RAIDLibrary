
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
Node::Node(Image* _image) {
    image = _image;
}


///Metodos


//-


///Getters & Setters


/**
 * Getter de imagen de Node.
 * @return Imagen
 */
Image* Node::getImage() {
    return image;
}

/**
 * Setter de imagen de Node.
 * @param _imagen
 */
void Node::setImage(Image* _image) {
    image = _image;
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
