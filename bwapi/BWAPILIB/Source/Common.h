#pragma once

#define BWAPI_TYPEDEF(type,x) const type x(Enum::x)

#ifndef countof
#define countof(x) ((unsigned int)( sizeof(x)/sizeof((x)[0]) ))
#endif
