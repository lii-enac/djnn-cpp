/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020-2024)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */
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
