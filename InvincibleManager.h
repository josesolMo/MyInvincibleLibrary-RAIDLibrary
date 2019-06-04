
#ifndef MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_INVINCIBLEMANAGER_H
#define MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_INVINCIBLEMANAGER_H

#include "RAIDLibrary.h"
#include "MetadataDB.h"


/**
 * Header de InvincibleManager
 *
 * @since 03/06/19
 */


class InvincibleManager {

private:
    RAIDLibrary* raidLibrary;
    MetadataDB* metadataDB;


public:

    ///Constructores
    InvincibleManager();
    InvincibleManager(RAIDLibrary* _raidLibrary, MetadataDB _metadataDB);

    ///Metodos
    //-

    ///Getters & Setters
    RAIDLibrary* getRAIDLibrary();
    void setRAIDLibrary(RAIDLibrary* _raidLibrary);
    MetadataDB* getMetadataDB();
    void setMetadataDB(MetadataDB* _metadataDB);

};


#endif //MYINVINCIBLELIBRARY_RAIDLIBRARY_METADATADB_INVINCIBLEMANAGER_H
