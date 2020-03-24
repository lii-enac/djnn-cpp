#ifndef __djnn_exec_env_c__
#define __djnn_exec_env_c__ 1

extern void djnn_init_exec_env ();
extern void djnn_clear_exec_env ();
extern void djnn_launch_mainloop ();
extern void djnn_init_prog ();

#ifdef DJNN_USE_FREERTOS
extern void * timer_thread_local;
extern void * freertos_thread_local;
#endif

#endif /*__djnn_exec_env_c__*/
