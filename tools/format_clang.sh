#!/bin/bash

# Utiliser le premier argument comme répertoire. Si aucun argument n'est fourni, utiliser le répertoire courant.
dir=${1:-.}

# Parcourir tous les fichiers .h et .cpp dans le répertoire donné et ses sous-répertoires
for file in $(find $dir -iname "*.h" -o -iname "*.cpp")
do
    # Appliquer clang-format au fichier
    clang-format -i $file

    # Afficher le nom du fichier formaté
    echo "Formatted: $file"
done
