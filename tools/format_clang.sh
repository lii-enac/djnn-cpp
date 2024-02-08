#!/bin/bash

# Utiliser le premier argument comme répertoire. Si aucun argument n'est fourni, utiliser le répertoire courant.
dir=${1:-.}

# Chemins vers les fichiers d'en-tête (.h)
HEADER_FILES=$(find $dir -type f -name "*.h")

# Chemins vers les fichiers de code source (.cpp)
CPP_FILES=$(find $dir -type f -name "*.cpp")

# Appliquer clang-format aux fichiers d'en-tête avec une configuration spécifique
for file in $HEADER_FILES
do
    clang-format --style=file:../src/_clang-format_header -i $file

    # Afficher le nom du fichier formaté
    echo "header formatted: $file"
done

# Appliquer clang-format aux fichiers de code source avec une configuration différente
for file in $CPP_FILES
do
    clang-format --style=file:../src/_clang-format -i $file

    # Afficher le nom du fichier formaté
    echo "src formatted: $file"
done