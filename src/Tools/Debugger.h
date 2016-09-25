#pragma once

#include <iostream>
#include <string>

#define DEBUG

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)



#ifdef DEBUG
#define _DEBUG_MSG(msg) std::cout << "[" << __FILENAME__ << "]\t"; std::cout << msg << std::endl;
#define _DEBUG_TIMER_INIT() double start=clock()
#define _DEBUG_TIMER_ZERO() start=clock()
#define _DEBUG_TIME(name) std::cout << "[" << __FILENAME__ << "]\t"; \
							std::cout << name << ": "<< (clock() - start) / CLOCKS_PER_SEC <<" s.\n"; \
							start=clock();

#define _PRINT_TIME(name, instruction) do{	\
										_DEBUG_TIMER_ZERO(); \
										instruction;		 \
										_DEBUG_TIME(name);	 \
										}while(0);

#else

#define _DEBUG_MSG
#define _DEBUG_TIMER_INIT()
#define	_DEBUG_TIMER_ZERO()
#define _DEBUG_TIME

#endif