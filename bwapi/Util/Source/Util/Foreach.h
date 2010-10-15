#pragma once

#ifdef __GNUC__
#define foreach(element, collection) \
    for(typeof((collection).begin()) _foreach_iter = (collection).begin(); _foreach_iter != (collection).end(); ++_foreach_iter) \
    if(bool _foreach_loop = false) {} else \
    for(element = *_foreach_iter; !_foreach_loop; _foreach_loop = true)
#else
#define foreach(element, collection) for each(element in collection)
#endif
