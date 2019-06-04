cmake_minimum_required(VERSION 3.13)
project(MyInvincibleLibrary-RAIDLibrary-MetadataDB)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyInvincibleLibrary-RAIDLibrary-MetadataDB main.cpp
        Galeria.cpp Galeria.h
        InvincibleManager.cpp InvincibleManager.h Image.cpp Image.h
        List.cpp List.h MetadataDB.cpp MetadataDB.h Node.cpp Node.h
        RAIDLibrary.cpp RAIDLibrary.h)