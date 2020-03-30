/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "serial.h"

#include "core/utils/error.h"
#include "exec_env/main_loop.h"
#include "utils/debug.h"

#include <iostream>
#include <string>
#include <unistd.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>


using namespace std;


namespace djnn
{

 Serial::Serial (Process *parent, const std::string& name, const std::string& port, int baudrate, char eol) :
         Process (name), _port (port),
         _out ( this, "out", ""),
         _in (this, "in", ""),
         _baudrate (this, "baudrate", baudrate),
         _out_a (this, "out_action"),
         _out_c ( &_out , ACTIVATION, &_out_a, ACTIVATION, true ),
         _buf_max (256), _fd (-1), _eol (eol)
 {

  _out_c.disable ();

  Process::finalize_construction (parent, name);
}

 Serial::~Serial ()
{
  remove_state_dependency (get_parent (), &_out_a);
}

void
Serial::set_parent (Process* p)
{ 
  /* in case of re-parenting remove edge dependency in graph */
  if (get_parent ()) {
      remove_state_dependency (get_parent (), &_out_a);
  }

  add_state_dependency (p, &_out_a);
    
  Process::set_parent (p); 
}

void
Serial::impl_activate ()
{
  init_comm ();
  _out_c.enable ();
  ExternalSource::start ();
}

void
Serial::impl_deactivate ()
{
  _out_c.disable ();
  please_stop();
}

/*
 * This code is taken from
 * https://github.com/todbot/arduino-serial/blob/master/arduino-serial-lib.c
 * See also https://todbot.com/blog/
 */
void
Serial::init_comm ()
{
  struct termios toptions;
  _fd = open (_port.c_str (), O_RDWR | O_NONBLOCK );

  if (_fd == -1)  {
    error (this, "serial port_init: Unable to open port ");
  }
  if (tcgetattr (_fd, &toptions) < 0) {
    error (this, "serialport_init: Couldn't get term attributes");
  }
  speed_t brate = _baudrate.get_value (); // let you override switch below if needed
  switch (_baudrate.get_value ()) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
	#ifdef B14400
    case 14400:  brate=B14400;  break;
	#endif
    case 19200:  brate=B19200;  break;
	#ifdef B28800
    case 28800:  brate=B28800;  break;
	#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
  }
  cfsetispeed (&toptions, brate);
  cfsetospeed (&toptions, brate);

  // 8N1
  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;
  // no flow control
  toptions.c_cflag &= ~CRTSCTS;

  //toptions.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid reset

  toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
  toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

  toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
  toptions.c_oflag &= ~OPOST; // make raw

  // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
  toptions.c_cc[VMIN]  = 0;
  toptions.c_cc[VTIME] = 0;
  //toptions.c_cc[VTIME] = 20;

  tcsetattr (_fd, TCSANOW, &toptions);
  if (tcsetattr (_fd, TCSAFLUSH, &toptions) < 0) {
    error (this, "init_serialport: Couldn't set term attributes");
  }
}

void
Serial::serial_write () {
  const char* msg = _out.get_value ().c_str();
  int len = strlen(msg);
  int n = write (_fd, msg, len);
  if( n!=len ) {
    error (this, "serial write: couldn't write whole string\n");
  }
}

void
Serial::run ()
{
  set_please_stop (false);
  try {
    char buf[_buf_max];
    char b[1];
    int i = 0;
    while (!get_please_stop ()) {
      memset (buf, 0, _buf_max);
      i = 0;
      b[0] = 0;
      do {
        int n = read (_fd, b, 1);  // read a char at a time
        if (n == -1)
          error (this, "Unable to read on serial port");    // couldn't read
        if (n == 0) {
          usleep (1 * 1000);  // wait 1 msec try again
          continue;
        }
        buf[i] = b[0];
        i++;
      } while( b[0] != _eol && i < _buf_max);
      if (i > 2) {
        djnn::get_exclusive_access (DBG_GET);
        string res (buf, i - 2);
        _in.set_value (res, true);
        GRAPH_EXEC;
        djnn::release_exclusive_access (DBG_REL);
      }
    }
  } catch (exception& e) {
    warning (nullptr, e.what());
  }
}

}
