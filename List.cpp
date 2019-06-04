
#include "List.h"


/**
 * Representacion de una Lista.
 *
 * @since 02/06/19
 */


///Constructores


/**
 * Constructor de List.
 */
List::List() {
    head = nullptr;
    len = 0;
}


///Metodos


/**
 * Crea un nuevo Node con una Ficha para ingresarlo en List.
 * @param data - numero para la Ficha
 */
void List::newNode(Image* _image){

    Node* nNode = new Node(_image);

    if (head == nullptr) {
        head = nNode;
    } else {
        Node* temp = head;
        head = nNode;
        nNode->setNext(temp);
    }

    len+=1;

}

/**
 * Elimina un Node de List.
 * @param data
 */
void List::deleteNode(Image* _image){
    Node* delNode = nullptr;
    Node* temp = head;
    Node* aux = head;
    while (aux != nullptr && aux->getImage() != _image) {
        temp = aux;
        aux = aux->getNext();
    }
    if (aux == nullptr) {
        cout << _image << " no esta en la lista\n" << endl;
    } else {
        delNode = aux;
        if (head == delNode) {
            setHead(head->getNext());
        } else {
            temp->setNext(aux->getNext());
        }
    }

    if (delNode != nullptr) {
        delNode->setNext(nullptr);
    } else {
        cout << "delNode == nullptr" << endl;
    }

    len-=1;

}

/**
 * Retorna un Node por su index.
 * @param posicion
 * @return Node
 */
Node *List::getNode(int _index) {
    Node* temp = nullptr;
    if (_index <= len) {
        int i = 1;
        temp = head;
        while (i != _index) {
            temp = temp->getNext();
            i++;
        }
    }
    return temp;
}

/**
 * Retorna un node por su letra.
 * @param letra
 * @return Node
 */
Node* List::getNode(string _nombre) {
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->getImage()->getNombre() == _nombre) {
            return temp;
        }
        temp = temp->getNext();
    }
    return nullptr;
}

/**
 * Imprime los nodos en List.
 */
void List::printList() {
    cout << "length: " << len << "\n[ ";
    Node* temp = head;
    while (temp != nullptr) {
        ///Para que no imprima el ultimo con una coma
        if (temp->getNext() == nullptr) {
            cout << temp->getImage()->getNombre() ;
            break;
        }
        cout << temp->getImage()->getNombre() << ", ";
        temp = temp->getNext();
    }

    cout << " ]" << endl;
}


///Getters & Setters


/**
 * Getter del head de List.
 * @returns head - primer Nodo de List
 */
Node* List::getHead() {
    return head;
}

/**
 * Setter del head de List.
 * @param _head - Node
 */
void List::setHead(Node* _head) {
    head = _head;
}

/**
 * Getter del length de List.
 * @returns len - Cantidad de Nodes en List
 */
int List::getLen() {
    return len;
}

/**
 * Setter del length de List.
 * @param _len - cantidad
 */
void List::setLen(int _len) {
    len = _len;
}
