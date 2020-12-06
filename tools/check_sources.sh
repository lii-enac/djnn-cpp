echo -- no include of precompiled headers (only in precompiled.h)
find src -type f -not -path "*/ext/*" | xargs grep -E "include <(vector|map|string|list)>" | grep -v '://'

echo -- no iostream in headers
find src -type f -name "*.h" | xargs grep "include <iostream>"

echo -- minimize use of iostream
find src -type f -not -path "*/ext/*" -name "*.cpp" | xargs grep "include <iostream>"

echo -- no include of a whole module in internal sources
find src -type f -not -path "*/ext/*" | xargs grep -E "(core|base|gui)\.h"
find src -type f -not -path "*/ext/*" | xargs grep -E "(core|base|gui)-dev\.h"
