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
git clone https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j  
```
---- WINDOWS -----  
install MSYS2 (https://www.msys2.org/)  
then in an MSYS2 terminal:
```
pacman -S git make pkg-config
```
launch a mingw64 terminal (and NOT MSYS2) then:  
```
git clone -c core.symlinks=true https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j  
``` 

---- ALL PLATFORMS -----  
ivy bus -> https://www.eei.cena.fr/products/ivy/  

Windows:
```
pacman -S subversion
svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c  
cd ivy-c/src
make static-libs shared-libs  
#sudo make installlibs includes  
```

Linux/Ubuntu:
```
sudo apt install subversion libpcre3-dev  
svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c  
cd ivy-c/src  
make static-libs shared-libs  
sudo make installlibs includes  
```
MacOS:  
```
brew install subversion pcre  
svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c  
cd ivy-c/src  
make -f Makefile.osx static-libs shared-libs  
sudo make -f Makefile.osx installlibs includes
```
