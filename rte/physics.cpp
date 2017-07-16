/*===============*\
| Chipmunk SOURCE |
\*===============*/



#include <chipmunk_structs.h>
#include "main.h"
#include "global.h"
#include "physics.h"

//-- Shapes

SQBool xyHasShape(SQInteger index) {
	if (static_cast<size_t>(index) >= vcShapes.size()) {
		xyError(0, "The physics shape index: %d does not exist", index);
		return false;
	}

	return true;
}

void xyDeleteShape(SQInteger index) {
	if (!xyHasShape(index)) {
		return;
	}

	cpShapeFree(vcShapes[index]);
	vcShapes.erase(vcShapes.begin() + index);
}

SQInteger xyNewStaticBox(SQInteger index, SQInteger x, SQInteger y, SQInteger width, SQInteger height) {
	if (!xyHasSpace(index)) {
		return -1;
	}

	cpSpace *space = vcSpaces[index];
	cpBody *body = space->staticBody;

	cpShape *ground = cpSegmentShapeNew(space->staticBody, cpv(x, y), cpv(x + width, y + height), 0);

	vcShapes.push_back(ground);

	cpSpaceAddShape(vcSpaces[index], ground);

	return static_cast<SQInteger>(vcShapes.size() - 1);
}

void xyShapeSetFriction(SQInteger index, SQFloat friction) {
	if (!xyHasShape(index)) {
		return;
	}

	cpShapeSetFriction(vcShapes[index], friction);
}

SQFloat xyGetCiricleMomentum(SQFloat mass, SQFloat radius) {

	cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

	return static_cast<SQFloat>(moment);
}

//-- Space Functions

SQInteger xyNewSpace() {
	vcSpaces.push_back(cpSpaceNew());
	size_t index = vcSpaces.size() - 1;

	return static_cast<SQInteger>(index);
}

SQBool xyHasSpace(SQInteger index) {
	if (static_cast<size_t>(index) >= vcSpaces.size()) {
		xyError(0, "The physics world index: %d does not exist", index);
		return false;
	}

	return true;
}

void xySetSpaceGravity(SQInteger index, SQFloat x, SQFloat y) {
	if (!xyHasSpace(index)) {
		return;
	}

	cpVect gravity = cpv(static_cast<cpFloat>(x), static_cast<cpFloat>(y));

	cpSpaceSetGravity(vcSpaces[index], gravity);
}

void xyDeleteSpace(SQInteger index) {
	if (!xyHasSpace(index)) {
		return;
	}

	cpSpaceFree(vcSpaces[index]);
	vcSpaces.erase(vcSpaces.begin() + index);
}

SQInteger xySpaceAddBody(SQInteger spaceIndex, SQInteger bodyIndex) {
	if (!xyHasSpace(spaceIndex)) {
		return -1;
	}
	if (!xyHasBody(bodyIndex)) {
		return -1;
	}

	cpSpaceAddBody(vcSpaces[spaceIndex], vcBodies[bodyIndex]);

	return bodyIndex;
}

SQInteger xySpaceAddShape(SQInteger spaceIndex, SQInteger shapeIndex) {
	if (!xyHasSpace(spaceIndex)) {
		return -1;
	}
	if (!xyHasShape(shapeIndex)) {
		return -1;
	}

	cpSpaceAddShape(vcSpaces[spaceIndex], vcShapes[shapeIndex]);

	return shapeIndex;
}

void xySpaceStep(SQInteger index, SQInteger timeStep) {
	if (!xyHasSpace(index)) {
		return;
	}

	cpSpaceStep(vcSpaces[index], timeStep);
}


//-- Bodies

SQBool xyHasBody(SQInteger index) {
	if (static_cast<size_t>(index) >= vcBodies.size()) {
		xyError(0, "The physics body index: %d does not exist", index);
		return false;
	}

	return true;
}

SQInteger xyNewBody(SQFloat mass, SQFloat momentum) {
	cpBody *body = cpBodyNew(mass, momentum);

	vcBodies.push_back(body);

	return static_cast<SQInteger>(vcBodies.size() - 1);
}

void xyDeleteBody(SQInteger index) {
	if (!xyHasBody(index)) {
		return;
	}

	cpBodyFree(vcBodies[index]);
	vcBodies.erase(vcBodies.begin() + index);
}
