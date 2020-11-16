/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Poirier <mathieu.poirier@enac.fr>
 *
 */


#include "comms.h"

#ifdef _WIN32
  /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501  /* Windows XP. */
  #endif
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
#endif


namespace djnn
{
  
  static bool __module_initialized = false;


  int clean_socket()
  {
    #ifdef _WIN32
      return WSACleanup();
    #else
      return 0;
    #endif
  }

  void
  init_comms () {

    if ( __module_initialized == false ) {

      __module_initialized = true;
      
      djnn::loadedModules.push_back("comms");
      
    }
#ifdef _WIN32
      WSADATA wsa_data;
      WSAStartup(MAKEWORD(2,2), &wsa_data);
#endif
  }
  //TODO We should clean winsock
}
