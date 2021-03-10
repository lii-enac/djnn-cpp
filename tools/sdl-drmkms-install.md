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
apt install -y git libpcre3-dev  
git clone git://github.com/lii-enac/libivy.git
cd libivy/src
make libivy.a
install -m644 libivy.a /usr/local/lib
make includes
cd ../..
# get rid of annoying perl message
export LANG=""
