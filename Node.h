
#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_NODE_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_NODE_H

#include "Image.h"


/**
 * Header de Node
 *
 * @since 02/06/19
 */


class Node {

private:
    ///Dato que guardara el nodo
    Image* image;
    ///Referencia al nodo que esta siguiente a este
    Node* next;

public:

    ///Constructores
    Node();
    Node(Image* _image);

    ///Metodos
    //-

    ///Getters & Setters
    Image* getImage();
    void setImage(Image* _image);
    Node* getNext();
    void setNext(Node* _next);
};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_NODE_H
