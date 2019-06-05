
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
    string byte;
    Node* next;
    string imageName;
    bool parity;

public:
    ///Constructores
    Node();
    Node(string byte, string imageName);

    ///Metodos
    //-

    ///Getters & Setters
    string getByte();
    void setByte(string _byte);
    Node* getNext();
    void setNext(Node* _next);
    string getImageName();
    void setImageName(string imageName);
    bool isParity();
    void setParity(bool _parity);
};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_NODE_H
