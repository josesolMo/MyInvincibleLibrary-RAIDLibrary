
#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_LIST_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_LIST_H


#include <iostream>
#include "Node.h"

using namespace std;


/**
 * Header de List.
 *
 * @since 02/06/19
 */


class List {

private:
    Node* head;
    int len;

public:
    ///Constructores
    List();

    ///Metodos
    void newNode(string byte, string imageName);
    void deleteNode(string _image);
    int getIndex(string _nombre);
    Node* getNode(int _index);
    Node* getNode(string _nombre);
    void printList();

    ///Getters & Setters
    Node* getHead();
    void setHead(Node* _head);
    int getLen();
    void setLen(int _len);

};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_LIST_H
