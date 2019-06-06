
#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_RAIDCONTROLLER_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_RAIDCONTROLLER_H

#include "Image.h"
#include "Disk.h"


/**
 * Header de InvincibleManager
 *
 * @since 04/06/19
 */


class RAIDController {

private:
    Disk* disk0;
    Disk* disk1;
    Disk* disk2;
    Disk* disk3;

    Image* actualImage;

    string actualSplit1;
    string actualSplit2;
    string actualSplit3;
    string actualParity;

    int parityDiskIndex;


public:
    ///Constructores
    RAIDController();

    ///Metodos
    void splitImage(Image* image);
    void nextParityDiskIndex();
    void testGetImageHeader(string nombre);

    ///Getters & Setters
    Disk* getDisk0();
    void setDisk0(Disk *_disk0);
    Disk* getDisk1();
    void setDisk1(Disk *_disk1);
    Disk* getDisk2();
    void setDisk2(Disk *_disk2);
    Disk* getDisk3();
    void setDisk3(Disk *_disk3);

    Image* getActualImage();
    void setActualImage(Image* _actualImage);

    //Splits y Parity pueden ser un Node

    string getActualSplit1();
    void setActualSplit1(string _actualSplit1);
    string getActualSplit2();
    void setActualSplit2(string _actualSplit2);
    string getActualSplit3();
    void setActualSplit3(string _actualSplit3);
    string getActualParity();
    void setActualParity(string _actualParity);

    int getParityDiskIndex();
    void setParityDiskIndex(int _parityDiskIndex);



};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_RAIDCONTROLLER_H
