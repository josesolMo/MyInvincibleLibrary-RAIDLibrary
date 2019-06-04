cmake_minimum_required(VERSION 3.13)
project(MyInvincibleLibrary-RAIDLibrary)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyInvincibleLibrary-RAIDLibrary main.cpp
        Disk.cpp Disk.h Galeria.cpp Galeria.h
        Image.cpp Image.h List.cpp List.h Node.cpp Node.h
        RAIDController.cpp RAIDController.h)