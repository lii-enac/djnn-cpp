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
 *      Stéphane Conversy <stephane.conversy@enac.fr>
 *
 */

#ifndef __djnn_exec_env_c__
#define __djnn_exec_env_c__ 1

namespace djnn {
    class CoreProcess;
}

extern void djnn_init_exec_env ();
extern void djnn_clear_exec_env ();
extern void djnn_launch_mainloop ();
extern void djnn_init_prog ();
djnn::CoreProcess* djnn_mainloop_instance ();

#ifdef DJNN_USE_FREERTOS
extern void * timer_thread_local;
extern void * freertos_thread_local;
#endif

#endif /*__djnn_exec_env_c__*/
