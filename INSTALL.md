We provide a Makefile that should work on MacOS (10.12+), Linux, Windows 10+.

---- LINUX -----
linux package for ubuntu 20.04: https://github.com/lii-enac/djnn-cpp/releases
in a Terminal:
```
dpkg -i djnn-cpp-x.xx.x.deb
````

or using sources:

```
sudo apt install git make pkg-config
git clone --depth 1 https://github.com/lii-enac/djnn-cpp.git  
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
```
or using sources:
```
git clone --depth 1 https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j  
```
---- WINDOWS -----  
install MSYS2 (from https://www.msys2.org/, follow installation steps **1 to 6** ONLY )  
then in an MSYS2-MSYS terminal:
```
pacman -Syu
pacman -S --needed git make pkg-config
```
launch a MSYS2-MinGW64/32 terminal (but NOT MSYS2-MSYS) then install pacmac (ArchLinux) package .tar.zst from https://github.com/lii-enac/djnn-cpp/releases :
```
pacman -U djnn-cpp-x.xx.x-1.pkg.tar.zst
```
or using sources:
```
git clone --depth 1 -c core.symlinks=true https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j  
``` 

---- ALL PLATFORMS -----  
ivy bus -> https://github.com/lii-enac/libivy

Windows:
install pacman (ArchLinux) package .tar.zst from https://github.com/lii-enac/libivy/releases :
```
pacman -U libivy-x.xx.x-1.pkg.tar.zst
```
or using sources:
```
#pacman -Syu
#pacman -Syu --needed git make pkg-config
git clone --depth 1 http://github.com:lii-enac/libivy.git  
cd libivy/src
make -j4 -f Makefile.mingw upgrade-pkgdeps
make -j4 -f Makefile.mingw 
```

Linux/Ubuntu:
linux package for ubuntu 20.04 . deb : https://github.com/lii-enac/libivy/releases
```
dpkg -i libivy-x.xx.x.deb
```
or using sources
```
sudo apt install libpcre3-dev  
git clone --depth 1 https://github.com/lii-enac/libivy 
cd libivy/src  
make -j4 
sudo make -j4 installlibs includes  
```
MacOS:  
```
brew install pcre pkg-config
brew tap lii-enac/repo
brew install libivy
```
or using sources
```
git clone --depth 1 git@github.com:lii-enac/libivy.git  
cd libivy/src
make -f Makefile.osx static-libs shared-libs  
make -f Makefile.osx install
```
