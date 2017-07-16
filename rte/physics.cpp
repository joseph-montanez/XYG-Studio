/*===============*\
| Chipmunk SOURCE |
\*===============*/



#include "main.h"
#include "global.h"
#include "physics.h"


Uint32 xyChipmunkSpaceNew() {
    vcSpaces.push_back(cpSpaceNew());

    return 0;
}
