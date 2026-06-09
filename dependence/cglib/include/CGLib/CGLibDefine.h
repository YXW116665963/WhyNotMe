#ifndef __CGLIBDEFINE_H__
#define __CGLIBDEFINE_H__

#ifndef DeleteData
#define DeleteData(pointer) \
   if(pointer != nullptr){delete pointer; pointer = nullptr;}
#endif

#if defined( _WIN32 )
#define CGLIB_EXPORTS __declspec(dllexport)

#else
#define CGLIB_EXPORTS
#endif

#include <limits>
#include <stdexcept>

#define PI 3.141592653589793

static const double DoubleEpsilon = 1E-6;
static const double DistanceEpsilon = 1E-4;
static const double AngleEpsilon = 1E-2;
static const double MaxDistance = 1E9;
//static const double Pi;
static const int NURBSMaxDegree = 7;

//#include <pthread.h>

#define VALIDATE_ARGUMENT(condition,arg,message)\
	if(!(condition)){\
		throw std::invalid_argument((std::string(message)+"\r\nParameter name: "+#arg).c_str());\
	}

#define VALIDATE_ARGUMENT_RANGE(arg, min, max)\
	if(((arg)<(min) && std::fabs((arg)-(min))>DoubleEpsilon) || \
	  ((arg)>(max) && std::fabs((arg)-(max))>DoubleEpsilon)){\
		throw std::out_of_range("Argument is out of range["#min","#max"]\r\nParameter name: "#arg);\
	}

#endif 