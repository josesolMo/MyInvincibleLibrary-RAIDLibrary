
#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_NODE_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_NODE_H

#include "Imagen.h"


/**
 * Header de Node
 *
 * @since 02/06/19
 */


class Node {

private:
    ///Dato que guardara el nodo
    Imagen* imagen;
    ///Referencia al nodo que esta siguiente a este
    Node* next;

public:

    ///Constructores
    Node();
    Node(Imagen* _imagen);

    ///Metodos
    //-

    ///Getters & Setters
    Imagen* getImagen();
    void setImagen(Imagen* _imagen);
    Node* getNext();
    void setNext(Node* _next);
};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_NODE_H
