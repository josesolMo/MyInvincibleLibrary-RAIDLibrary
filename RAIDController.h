
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

    Image* actualImage;

    string TICDirectory;
    string DCDirectory;

    string actualSplit1;
    string actualSplit2;
    string actualSplit3;
    string actualParity;

    int parityDiskIndex;
    int imagePart1DiskIndex;
    int imagePart2DiskIndex;
    int imagePart3DiskIndex;


public:
    ///Constructores
    RAIDController();

    ///Metodos

    bool write(Image *newImage);
    bool hexDataToBMP(string name, string hexData);
    int hexToDecimal(string h);
    bool split();
    int getActualImageFileLength();
    string BMPtoBinaryData(string disk, string num);
    bool brokenBinary();
    bool XORParity();



    void verifyParity();



    int getActualImageFileLength(string name);




    bool saveImage(Image *newImage);

    bool binaryDataToBMP();

    string decimalToBinary(int d);
    int binaryToDecimal(string b);

    void nextParityDiskIndex();





    bool split(string name, int fileSize);

    ///Getters & Setters

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
