#!/bin/bash

# howTo
# cd tools
# ./format_clang.sh ../src/core 

# Utiliser le premier argument comme répertoire. Si aucun argument n'est fourni, utiliser le répertoire courant.
dir=${1:-.}

# Liste pour stocker les fichiers exclus
excluded_files=""

# Fonction pour vérifier si un chemin contient "/ext"
contains_excluded_pattern() {
    if [[ $1 == *"/ext/"* || $1 == *"/gl/"* || $1 == *"/sdl/"* ]]; then
        return 0
    else
        return 1
    fi
}

# Appliquer clang-format aux fichiers d'en-tête (.h)
while IFS= read -r file; do
    if contains_excluded_pattern "$file"; then
        excluded_files+=" $file"
        continue
    fi
    clang-format --style=file:../src/.clang-format_header -i "$file" --verbose
    
done < <(find "$dir" -type f -name "*.h")

# Appliquer clang-format aux fichiers de code source (.cpp)
while IFS= read -r file; do
    if contains_excluded_pattern "$file"; then
        excluded_files+=" $file"
        continue
    fi
    clang-format --style=file:../src/.clang-format -i "$file" --verbose
    
done < <(find "$dir" -type f -name "*.cpp")

# Afficher la liste des fichiers exclus
echo "---------  path containing /ext/ are excluded files  -----------"
for file in $excluded_files; do
    echo "$file"
done