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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

#define STRINGIZE(x)  STRINGIZE2 (x)
#define STRINGIZE2(x) #x
#define LINE_STRING   STRINGIZE(__LINE__)

#define DBG_GET (">> GET " __FILE__ ":" LINE_STRING)
#define DBG_REL ("<< REL " __FILE__ ":" LINE_STRING)

namespace djnn {

class FatProcess;

void init_global_mutex ();
bool is_global_mutex_inited ();

void get_exclusive_access (const char* debug);
void release_exclusive_access (const char* debug);

void lock_ios_mutex ();
void release_ios_mutex ();

void start (FatProcess* c);
void stop (FatProcess* c);
} // namespace djnn
