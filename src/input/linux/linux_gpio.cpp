/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2019)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Stéphane Chatty <chatty@djnn.net>
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#include "core/utils/error.h"
#include "core/execution/graph.h"
#include "linux_input.h"

#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

namespace djnn {
  static int num_gpios = 0;
  static std::map<int,GPIOLine*> gpio_lines;

  void
  p_init_gpios ()
  {
    int num = 0;

    /* iterate on all controllers to determine the total number of GPIOs */
    while (1) {
      char filename[64];
      int fd;
      char buf[10];
      const char* p;
      int numgpio = 0;

      /* attempt to read the information file for the next controller */
      snprintf (filename, 64, "/sys/class/gpio/gpiochip%d/ngpio", num);
      fd = open (filename, O_RDONLY);
      if (fd < 0)
        break;

      /* read the number of GPIOs in this controller */
      read (fd, buf, 10);
      for (p = buf; *p != '\n'; ++p)
        numgpio = 10*numgpio + *p - '0';
      close (fd);

      /* update the total number of GPIOs */
      num += numgpio;
    }

    num_gpios = num-1;
    if (num_gpios < 0)
      fprintf (stderr, "djnn warning: no GPIO\n");
  }

  Process*
  p_find_gpio (const std::string& path, direction_e dir)
  {
    try {
      string::size_type sz;
      size_t index = std::stoi (path, &sz);
      map<int, GPIOLine*>::iterator it = gpio_lines.find (index);
      GPIOLine* line = nullptr;
      if (it != gpio_lines.end ()) {
        line = it->second;
      } else {
        line = new GPIOLine (nullptr, "line" + to_string (index), index, dir);
        line->activate ();
      }
      if (path.length() > (sz + 1))
          return line->find_child (path.substr ((sz + 1)));
        else
          return line;
      }
    catch (invalid_argument& arg) {
      warning (nullptr, "invalid gpio path specification: " + path);
    }
    return nullptr;
  }

  GPIOLine::GPIOLine (Process *parent, const std::string& name, int pin, direction_e dir)
   : Process (name),
   _pin (pin),
   _dir (dir),
   _iofd (nullptr),
   _action (nullptr),
   _c_action (nullptr)
  {
    if (pin < 0 || pin > num_gpios)
      error (this, "no gpio " + to_string (pin));
    _value = new BoolProperty (this, "value", true);

    /* activate the GPIO interface */
    _fd = open ("/sys/class/gpio/export", O_WRONLY);
    char buf[64];
    const char* direction = _dir == IN ? "in" : "out";
    const int dirlen = _dir == IN ? 2 : 3;

    if (_fd < 0)
      error (this, "unable to open gpio");
    write (_fd, buf, snprintf (buf, 64, "%d", _pin));
    close (_fd);

    /* set it to the desired direction */
    snprintf (buf, 64, "/sys/class/gpio/gpio%d/direction", pin);
    _fd = open (buf, O_WRONLY);
    if (_fd < 0) {
      error (this, "cannot set direction of GPIO " + to_string (pin));
    }
    write (_fd, direction, dirlen);
    close (_fd);

    /* open the value file  */
    snprintf (buf, 64, "/sys/class/gpio/gpio%d/value", pin);
    _fd = open (buf, _dir == IN ? O_RDONLY : O_WRONLY);
    if (_fd < 0) {
      error (this, "cannot open GPIO " + to_string (pin));
    }
    if (dir == IN) {
      _iofd = new IOFD (nullptr, "gpiofd", _fd);
      _iofd->activate ();
      _action = new GPIOLineReadAction (this, "read");
      _c_action = new Coupling (_iofd->find_child ("readable"), ACTIVATION, _action, ACTIVATION);
      Graph::instance().add_edge (_iofd->find_child ("readable"), _action);
    } else {
      _action = new GPIOLineWriteAction (this, "write");
      _c_action = new Coupling (_value, ACTIVATION, _action, ACTIVATION);
      Graph::instance().add_edge (_value, _action);
    }
    Process::finalize_construction (parent, name);
  }

  GPIOLine::~GPIOLine()
  {
    if (_dir == IN) {
      _iofd->deactivate ();
      Graph::instance().remove_edge (_iofd->find_child ("readable"), _action);
      delete _c_action;
      delete _action;
      delete _iofd;
    } else {
      Graph::instance().remove_edge (_value, _action);
      delete _c_action;
      delete _action;
    }
    close (_fd);
    delete _value;
  }

  void
  GPIOLine::read_value ()
  {
    char buf[10];
    lseek (_fd, 0, SEEK_SET);
    if (read (_fd, buf, 10) > 0)
      _value->set_value (buf[0] - '0', true);
  }

  void
  GPIOLine::write_value ()
  {
    char buf[4];
    int value = _value->get_value ();
    snprintf (buf, 4, "%d", value);
    write (_fd, buf, strlen (buf) + 1);
  }
}
