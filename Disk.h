
#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_DISK_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_DISK_H


#include "List.h"

/**
 * Header de Disk.
 *
 * @since 04/06/19
 */


class Disk {

private:
    List* bitList;
    int id;

public:
    ///Constructores
    Disk(int _id);

    ///Metodos


    ///Getters & Setters
    List* getBitList();
    void setBitList(List *_list);
    int getId();
    void setId(int _id);

};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_DISK_H
