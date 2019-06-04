
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
void List::newNode(string byte, string imageName){

    Node* nNode = new Node(byte, imageName);

    if (head == nullptr) {
        head = nNode;
    } else {
        Node* temp = head;
        head = nNode;
        nNode->setNext(temp);
    }

    len+=1;
    delete nNode;

}

/**
 * Elimina un Node de List.
 * @param data
 */
void List::deleteNode(string _image){
    Node* delNode = nullptr;
    Node* temp = head;
    Node* aux = head;
    while (aux != nullptr && aux->getImageName() != _image) {
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

    delete delNode;
    delete temp;
    delete aux;

}

/**
 * Busca el indice del Node si se le ingresa un nombre para buscarlo
 * @param _nombre
 * @return int
 */
int List::getIndex(string _nombre) {
    Node* temp = head;
    int i = 0;
    while (temp != nullptr) {
        if (temp->getImageName() == _nombre) {
            return i;
        }
        i++;
        temp = temp->getNext();
    }

    delete temp;

    ///Retorna -1 si no lo encuentra
    return -1;
}

/**
 * Retorna un Node por su index.
 * @param posicion
 * @return Node
 */
Node* List::getNode(int _index) {
    Node* temp = nullptr;
    ///_index = [0 , len]
    if (_index >= 0 && _index <= len) {
        int i = 0;
        temp = head;
        while (i != _index) {
            temp = temp->getNext();
            i++;
        }
    }
    return temp;
}

/**
 * Retorna un Node por su nombre.
 * @param letra
 * @return Node
 */
Node* List::getNode(string _nombre) {
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->getImageName() == _nombre) {
            return temp;
        }
        temp = temp->getNext();
    }

    delete temp;

    return nullptr;
}

/**
 * Imprime los Nodes en List.
 */
void List::printList() {
    cout << "length: " << len << "\n[ ";
    Node* temp = head;
    while (temp != nullptr) {
        ///Para que no imprima el ultimo con una coma
        if (temp->getNext() == nullptr) {
            cout << temp->getImageName();
            break;
        }
        cout << temp->getImageName() << ", ";
        temp = temp->getNext();
    }

    cout << " ]" << endl;

    delete temp;

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
