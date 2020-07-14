#pragma once

#ifdef DJNN_CRAZYFLIE
extern "C" {
  void DJNN_DEBUG_PRINT(const char* fmt);
  //extern void vTaskDelay (const unsigned uint_32);
  //extern void vTaskDelay( const TickType_t xTicksToDelay ) PRIVILEGED_FUNCTION;
}
#ifdef CRAZYFLIE_FW
#include "freertos.h"
#include "task.h" // vTaskDelay
#endif

// #ifndef DEBUG_PRINT
// int eprintf(putc_t putcf, char * fmt, ...);
// #define consolePrintf(FMT, ...) eprintf(consolePutchar, FMT, ## __VA_ARGS__)
// #define DEBUG_FMT(fmt) fmt
// #define DEBUG_PRINT(fmt, ...) consolePrintf(DEBUG_FMT(fmt), ##__VA_ARGS__)
// #endif

#endif

#ifndef __djnn_str1
#define __djnn_str(x) #x
#define __djnn_str1(x) __djnn_str(x)
#endif

#ifndef __FL__

#ifndef __DJNN__FUNCTION__
    #ifdef __PRETTY_FUNCTION__
        #define __DJNN_FUNCTION__ __PRETTY_FUNCTION__
    #else
        #define __DJNN_FUNCTION__ __FUNCTION__
    #endif
#endif

#define __FL__ " " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;

#ifndef DBG

#ifdef DJNN_CRAZYFLIE
//#define DBG  {DJNN_DEBUG_PRINT(__FILE__ ":" __djnn_str1(__LINE__) "\n"); vTaskDelay ((unsigned int)200);} // allow some time for transmitting text before crash
#define DBG  {DJNN_DEBUG_PRINT(__FILE__ ":" __djnn_str1(__LINE__) "\n");}
#else
#define DBG djnn::__debug(__FILE__, __DJNN_FUNCTION__, __djnn_str1(__LINE__));
//#define DBG std::cerr << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#endif

#define DBGPROC std::cerr << this << " " << get_name() << " " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl;
#define UNIMPL { static int done=false; if(!done) {std::cerr << "unimplemented " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl; done=true;} }
#define DEPR { static int done=false; if(!done) {std::cerr << "deprecated " << __DJNN_FUNCTION__ << ":" << __FILE__ << ":" << __LINE__ << std::endl; done=true;} }

#define DBGVAR(x) #x << ":" << x

#endif
#endif


#ifndef DJNN_NO_DEBUG

/* DEBUG DISPLAY INFORMATION */
/* 
  diplayed infos :
    - a new frame with interactive object beside (right) the main frame
*/
#define _DEBUG_SEE_COLOR_PICKING_VIEW  0

/*
 diplayed infos (LIGHT BLUE) : 
 - time to draw and average
 - nb object to draw
 - nb object in color picking view
*/
#define _DEBUG_SEE_GUI_INFO_PREF 0

/*
 diplayed infos (GREEN and YELLOW) : 
 - time to exec graph and average
 - time to sort graph and average
 - nb vertices and edges in graph 
 - nb vertices and edges in sorted_graph 
*/
#define _DEBUG_SEE_GRAPH_INFO_PREF 0


/*
 diplayed infos (YELLOW) : 
 - display order in which component are activated and dactivated between to GRAPH_EXEC
*/
#define _DEBUG_SEE_ACTIVATION_SEQUENCE 0

/*
 diplayed infos (PURPLE, RED and GREEN) : 
 - display order in which component are created or destroyed between to call of utils/display_creation_stats () function
*/
#define _DEBUG_SEE_CREATION_DESTRUCTION_ORDER 0


#endif