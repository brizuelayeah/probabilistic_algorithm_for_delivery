#!/bin/bash
#script para ejecutar y compilar el programa

# ejecuta el fichero en python
echo "Generando fichero de texto con los datos"
python3 graph_generator.py

echo "Fichero generado y compilando y ejecutando el programa"
g++ -std=c++11 -o main main.cc
./main
