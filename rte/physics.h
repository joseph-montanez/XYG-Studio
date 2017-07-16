/*===============*\
| Chipmunk SOURCE |
\*===============*/



#ifndef RTE_PHYSICS_H
#define RTE_PHYSICS_H

#include "main.h"

//-- Shapes
SQBool xyHasShape(SQInteger index);
void xyDeleteShape(SQInteger index);
SQInteger xyNewStaticBox(SQInteger index, SQInteger x, SQInteger y, SQInteger width, SQInteger height);
void xyShapeSetFriction(SQInteger index, SQFloat friction);
SQFloat xyGetCiricleMomentum(SQFloat mass, SQFloat radius);

//-- Space Functions
SQInteger xyNewSpace();
SQBool xyHasSpace(SQInteger index);
void xySetSpaceGravity(SQInteger index, SQFloat x, SQFloat y);
void xyDeleteSpace(SQInteger index);
SQInteger xySpaceAddBody(SQInteger spaceIndex, SQInteger bodyIndex);
SQInteger xySpaceAddShape(SQInteger spaceIndex, SQInteger shapeIndex);
void xySpaceStep(SQInteger index, SQInteger timeStep);

//-- Bodies
SQBool xyHasBody(SQInteger index);
SQInteger xyNewBody(SQFloat mass, SQFloat momentum);
void xyDeleteBody(SQInteger index);


#endif //RTE_PHYSICS_H
