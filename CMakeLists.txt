cmake_minimum_required(VERSION 3.13)
project(MyInvincibleLibrary-RAIDLibrary-MetadataDB)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyInvincibleLibrary-RAIDLibrary-MetadataDB main.cpp
        List.cpp List.h Node.cpp Node.h Imagen.cpp Imagen.h
        InvincibleManager.cpp InvincibleManager.h Galeria.cpp Galeria.h)