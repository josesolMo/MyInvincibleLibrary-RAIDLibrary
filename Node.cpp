
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
Node::Node(string _byte, string _imageName) {
    byte = _byte;
    next = nullptr;
    imageName = _imageName;
    pairity = false;
}


///Metodos


//-


///Getters & Setters


/**
 * Getter de byte de Node.
 * @return string
 */
string Node::getByte() {
    return byte;
}

/**
 * Setter de byte de Node.
 * @param _byte
 */
void Node::setByte(string _byte) {
    byte = _byte;
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

/**
 * Getter de imageName de Node.
 * @return nombre
 */
string Node::getImageName() {
    return imageName;
}

/**
 * Setter de xxxxxx de Node.
 * @param xxxxx
 */
void Node::setImageName(string _imageName) {
    imageName = _imageName;
}

bool Node::isPairity() {
    return pairity;
}

/**
 * Setter de xxxxxx de Node.
 * @param xxxxx
 */
void Node::setPairity(bool _pairity) {
    pairity = _pairity;
}
