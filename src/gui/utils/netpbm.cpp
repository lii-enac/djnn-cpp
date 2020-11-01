#include <stdio.h>

namespace djnn {

bool write_pgm(const unsigned char* buf, 
               unsigned width, 
               unsigned height, 
               const char* file_name)
{
    FILE* fd = fopen(file_name, "wb");
    if(fd) {
        fprintf(fd, "P5 %d %d 255\n", width, height);
        fwrite(buf, 1, width * height, fd);
        fclose(fd);
        return true;
    } else {
        perror("pgm");
    }
    return false;
}

bool write_ppm(const unsigned char* buf, 
                unsigned width, 
                unsigned height, 
                const char* file_name)
{
    FILE* fd = fopen(file_name, "wb");
    if(fd) {
        fprintf(fd, "P6 %d %d 255\n", width, height);
        fwrite(buf, 1, width * height * 3, fd);
        fclose(fd);
        return true;
    } else {
        perror("ppm");
    }
    return false;
}

bool write_ppm_argb(const unsigned char* buf, 
            unsigned width, 
            unsigned height, 
            const char* file_name)
{
    FILE* fd = fopen(file_name, "wb");
    if(fd) {
        unsigned char _buf2[width*height*3];
        unsigned char* buf2=_buf2;
        for (int i=0; i<width*height; ++i) {
            buf2[0] = buf[2];
            buf2[1] = buf[1];
            buf2[2] = buf[0];
            buf2+=3;
            buf+=4;
        }
        fprintf(fd, "P6 %d %d 255\n", width, height);
        fwrite(_buf2, width * height * 3, 1, fd);
        fclose(fd);
        return true;
    } else {
        perror("ppm_argb");
    }
    return false;
}

bool write_pam(const unsigned char* buf, 
            unsigned width, 
            unsigned height, 
            const char* file_name)
{
    FILE* fd = fopen(file_name, "wb");
    if(fd) {
        fprintf(fd, "P7\nWIDTH %d\nHEIGHT %d\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n", width, height);
        fwrite(buf, 1, width * height * 4, fd);
        /*for (int i=0; i<width*height; ++i) {
            fwrite(&buf[i*4+1], 3, 1, fd); // RGB
            fwrite(&buf[i*4], 1, 1, fd); // A
        }*/
        fclose(fd);
        return true;
    } else {
        perror("pam");
    }
    return false;
}
}
