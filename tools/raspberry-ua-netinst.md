# raspberry unattended network installation for djnn/smala
# uses opengl, sdl and kmsdrm (no X11)
# should be minimal and reasonably fast booting
# allows for installation of dev tools if need be, and easier/faster to setup than buildroot
# regular, up-to-date raspbian
# only a ~50mb .img to burn at first, everything is installed fresh from the network
# next steps: make it completely unattended

# download last raspbian-ua-netinst release and burn it onto an sdcard with e.g. etcher (https://www.balena.io/etcher/):
# https://github.com/FooDeas/raspberrypi-ua-netinst
# https://github.com/FooDeas/raspberrypi-ua-netinst/releases

# optional: /raspberrypi-ua-netinst/config/installer-config.txt on the fat32 partition, replaced by apt install -y in the following steps
# packages=git,make,g++,libgles2-mesa-dev,libgbm-dev,pkg-config

# future /raspberrypi-ua-netinst/config/post-install.txt for complete unattended installation
# as of now, do it by hand after boot:
# login: root
# passwd: raspbian

# if git cloning from git.recherche.enac.fr, not necessary from github
ssh-keygen
ssh-keyscan git.recherche.enac.fr >> ~/.ssh/known_hosts
cat /root/.ssh/id_rsa.pub
# put the content of id_rsa.pub into https://git.recherche.enac.fr/my/account

# from here, copy/paste the remaining and it should install unattended
apt update
apt install -y git make g++ libgles2-mesa-dev libgbm-dev pkg-config

git clone git@git.recherche.enac.fr:liiprojects/djnn-cpp.git -b opengl
git clone git@git.recherche.enac.fr:liiprojects/smalaproject/smala.git

cd djnn-cpp
# echo -e "display := SDL\ngraphics := GL\nspecialtarget := raspberry-ua-netinst" > config.mk
# echo -e "display := DRM\ngraphics := GL\nspecialtarget := raspberry-ua-netinst\nCXXFLAGS +=-DGL2D_RPI2 -DMASK_STB" > config.mk
cat <<EOT >> config.mk
display := DRM
graphics := GL
gl=gles2
picking := COLOR
specialtarget := raspberry-ua-netinst
CFLAGS += -O0 -g
CXXFLAGS +=-DGL2D_RPI4 -DMSDFGL
EOT

make install-pkgdeps
cd ..

# get rid of annoying perl message
export LANG=""

cd smala
make install-pkgdeps
cd ..

# raspbian SDL2 is not compiled with kmsdrm enabled, compile it with the option
wget https://www.libsdl.org/release/SDL2-2.0.14.tar.gz
tar xzf SDL2-*.tar.gz
cd SDL2-2.0.14
./configure --enable-video-kmsdrm --disable-video-x11
make -j4
make install
cd ..

# could have apt installed SDL2_image, but it would install SDL2 deb and a *lot* of dependencies that we do not want
wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5.tar.gz
tar xzf SDL2_image*.tar.gz
cd SDL2_image-2.0.5
./configure
make -j4
make install
cd ..

# ivy
apt install -y subversion libpcre3-dev  
svn co https://svn.tls.cena.fr/svn/ivy/ivy-c/trunk ivy-c  
cd ivy-c/src
make libivy.a
install -m644 libivy.a /usr/local/lib
make includes
cd ../..
# get rid of annoying perl message
export LANG=""


# compile everything
cd djnn-cpp
make -j4
cd ..

cd smala
make -j4
cd ..

# test
cd smala
make -j4 simplest_test


# notes:

# test kmsdrm:
# apt install kmscube
# enable option vc4-kms* in /boot/config.txt
# kmscube -D /dev/dri/card1

# to see why certain packages are installed:
# apt rdepends --installed pkgname

# check ip
# arp -i bridge100 -a

# update firmware
# sudo apt-get install rpi-update
# rpi-update

# common errors
# "failed to set crtc for connector"
# => width an height of frame should be the same of the connector
