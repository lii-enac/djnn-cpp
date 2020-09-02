We provide a Makefile that should work on MacOS (10.12+), Linux, Windows 10+.

---- LINUX -----  
in a Terminal:

```
sudo apt install git make pkg-config
git clone https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j4  
```
---- MACOS -----  
install homebrew (https://brew.sh/)  
then in a Terminal:
```
brew install git make pkg-config
brew tap lii-enac/repo
brew install djnn-cpp

or using sources

git clone https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j  
```
---- WINDOWS -----  
install MSYS2 (follow instructions from https://www.msys2.org/)  
then in an MSYS2-MSYS terminal:
```
pacman -S git make pkg-config
```
launch a MSYS2-MinGW64/32 terminal (but NOT MSYS2-MSYS) then:  
```
git clone -c core.symlinks=true https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j  
``` 

---- ALL PLATFORMS -----  
ivy bus -> https://github.com/lii-enac/libivy 

Windows:
```
pacman -S subversion patch
svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c  
cd ivy-c
patch -p0 -i ../djnn-cpp/tools/ivy-C-patch-windows.diff
cd src
make -j4 -f Makefile.mingw static-libs shared-libs
make -j4 -f Makefile.mingw installlibs
```

Linux/Ubuntu:
```

linux package for ubuntu 18.04: https://github.com/lii-enac/libivy/releases 
else
sudo apt install libpcre3-dev  
git clone https://github.com/lii-enac/libivy 
cd libivy/src  
make -j4 
sudo make -j4 installlibs includes  
```
MacOS:  
```
brew install subversion pcre pkg-config
brew tap lii-enac/repo
brew install libivy

or using sources

svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c  
cd ivy-c/src  
make -f Makefile.osx static-libs shared-libs  
sudo make -f Makefile.osx installlibs includes
```
