# libblif

A library to parse BLIF (Berkeley Logic Interchange Format) files.

## How to use

    cd build
    cmake ..
    make -j4 && sudo make install
    sudo ldconfig

## Develop

    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make -j4
    make test

## Documentation

    cd src
    doxygen
