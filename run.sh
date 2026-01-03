#!/bin/bash

# Nome della cartella build
BUILD_DIR="build"

# Funzione per buildare il progetto
build_project() {
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR" || exit
    cmake ..
    make -j$(nproc)
    cd ..
}

# Funzione per buildare ed eseguire il progetto
start_project() {
    EXECUTABLE="./$BUILD_DIR/maziogra_http"
    if [ -f "$EXECUTABLE" ]; then
        echo "Avvio del server..."
        "$EXECUTABLE"
    else
        echo "Errore: eseguibile non trovato!"
        exit 1
    fi
}

# Controllo argomenti
if [ "$1" == "-b" ]; then
    build_project
elif [ "$1" == "-s" ]; then
    start_project
elif [ "$1" == "-bs" ]; then
    build_project
    start_project
else
    echo "Uso: $0 -build | -start"
    exit 1
fi
