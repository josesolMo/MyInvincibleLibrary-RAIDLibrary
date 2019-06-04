
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
    Disk* disk1;
    Disk* disk2;
    Disk* disk3;
    Disk* disk4;

    string actualSplit1;
    string actualSplit2;
    string actualSplit3;
    string actualParity;


public:
    ///Constructores
    RAIDController();

    ///Metodos
    void splitImage(Image* image);

    ///Getters & Setters
    Disk* getDisk1();
    void setDisk1(Disk* _disk1);
    Disk* getDisk2();
    void setDisk2(Disk* _disk2);
    Disk* getDisk3();
    void setDisk3(Disk* _disk3);
    Disk* getDisk4();
    void setDisk4(Disk* _disk4);

    string getActualSplit1();
    void setActualSplit1(string _actualSplit1);
    string getActualSplit2();
    void setActualSplit2(string _actualSplit2);
    string getActualSplit3();
    void setActualSplit3(string _actualSplit3);
    string getActualParity();
    void setActualParity(string _actualParity);



};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_RAIDCONTROLLER_H
