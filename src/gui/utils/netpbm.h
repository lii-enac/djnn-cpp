namespace djnn {
bool write_pgm (const unsigned char* buf,
                unsigned             width,
                unsigned             height,
                const char*          file_name);

bool write_ppm (const unsigned char* buf,
                unsigned             width,
                unsigned             height,
                const char*          file_name);

bool write_ppm_argb (const unsigned char* buf,
                     unsigned             width,
                     unsigned             height,
                     const char*          file_name);

bool write_pam (const unsigned char* buf,
                unsigned             width,
                unsigned             height,
                const char*          file_name);
} // namespace djnn
