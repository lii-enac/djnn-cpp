#!/bin/bash

rm -rf stand_alone

# Vérifie si l'argument est un fichier exécutable
if [ ! -x "$1" ]; then
  echo "Usage: $0 executable_file [directory]"
  exit 1
fi

# Récupère le nom de l'exécutable
executable=$(basename "$1")

# Récupère le répertoire de l'exécutable
directory=$(dirname "$1")

# Si un deuxième argument est fourni, utilise-le comme répertoire de recherche
if [ -n "$2" ]; then
  directory="$2"
fi

# Définit la variable d'environnement PATH
export PATH=:/home/lii/djnn-cpp/build/lib:/home/lii/smala/build/lib:$PATH

# Parcourt tous les fichiers exécutables dans le répertoire donné
find "$directory" -type f -executable -print0 | while read -d $'\0' file; do
  # Ignore les fichiers exécutables qui ne sont pas du même type que l'exécutable donné
  if [ "$(file -b "$file")" != "$(file -b "$1")" ]; then
    continue
  fi

  # Affiche le nom du fichier exécutable
  echo "=== $file ==="

  # Liste toutes les bibliothèques dynamiques requises par le fichier exécutable
  libs=$(ldd "$file" | awk '{print $3}' | grep -iv 'Windows/System32' | sort -u)

  # Affiche le nom du fichier exécutable
  echo "$libs"

  # Crée un répertoire stand_alone s'il n'existe pas déjà
  mkdir -p stand_alone

  # Copie chaque bibliothèque dynamique requise dans le répertoire stand_alone
  for lib in $libs; do
    cp "$lib" "stand_alone/$(basename "$lib")"
  done
done
