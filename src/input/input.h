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
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

#pragma once

#include "core/ontology/process.h"

namespace djnn {

	extern vector<string> loadedModules;
	extern FatProcess *InputDevices, *Mice, *TouchPanels, *GPIOs;
 	
 	void init_input ();
  
}


/*
note on udev/evdev :

Accessing to the device files

By default, for security reasons, a non-root user does not have read/write permission to the device files managed by the udevd daemon. This is an issue for the djnn's input module. One solution consists in creating a udev rule that will set the correct permissions to a specific group: 
- Create a new input group: sudo groupadd -f input
- Add the current user to this group: sudo gpasswd -a <username> input
- Create the file (the location may be distro-dependent) : sudo touch /etc/udev/rules.d/99-input.rules
- with the following content

 KERNEL=="event*", NAME="input/%k", MODE="660",GROUP="input"

- Restart the udev daemon: sudo /etc/init.d/udev restart  or  reboot
- reboot the computer 

*/