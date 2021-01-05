#echo -- no include of precompiled headers - only in precompiled.h
#find src -type f -not -path "*/ext/*" -not -name "precompiled.h" | xargs grep -E "include <(vector|map|string|list)>" | grep -v '://'

echo
echo -- no iostream in headers
find src -type f -name "*.h" | xargs grep "include <iostream>"

echo
echo -- no include of a whole module in internal sources
find src -type f -not -path "*/ext/*" | xargs grep -E "/(core|exec_env|base|display|gui)\.h"
#find src -type f -not -path "*/ext/*" | xargs grep -E "(core|base|gui)-dev\.h"

echo
echo -- no include of a whole submodule in internal headers
find src -type f -name "*.h" -not -name "shapes.h" -not -name "gui.h" | xargs grep "include" | grep "shapes.h"
find src -type f -name "*.h" -not -name "*style.h" -not -name "gui.h" | xargs grep "include" | grep "/style.h"
find src -type f -name "*.h" -not -name "*transformations.h" -not -name "gui.h" | xargs grep "include" | grep "transformations.h"

echo
echo -- minimize use of iostream
find src -type f -not -path "*/ext/*" -name "*.cpp" | xargs grep "include <iostream>"

echo
echo -- minimize include of a whole submodule in internal sources
find src -type f -name "*.cpp" -not -name "shapes.h" | xargs grep "include" | grep "shapes.h"
find src -type f -name "*.cpp" -not -name "*style.h" | xargs grep "include" | grep "/style.h"
find src -type f -name "*.cpp" -not -name "*transformations.h" | xargs grep "include" | grep "transformations.h"