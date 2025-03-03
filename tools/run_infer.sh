#!/bin/bash

# Définir la racine du projet (en supposant que le script est dans le répertoire `tools`)
PROJECT_ROOT=$(realpath "$(dirname "$0")/..")
INFER_BIN="$PROJECT_ROOT/../infer-osx-x86_64-v1.2.0/bin/infer"

INFER_CONFIG_FILE=".inferconfig"

# Fonction pour filtrer les messages indésirables (ex : dyld)
filter_output() {
    grep -v 'dyld\[.*\]:'
}

# Vérification que le paramètre d'entrée a été fourni
if [ -z "$1" ]; then
  # Si aucun paramètre n'est donné, on utilise `make -j4`
  TARGET="make"
  echo "⚠️  Aucune cible de make fournie. Utilisation par défaut : '$TARGET'"
else
  # Sinon, on utilise la cible fournie en paramètre
  TARGET="make $1 -j4"
  echo "✅ Utilisation de la cible fournie : '$TARGET'"
fi

# Vérification que .inferconfig existe à la racine du projet
# if [ ! -f "$PROJECT_ROOT/.inferconfig" ]; then
#   echo "❌ Erreur: Le fichier .inferconfig est manquant. Veuillez le placer à la racine du projet."
#   exit 1
# fi

# # Vérification du format JSON de .inferconfig
# if ! cat "$PROJECT_ROOT/.inferconfig" | jq . > /dev/null 2>&1; then
#   echo "❌ Erreur: Le fichier .inferconfig a un format JSON invalide."
#   exit 1
# fi

# Vérification de l'existence d'Infer dans le répertoire spécifié
if [ ! -f "$INFER_BIN" ]; then
  echo "❌ Erreur: Infer n'a pas été trouvé à l'emplacement $INFER_BIN."
  exit 1
fi

# Nettoyage des anciens rapports Infer
echo "🧹 Suppression des anciens rapports Infer..."
rm -rf "$PROJECT_ROOT/infer-out/"

# Affichage de la commande en évidence avant exécution
echo -e "\n **Commande exécutée**  "
#echo -e "\033[1;33m INFER_CONFIG_FILE=$INFER_CONFIG_FILE $INFER_BIN run --keep-going -- $TARGET\033[0m"
echo -e "\033[1;33m $INFER_BIN run --keep-going -- $TARGET\033[0m"
echo -e "\n"

# Exécution de la commande Infer avec filtrage des erreurs dyld
# cd .. &&  exec env INFER_CONFIG_FILE="$INFER_CONFIG_FILE" script -q /dev/null "$INFER_BIN" run --keep-going -- $TARGET 2>&1 | filter_output
cd .. &&  exec script -q /dev/null "$INFER_BIN" run --keep-going -- $TARGET 2>&1 | filter_output

# Copie du rapport infer
cp "$PROJECT_ROOT/infer-out/"report.txt "$PROJECT_ROOT/tools/infer_report.txt"

# Ouverture du report infer
open "$PROJECT_ROOT/tools/infer_report.txt"