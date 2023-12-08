We provide a `Makefile` that should work on MacOS (10.12+), Linux, Windows 10+.

---- LINUX -----

* With a debian binary package for ubuntu 20.04:  
  get the package here: <https://github.com/lii-enac/djnn-cpp/releases>,  
  then in a Terminal:

```
dpkg -i djnn-cpp-x.xx.x.deb
````

* or using sources:

```
sudo apt install git make pkg-config
git clone --depth 1 https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j4  
```

---- MACOS -----  

  You need to install homebrew: follow the installation instructions over at <https://brew.sh/>  
  Then in a Terminal:
```
brew install git make pkg-config
```

* Then with the binary package...:

```
brew tap lii-enac/repo
brew install djnn-cpp
```

* ...or using the sources:

```
git clone --depth 1 https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j  
```

---- WINDOWS -----  

You need to install MSYS2: follow the installation instructions over at <https://www.msys2.org/>  
Then in an MSYS2-MinGW64/32 terminal:

```
pacman -Syu
pacman -S --needed git make
```

* Then with an MSYS2 binary package...  
  get the `.tar.zst` package here: <https://github.com/lii-enac/djnn-cpp/releases>  
  launch an MSYS2-MinGW64/32 terminal (and *not* an MSYS2-MSYS one)  
  then install it:

```
pacman -U djnn-cpp-x.xx.x-1.pkg.tar.zst
```

* ...or using the sources:

```
git clone --depth 1 -c core.symlinks=true https://github.com/lii-enac/djnn-cpp.git  
cd djnn-cpp  
make install-pkgdeps  
make -j  
```

---- ALL PLATFORMS -----  
if you need the ivy bus <https://github.com/lii-enac/libivy>

Windows:
download the pacman package .tar.zst from <https://github.com/lii-enac/libivy/releases>, and install it:

```
pacman -U libivy-x.xx.x-1.pkg.tar.zst
```

or using the sources:

```
git clone --depth 1 http://github.com:lii-enac/libivy.git  
cd libivy/src
make -j4 -f Makefile.mingw upgrade-pkgdeps
make -j4 -f Makefile.mingw 
```

Linux/Ubuntu:
linux package for ubuntu 20.04 . deb : <https://github.com/lii-enac/libivy/releases>

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
