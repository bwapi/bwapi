#pragma once
#define BWAPI_TYPESET(setname,name,...) static const int setname ## struc[] = { __VA_ARGS__ };                    \
                            static const name::const_set setname(setname ## struc, countof(setname ## struc));

#define BWAPI_TYPEDEF(type,x) const type x(Enum::x)

#ifndef countof
#define countof(x) ((unsigned int)( sizeof(x)/sizeof((x)[0]) ))
#endif
