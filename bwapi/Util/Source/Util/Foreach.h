#pragma once

#ifdef __GNUC__
#define foreach(element, collection) for(element : collection)  
#else
#define foreach(element, collection) for each(element in collection)
#endif
