#!/bin/bash

# --- user data --- #
# - les repertoires a copier si besoin
# - Définit la variable d'environnement PATH -
export PATH=:../djnn-cpp/build/lib:../smala/build/lib:$PATH
#data_dir="data curve img diagrams"

echo 
echo "----------------------------------"
echo "note:" 
echo " - Copy and execute this script directly in the directory that you wish to transform into an app."
echo " - Also, don't forget to enter the PATH and the data directories to copy directly into the script."
echo "----------------------------------"

# Vérifie si l'argument est un fichier exécutable
if [ ! -x "$1" ]; then
  echo "Usage: $0 executable_file"
  exit 1
fi

# Récupère le nom de l'exécutable
executable=$(basename "$1")

# supprime et Crée un répertoire de travail nommé "executable_win_app"
rm -rf "${executable%.*}_win_app"
mkdir -p "${executable%.*}_win_app"/plugins

# Copie tous les fichiers du répertoire /c/msys64/mingw64/share/qt5/plugins/ dans le répertoire executable_win_app
cp -r /c/msys64/mingw64/share/qt5/plugins/platforms "${executable%.*}_win_app/plugins"

# Crée un fichier batch nommé "executable.bat" dans le répertoire executable_win_app
echo "@echo off" > "${executable%.*}_win_app/${executable%.*}.bat"
echo "set QT_QPA_PLATFORM=windows" >> "${executable%.*}_win_app/${executable%.*}.bat"
echo "set QT_PLUGIN_PATH=plugins" >> "${executable%.*}_win_app/${executable%.*}.bat"
echo "" >> "${executable%.*}_win_app/${executable%.*}.bat"
echo "start ${executable%.*}.exe" >> "${executable%.*}_win_app/${executable%.*}.bat"
echo "" >> "${executable%.*}_win_app/${executable%.*}.bat"
echo "rem \"enjoy !\"" >> "${executable%.*}_win_app/${executable%.*}.bat"
echo "" >> "${executable%.*}_win_app/${executable%.*}.bat"
echo "exit" >> "${executable%.*}_win_app/${executable%.*}.bat"

# Copie l'exécutable donné en argument dans le répertoire executable_win_app
cp "$1" "${executable%.*}_win_app/"

# Récupère le répertoire de l'exécutable
directory=$(dirname "$1")

# Si un deuxième argument est fourni, utilise-le comme répertoire de recherche
if [ -n "$2" ]; then
  directory="$2"
fi

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

  # Copie chaque bibliothèque dynamique requise dans le répertoire "${executable%.*}_win_app/"
  for lib in $libs; do
    cp "$lib" "${executable%.*}_win_app/$(basename "$lib")"
  done
done

# Copie tous les répertoires locaux spécifiés dans la variable data_dir dans le répertoire executable_win_app
echo "=== data dir = \"$data_dir\" ==="
for dir in $data_dir; do
  echo "copy of $dir"
  cp -r "$dir" "${executable%.*}_win_app/"
done