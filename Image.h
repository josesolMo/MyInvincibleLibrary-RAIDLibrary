
#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_IMAGEN_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_IMAGEN_H

#include <iostream>

using namespace std;


/**
 * Header de Image.
 *
 * @since 02/06/19
 */


class Image {

private:
    string name;
    string rawHexString;
    string tempDirectory;
    int byteQuantity;



    string modBinString;
    int additionalBytes;

public:
    ///Constructores
    Image(string _nombre);
    Image(string _nombre, string _rawHexadecimalString);

    ///Metodos
    int getFileSizeFromDirectory();





    string decimalToBinary(int d);
    int binaryToDecimal(string b);

    string toBinary();
    void toBmp();

    void split();

    void testImage();
    void getHeader();
    void printBytes();
    void recreateFile();

    ///Getters & Setters
    string getNombre();
    void setNombre(string _nombre);
    string getRawHexString();
    void setRawHexString(string _rawHexString);
    string getTempDirectory();
    void setTempDirectory(string _tempDirectory);
    int getByteQuantity();
    void setByteQuantity(int _byteQuantity);

};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_IMAGEN_H
