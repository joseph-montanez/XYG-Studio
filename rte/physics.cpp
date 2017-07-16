/*===============*\
| Chipmunk SOURCE |
\*===============*/



#include "main.h"
#include "global.h"
#include "physics.h"


size_t xyNewSpace() {
	vcSpaces.push_back(cpSpaceNew());
	size_t index = vcSpaces.size() - 1;

    return index;
}

bool xyHasPhysics(SQInteger index) {
	if (static_cast<size_t>(index) >= vcSpaces.size()) {
		xyError(0, "The physics world index: %d does not exist", index);
		return false;
	}

	return true;
}

void xySetGravity(SQInteger index, SQFloat x, SQFloat y) {
	if (!xyHasPhysics(index)) {
		return;
	}

	cpVect gravity = cpv(static_cast<cpFloat>(x), static_cast<cpFloat>(y));

	cpSpaceSetGravity(vcSpaces[index], gravity);
}

void xyDeleteSpace(SQInteger index) {
	if (!xyHasPhysics(index)) {
		return;
	}

	vcSpaces.erase(vcSpaces.begin() + index);
}


/*
cpShapeFree(ballShape);
cpBodyFree(ballBody);
cpShapeFree(ground);
cpSpaceFree(space);

*/
