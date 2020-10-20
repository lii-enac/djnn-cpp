//http://www.youtube.com/user/thecplusplusguy
//Playing 3D sound with OpenAL, and loading a wav file manually
#include <iostream>
#include <fstream>
#include <cstring>

#include <unistd.h>

bool isBigEndian()
{
    int a = 1;
    return !((char*)&a)[0];
}

int convertToInt(char* buffer, int len)
{
    int a = 0;
    if (!isBigEndian())
        for (int i = 0; i<len; i++)
            ((char*)&a)[i] = buffer[i];
    else
        for (int i = 0; i<len; i++)
            ((char*)&a)[3 - i] = buffer[i];
    return a;
}

char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
    char buffer[4];
    std::ifstream in(fn, std::ios::binary);
    if(!in.good())
    {
        std::cout << "could not open " << fn << std::endl;
        return NULL;
    }
    in.read(buffer, 4); //RIFF
    if (!in.good() || strncmp(buffer, "RIFF", 4) != 0)
    {
        std::cout << fn << " is not a valid WAVE file" << std::endl;
        return NULL;
    }
    in.read(buffer, 4);
    in.read(buffer, 4);      //WAVE
    in.read(buffer, 4);      //fmt
    in.read(buffer, 4);      //16
    
    if(!in.good() || convertToInt(buffer, 4) != 16) {
        std::cout << fn << " is not a PCM WAV file" << std::endl;
        return NULL;
    }
    in.read(buffer, 2);      //1
    if(!in.good() || convertToInt(buffer, 2) != 1) {
        std::cout << fn << " is not a PCM WAV file" << std::endl;
        return NULL;
    }
    in.read(buffer, 2);
    if(!in.good()) {
        std::cout << fn << " is not a PCM WAV file" << std::endl;
        return NULL;
    }
    chan = convertToInt(buffer, 2);
    if(chan <=0 || chan >2) {
        std::cout << fn << " number of audio channels" << std::endl;
        return NULL;
    }
    in.read(buffer, 4);
    samplerate = convertToInt(buffer, 4);
    in.read(buffer, 4);
    in.read(buffer, 2);
    in.read(buffer, 2);
    bps = convertToInt(buffer, 2);
    in.read(buffer, 4);      //data
    in.read(buffer, 4);
    size = convertToInt(buffer, 4);
    //std::cout << chan << " " << samplerate << " " << bps << " " << size << std::endl;
    char* data = new char[size];
    in.read(data, size);
    return data;
}
