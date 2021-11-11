#pragma once
#include <SDL/SDL.h>

#define LOG_SHOW 1
#if LOG_SHOW
    #define SCOPE_LOG(name) SDL_Log (name)
#else
    #define SCOPE_LOG(name) 
#endif 

#define CONSTRUCT_LOG_SHOW 0
#if CONSTRUCT_LOG_SHOW
    #define CONSTRUCT_LOG() SCOPE_LOG(__FUNCTION__)
#else
#define CONSTRUCT_LOG() 
#endif

#define DESTRUCTOR_LOG_SHOW 0
#if DESTRUCTOR_LOG_SHOW
#define DESTRUCTOR_LOG() SCOPE_LOG(__FUNCTION__)
#else
#define DESTRUCTOR_LOG() 
#endif


#define FUNC_LOG_SHOW 0
#if FUNC_LOG_SHOW
#define FUNC_LOG() SCOPE_LOG(__FUNCTION__)
#else
#define FUNC_LOG() 
#endif

#define DEBUGGING_ATTACK_SPRITE 0
