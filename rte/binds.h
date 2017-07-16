/*===============*\
| BINDINGS HEADER |
\*===============*/

#ifndef _BINDINGS_H_
#define _BINDINGS_H_

//Main
SQInteger sqUpdate(HSQUIRRELVM v);
SQInteger sqGetOS(HSQUIRRELVM v);
SQInteger sqGetTicks(HSQUIRRELVM v);
SQInteger sqGetFPS(HSQUIRRELVM v);
SQInteger sqSetFPS(HSQUIRRELVM v);
SQInteger sqSetWindowTitle(HSQUIRRELVM v);

//File IO
SQInteger sqImport(HSQUIRRELVM v);
SQInteger sqFileExists(HSQUIRRELVM v);
SQInteger sqGetDir(HSQUIRRELVM v);	//Needs def
SQInteger sqSetDir(HSQUIRRELVM v);	//Needs def

//Graphics
SQInteger sqWait(HSQUIRRELVM v);
SQInteger sqClearScreen(HSQUIRRELVM v);
SQInteger sqSetDrawTarget(HSQUIRRELVM v);
SQInteger sqDrawImage(HSQUIRRELVM v);
SQInteger sqSetDrawColor(HSQUIRRELVM v);
SQInteger sqSetBackgroundColor(HSQUIRRELVM v);
SQInteger sqLoadImage(HSQUIRRELVM v);
SQInteger sqLoadImageKeyed(HSQUIRRELVM v);
SQInteger sqSetScalingFilter(HSQUIRRELVM v);
SQInteger sqSetResolution(HSQUIRRELVM v);

//Sprites
SQInteger sqNewSprite(HSQUIRRELVM v);
SQInteger sqDrawSprite(HSQUIRRELVM v);
SQInteger sqDrawSpriteEx(HSQUIRRELVM v);
SQInteger sqDeleteSprite(HSQUIRRELVM v);

//Input
SQInteger sqKeyPress(HSQUIRRELVM v);
SQInteger sqKeyRelease(HSQUIRRELVM v);
SQInteger sqKeyDown(HSQUIRRELVM v);
SQInteger sqResetDrawTarget(HSQUIRRELVM v);
SQInteger sqLoadImage(HSQUIRRELVM v);
SQInteger sqDrawImage(HSQUIRRELVM v);
SQInteger sqMouseX(HSQUIRRELVM v);
SQInteger sqMouseY(HSQUIRRELVM v);
SQInteger sqMouseDown(HSQUIRRELVM v);
SQInteger sqMousePress(HSQUIRRELVM v);
SQInteger sqMouseRelease(HSQUIRRELVM v);
SQInteger sqGetQuit(HSQUIRRELVM v);

//Maths
SQInteger sqRandomFloat(HSQUIRRELVM v);
SQInteger sqRandomInt(HSQUIRRELVM v);
SQInteger sqDistance2(HSQUIRRELVM v);
SQInteger sqWrap(HSQUIRRELVM v);
SQInteger sqFloor(HSQUIRRELVM v);
SQInteger sqPointAngle(HSQUIRRELVM v);

//Text
SQInteger sqDrawText(HSQUIRRELVM v);
SQInteger sqOpenFont(HSQUIRRELVM v);
SQInteger sqCloseFont(HSQUIRRELVM v);

//Audio
SQInteger sqLoadSound(HSQUIRRELVM v);
SQInteger sqLoadMusic(HSQUIRRELVM v);
SQInteger sqPlaySound(HSQUIRRELVM v);
SQInteger sqPlayMusic(HSQUIRRELVM v);
SQInteger sqDeleteSound(HSQUIRRELVM v);
SQInteger sqDeleteMusic(HSQUIRRELVM v);

//Misc
SQInteger sqEmbedTest(HSQUIRRELVM v);

//Physics
SQInteger sqNewSpace(HSQUIRRELVM v);
SQInteger sqDeleteSpace(HSQUIRRELVM v);
SQInteger sqSpaceStep(HSQUIRRELVM v);
SQInteger sqSpaceSetGravity(HSQUIRRELVM v);
SQInteger sqSpaceAddBody(HSQUIRRELVM v);
SQInteger sqNewBody(HSQUIRRELVM v);
SQInteger sqSpaceAddShape(HSQUIRRELVM v);
SQInteger spNewStaticBox(HSQUIRRELVM v);
SQInteger spShapeSetFriction(HSQUIRRELVM v);


#endif
