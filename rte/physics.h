/*===============*\
| Chipmunk SOURCE |
\*===============*/



#ifndef RTE_PHYSICS_H
#define RTE_PHYSICS_H

#include "main.h"

size_t xyNewSpace();
bool xyHasPhysics(SQInteger index);
void xySetGravity(SQInteger index, SQFloat x, SQFloat y);


#endif //RTE_PHYSICS_H
