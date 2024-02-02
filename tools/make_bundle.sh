#!/bin/bash

# cmd to DEBUG
#rm dbg.txt ; open --stderr dbg.txt --stdout dbg.txt magnitude.app ; cat dbg.txt

# --- user data --- #
# - les repertoires a copier si besoin
# - Définit la variable d'environnement PATH -

#data_dir="data curve img diagrams"

echo 
echo "----------------------------------"
echo "note:" 
echo " - Copy and execute this script directly in the directory that you wish to transform into an app."
echo " - Also, don't forget to enter the PATH and the data directories to copy directly into the script."
echo "----------------------------------"

# Vérifiez que le script a été appelé avec un argument (le chemin de l'exécutable).
if [ $# -ne 1 ]; then
    echo "Usage: $0 <executable>"
    exit 1
fi

# Récupérez le nom de l'exécutable sans le chemin.
executable="$1"
executable_name=$(basename "$executable")

echo "Erase Bundle : $executable_name.app"
rm -rf ${executable_name}.app

# Créez un répertoire pour le bundle.
bundle_dir="${executable_name}.app"
mkdir -p "$bundle_dir/Contents/MacOS"

# Copiez l'exécutable dans le bundle.
cp "$executable" "$bundle_dir/Contents/MacOS/"

# Vérifiez si data_dir est vide sinon poser la question
if [[ -z $data_dir ]]; then
    # Ask how many directories to copy
    echo "How many directories do you want to copy to $bundle_dir/Contents/MacOS?"
    read -p "Enter the number of directories to copy: " num_directories

    if [[ $num_directories -eq 0 ]]; then
        echo "Nothing to do."
    elif [[ $num_directories -gt 0 ]]; then
        echo 
        for ((i=1; i<=$num_directories; i++)); do
            read -p "Enter the path of directory $i: " source_directory
            cp -r "$source_directory" "$bundle_dir/Contents/MacOS/"
            echo "Directory $source_directory copied to $bundle_dir/Contents/MacOS/"
        done
    else
            echo "Invalid number. Please enter a number greater than 1."
    fi
else
    # Si data_dir n'est pas vide, copiez chaque répertoire dans $bundle_dir/Contents/MacOS/
    for dir in $data_dir; do
        cp -r "$dir" "$bundle_dir/Contents/MacOS/"
        echo "Directory $dir copied to $bundle_dir/Contents/MacOS/"
    done
    num_directories=$(echo $data_dir | wc -w)
fi


# Créez un fichier Info.plist pour le bundle.
cat > "$bundle_dir/Contents/Info.plist" <<EOL
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>${executable_name}</string>
    <key>CFBundleName</key>
    <string>${executable_name}</string>
    <key>CFBundleIdentifier</key>
    <string>com.enac.${executable_name}</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundleVersion</key>
    <string>1</string>
    <key>NSFileReadAccess</key>
    <true/>
    <key>NSFileWriteAccess</key>
    <true/>
    <key>LSEnvironment</key>
     <dict>
        <key>MinimumSystemVersion</key>
        <string>12.0.0</string>
    </dict>
</dict>
</plist>
EOL

# crée fichier de launch.sh
cat > "$bundle_dir/Contents/MacOS/launch.sh" <<EOL
#!/bin/bash

working_directory=$(dirname "$0")
cd $working_directory ; ./${executable_name}
EOL

chmod +x $bundle_dir/Contents/MacOS/launch.sh


mkdir -p "$bundle_dir/Contents/Library"
cp ../djnn-cpp/build/lib/libdjnn-* .
cp ../smala/build/lib/lib* .

dylibbundler -od  -b -x ./$bundle_dir/Contents/MacOS/$executable_name -d ./$bundle_dir/Contents/Library -p @executable_path/../Library -s ./

echo ""
echo " -----------  CHECK EXEC ---------- "
echo ""
otool -L ./$bundle_dir/Contents/MacOS/$executable_name

echo ""
echo " -----------  Create Qt framework dependencies using : macdeployqt ----------  "
echo ""

/usr/local/opt/qt@5/bin/macdeployqt $bundle_dir -verbose=1 

echo ""
echo " -----------  CHECK libdjnn-gui ----------  "
echo ""
otool -L ./$bundle_dir/Contents/Library/libdjnn-gui.dylib

echo ""
echo " -----------  CHECK PlugIns libqcocoa.dylib ----------  "
echo ""
install_name_tool $bundle_dir/Contents/PlugIns/platforms/libqcocoa.dylib -id "@executable_path/../PlugIns/platforms/libqcocoa.dylib"
otool -L ./$bundle_dir/Contents/PlugIns/platforms/libqcocoa.dylib


rm libdjnn-*
rm libsmala*

# fabrique un faux launch si besoin d'image ..etc 
# Check the number of directories
if [[ $num_directories -eq 0 ]]; then
    echo "Nothing to do."
elif [[ $num_directories -gt 0 ]]; then

    # Créez un fichier Info.plist pour le bundle.
cat > "$bundle_dir/Contents/Info.plist" <<EOL
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>launch.sh</string>
    <key>CFBundleName</key>
    <string>${executable_name}</string>
    <key>CFBundleIdentifier</key>
    <string>com.enac.${executable_name}</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundleVersion</key>
    <string>1</string>
    <key>NSFileReadAccess</key>
    <true/>
    <key>NSFileWriteAccess</key>
    <true/>
    <key>LSEnvironment</key>
     <dict>
        <key>MinimumSystemVersion</key>
        <string>12.0.0</string>
    </dict>
</dict>
</plist>
EOL

    #crée fichier de launch.sh
cat > "$bundle_dir/Contents/MacOS/launch.sh" <<EOL
#!/bin/bash

working_directory=\$(dirname "\$0")
cd \$working_directory ; ./${executable_name}
EOL

    chmod +x $bundle_dir/Contents/MacOS/launch.sh
else
        echo "Invalid number. Please enter a number greater than 1."
fi

echo ""
echo " -----------  CHECK signature ----------  "
echo ""
codesign --force --deep --preserve-metadata=entitlements,requirements,flags,runtime --sign - $executable_name.app
codesign --verbose=2 $executable_name.app

# install_name -id changed

# remarque : pour debugger le bundle 
# open --stderr dbg.txt --stdout dbg.txt toto.app