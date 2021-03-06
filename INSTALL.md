We provide a Makefile that should work on MacOS (10.12+), Linux, Windows 10+.

---- LINUX -----  
in a Terminal:

```
linux package for ubuntu 20.04: https://github.com/lii-enac/djnn-cpp/releases
dpkg -i djnn-cpp-x.xx.x.deb

or using sources

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
pacman -Syu
pacman -Syu --needed git make pkg-config
```
launch a MSYS2-MinGW64/32 terminal (but NOT MSYS2-MSYS) then:  
```
install pacmac (ArchLinux) package .tar.zst from https://github.com/lii-enac/djnn-cpp/releases 
pacman -U djnn-cpp-x.xx.x-1.pkg.tar.zst

or using sources

git clone -c core.symlinks=true https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j  
``` 

---- ALL PLATFORMS -----  
ivy bus -> https://github.com/lii-enac/libivy

Windows:
```
install pacmac (ArchLinux) package .tar.zst from https://github.com/lii-enac/libivy/releases 
pacman -U libivy-x.xx.x-1.pkg.tar.zst

or using sources

(pacman -Syu)
(pacman -Syu --needed git make pkg-config)
git clone http://github.com:lii-enac/libivy.git  
cd libivy/src
make -j4 -f Makefile.mingw upgrade-pkgdeps
make -j4 -f Makefile.mingw 
```

Linux/Ubuntu:
```

linux package for ubuntu 20.04 . deb : https://github.com/lii-enac/libivy/releases 
dpkg -i libivy-x.xx.x.deb

or using sources

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

git clone git@github.com:lii-enac/libivy.git  
cd libivy/src
make -f Makefile.osx static-libs shared-libs  
make -f Makefile.osx install
```
