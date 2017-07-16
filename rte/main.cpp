/*==============================================*\
| PROJECT:	XYG Studio Runtime Environment       |
| AUTHOR:		Nick Kovacs                      |
|	DATE:		8-15-15                          |
|	DESCRIPTION:Runtime environment used for     |
|	  games and applications created using       |
|	  the XYG Studio framework.                  |
|	LICENSE:	You are free to use, modify, and |
|	  redistribute this source code, in part,    |
|	  or in full, provided proper attribution    |
|	  is included and this information is not    |
|	  modified or removed. Please include a      |
|	  link to WWW.XYGSTUDIO.ORG in all           |
|	  projects that use this source code.        |
\*==============================================*/

/*===========*\
| MAIN SOURCE |
\*===========*/

//Main header
#include "main.h"
#include "global.h"
#include "input.h"
#include "graphics.h"
#include "maths.h"
#include "fileio.h"
#include "binds.h"
#include "text.h"
#include "audio.h"


/////////////////
//MAIN FUNCTION//
/////////////////

int main(int argc, char* args[]){
	//Initiate everything
	if(xyInit() == 0){
		xyPrint(0, "Failed to initiate XYG!");
		xyEnd();
		return 1;
	};

	//Process arguments
	string xygapp = "test.nut";
	string curarg = "";
	for(int i = 0; i < argc; i++){
		//Print each argument and process them
		curarg = args[i];
		if(i == 0){
			//-- Get Full Path
			char xyg_app_real_path[PATH_MAX];
			realpath(curarg.c_str(), xyg_app_real_path);
			curarg = xyg_app_real_path;
			//-- Get path from executable
			gvAppDir = curarg.substr(0, curarg.find_last_of("/\\") + 1);

			xyPrint(0, "App directory: %s", gvAppDir.c_str());
		};

		//Input file
		//If the file is long enough
		if(curarg.length() > 4 && curarg.find(".") !=std::string::npos){
			//If the file ends in '.xyg'
			size_t ext_pos = curarg.find_last_of(".");
			bool is_sq = curarg.substr(ext_pos) == ".sq";
			bool is_nut = curarg.substr(ext_pos) == ".nut";
			if(is_sq || is_nut){
				//-- Get Full Path
				char xyg_working_real_path[PATH_MAX];
				realpath(curarg.c_str(), xyg_working_real_path);

				//Check that the file really exists
				if(xyFileExists(xyg_working_real_path)){
					//All checks pass, assign the file
					xygapp = xyg_working_real_path;

					gvWorkDir = xygapp.substr(0, xygapp.find_last_of("/\\") + 1);
					chdir(gvWorkDir.c_str());
					char *cwd = getcwd(0, 0);
					xyPrint(0, "This is the working directory: %s", cwd);
				};
			};
		};

		//Other arguments
	};


	//Run app
	string strCoreLib = gvAppDir;
	strCoreLib += "xylib/core.nut";
	sqstd_dofile(gvSquirrel, strCoreLib.c_str(), 0, 1);
	sqstd_dofile(gvSquirrel, xygapp.c_str(), 0, 1);

	//End game
	xyEnd();

	return 0;
};

//////////////
//EXCEPTIONS//
//////////////



///////////////////
//OTHER FUNCTIONS//
///////////////////

int xyInit(){
	//Initiate log file
	remove("log.txt");
	gvLog.open("log.txt");

	//Print opening message
	xyPrint(0, "\n/========================\\\n| XYG STUDIO RUNTIME LOG |\n\\========================/\n\n");
	xyPrint(0, "Initializing program...\n\n");

	//Initiate SDL
	if(SDL_Init(SDL_INIT_EVERYTHING < 0)){
		xyPrint(0, "Failed to initialize! %s", SDL_GetError());
		return 0;
	};

	//Create window
	gvWindow = SDL_CreateWindow("XYG Runtime Environment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gvScrW, gvScrH, SDL_WINDOW_RESIZABLE);
	if(gvWindow == 0){
		xyPrint(0, "Window could not be created! SDL Error: %s\n", SDL_GetError());
		return 0;
	} else {
		//Create renderer for window
		gvRender = SDL_CreateRenderer(gvWindow, -1, SDL_RENDERER_ACCELERATED);
		if(gvRender == 0){
			xyPrint(0, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			return 0;
		} else {
			//Initialize renderer color
			SDL_SetRenderDrawColor(gvRender, 0xFF, 0xFF, 0xFF, 0xFF);

			//Initialize PNG loading
			if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
				xyPrint(0, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				return 0;
			};

			//Set up the viewport
			SDL_Rect screensize;
			screensize.x = 0;
			screensize.y = 0;
			screensize.w = gvScrW;
			screensize.h = gvScrH;
			SDL_RenderSetViewport(gvRender, &screensize);
			SDL_RenderSetLogicalSize(gvRender, gvScrW, gvScrH);

		};
	};

	//Initialize TTF
	if(TTF_Init() < 0){
		xyError(0, "SDL_ttf could not initialize! SDL_ttf error: %s\n", TTF_GetError());
		return 0;
	};

	//Initialize audio
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
		xyError(0, "SDL_mixer could not initialize! SDL_mixer error: %s\n", Mix_GetError());
		return 0;
	};

	//Initialize input
	xyInitInput();

	xyPrint(0, "SDL initialized successfully!");

	//Initiate Squirrel
	gvSquirrel = sq_open(1024);

	sqstd_register_mathlib(gvSquirrel);
	sqstd_register_iolib(gvSquirrel);
	sq_setprintfunc(gvSquirrel, xyPrint, xyError);
	sq_pushroottable(gvSquirrel);

	xyBindAllFunctions(gvSquirrel);

	/*Error handler does not seem to print compile-time errors. I haven't
	been able to figure out why, as the same code works in my other apps,
	and is taken from the sq.c example included with Squirrel.*/
	sqstd_seterrorhandlers(gvSquirrel);

	xyPrint(0, "Squirrel initialized successfully!");

	//Initiate other
	vcTextures.push_back(0);
	vcSprites.push_back(0);
	vcSounds.push_back(0);
	vcMusic.push_back(0);

	gvLog << "================\n\n";
	cout << "================\n\n";

	//Return success
	return 1;
};

void xyError(HSQUIRRELVM v, const SQChar *s, ...){
	va_list args;
	va_start(args, s);
	SQChar buffer[1024] = _SC("");
	vsnprintf(buffer, sizeof(buffer), s, args);
	va_end(args);

	cout << "!ERROR! >:: " << buffer << endl << endl;
	gvLog << "!ERROR! >:: " << buffer << endl << endl;
};

void xyEnd(){

	gvLog << "";
//	fprintf(gvLog, "================\n\n");
	printf("================\n\n");

	//Cleanup all resources
	xyPrint(0, "Cleaning up all resources...");
	for(int i = 0; i < vcTextures.size(); i++){
		xyDeleteImage(i);
	};

	for(int i = 0; i < vcSprites.size(); i++){
		delete vcSprites[i];
	};

	for(int i = 0; i < vcSounds.size(); i++){
		xyDeleteSound(i);
	};

	for(int i = 0; i < vcMusic.size(); i++){
		xyDeleteMusic(i);
	};

	//Close Squirrel
	xyPrint(0, "Closing Squirrel...");
	SQInteger garbage = sq_collectgarbage(gvSquirrel);
	xyPrint(0, "Collected %i junk obects.", garbage);
	sq_pop(gvSquirrel, 1);
	sq_close(gvSquirrel);

	//Close SDL
	xyPrint(0, "Closing SDL...");
	SDL_DestroyRenderer(gvRender);
	SDL_DestroyWindow(gvWindow);
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();

	//Close log file
	xyPrint(0, "System closed successfully!");
	gvLog.close();
};

void xyPrint(HSQUIRRELVM v, const SQChar *s, ...){
	va_list args;
	va_start(args, s);
	SQChar buffer[1024] = _SC("");
	vsnprintf(buffer, sizeof(buffer), s, args);
	va_end(args);

	cout << ">:: " << buffer << endl << endl;
	gvLog << ">:: " << buffer << endl << endl;
};

void xyBindFunc(HSQUIRRELVM v, SQFUNCTION func, const SQChar *key){
	sq_pushroottable(v);
	sq_pushstring(v, key, -1);
	sq_newclosure(v, func, 0);
	sq_newslot(v, -3, false);
	sq_pop(v, 1);
};

void xyBindFunc(HSQUIRRELVM v, SQFUNCTION func, const SQChar *key, SQInteger nParams, const SQChar* sParams){
	sq_pushroottable(v);
	sq_pushstring(v, key, -1);
	sq_newclosure(v, func, 0);
	sq_setparamscheck(v, nParams, sParams);
	sq_newslot(v, -3, false);
	sq_pop(v, 1);
};

void xyBindAllFunctions(HSQUIRRELVM v){
	//Main
	xyPrint(0, "Embedding main...");
	xyBindFunc(v, sqUpdate, "update");
	xyBindFunc(v, sqGetOS, "getOS");
	xyBindFunc(v, sqGetTicks, "getTicks");
	xyBindFunc(v, sqImport, "import", 2, ".s");
	xyBindFunc(v, sqGetFPS, "getFPS");
	xyBindFunc(v, sqSetFPS, "setFPS", 2, ".n");
	xyBindFunc(v, sqSetWindowTitle, "setWindowTitle", 2, ".s");

	//Graphics
	xyPrint(0, "Embedding graphics...");
	xyBindFunc(v, sqWait, "wait", 2, ".n");
	xyBindFunc(v, sqSetDrawTarget, "setDrawTarget", 2, ".n");
	xyBindFunc(v, sqClearScreen, "clearScreen");
	xyBindFunc(v, sqResetDrawTarget, "resetDrawTarget");
	xyBindFunc(v, sqDrawImage, "drawImage", 4, ".nnn");
	xyBindFunc(v, sqSetDrawColor, "setDrawColor", 2, ".n");
	xyBindFunc(v, sqLoadImage, "loadImage", 2, ".s");
	xyBindFunc(v, sqLoadImageKeyed, "loadImageKey", 3, ".sn");
	xyBindFunc(v, sqDrawImage, "drawImage", 4, ".inn");
	xyBindFunc(v, sqSetBackgroundColor, "setBackgroundColor", 2, ".n");
	xyBindFunc(v, sqSetScalingFilter, "setScalingFilter", 2, ".n|b");
	xyBindFunc(v, sqSetResolution, "setResolution", 3, ".nn");

	//Sprites
	xyPrint(0, "Embedding sprites...");
	xyBindFunc(v, sqNewSprite, "newSprite", 9, ".siiiiiii");
	xyBindFunc(v, sqDrawSprite, "drawSprite", 5, ".innn");
	xyBindFunc(v, sqDrawSpriteEx, "drawSpriteEx", 10, ".innnninnn");
	xyBindFunc(v, sqDeleteSprite, "deleteSprite", 2, ".i");

	//Input
	xyPrint(0, "Embedding input...");
	xyBindFunc(v, sqKeyPress, "keyPress", 2, ".n");
	xyBindFunc(v, sqKeyRelease, "keyRelease", 2, ".n");
	xyBindFunc(v, sqKeyDown, "keyDown", 2, ".n");
	xyBindFunc(v, sqMouseX, "mouseX");
	xyBindFunc(v, sqMouseY, "mouseY");
	xyBindFunc(v, sqMouseDown, "mouseDown", 2, ".i");
	xyBindFunc(v, sqMousePress, "mousePress", 2, ".i");
	xyBindFunc(v, sqMouseRelease, "mouseRelease", 2, ".i");
	xyBindFunc(v, sqGetQuit, "getQuit");

	//Maths
	xyPrint(0, "Embedding maths...");
	xyBindFunc(v, sqRandomFloat, "randFloat", 2, ".n");
	xyBindFunc(v, sqRandomInt, "randInt", 2, ".n");
	xyBindFunc(v, sqDistance2, "distance2", 5, ".nnnn");
	xyBindFunc(v, sqWrap, "wrap", 4, ".nnn");
	xyBindFunc(v, sqFloor, "floor", 2, ".n");
	xyBindFunc(v, sqPointAngle, "pointAngle", 5, ".nnnn");

	//Text
	xyPrint(0, "Embedding text...");
	xyBindFunc(v, sqDrawText, "drawText", 8, ".nnsnnnn");
	xyBindFunc(v, sqOpenFont, "openFont", 3, ".sn");
	xyBindFunc(v, sqCloseFont, "closeFont", 2, ".n");

	//File IO
	xyPrint(0, "Embedding file I/O...");
	xyBindFunc(v, sqFileExists, "fileExists", 2, ".s");

	//Audio
	xyPrint(0, "Embedding audio...");
	xyBindFunc(v, sqLoadMusic, "loadMusic", 2, ".s");
	xyBindFunc(v, sqLoadSound, "loadSound", 2, ".s");
	xyBindFunc(v, sqPlaySound, "playSound", 3, ".nn");
	xyBindFunc(v, sqPlayMusic, "playMusic", 3, ".nn");
	xyBindFunc(v, sqDeleteSound, "deleteSound", 2, ".n");
	xyBindFunc(v, sqDeleteMusic, "deleteMusic", 2, ".n");

	//Misc
	xyBindFunc(v, sqEmbedTest, "xygTest");

	//Physics
	xyPrint(0, "Embedding physics...");
	xyBindFunc(v, sqNewSpace, "newSpace");
	xyBindFunc(v, sqDeleteSpace, "deleteSpace");
	xyBindFunc(v, sqSpaceStep, "spaceStep", 3, ".nn");
	xyBindFunc(v, sqSpaceSetGravity, "setSpaceGravity", 4, ".nnn");
	xyBindFunc(v, sqSpaceAddBody, "spaceSetGravity", 3, ".nn");
	xyBindFunc(v, sqNewBody, "newBody", 3, ".nn");
	xyBindFunc(v, sqSpaceAddShape, "spaceAddShape", 3, ".nn");
	xyBindFunc(v, spNewStaticBox, "newStaticBox", 6, ".nnnnn");
	xyBindFunc(v, spShapeSetFriction, "setShapeFriction", 3, ".nn");

};

void xyUpdate(){
	//Update ticks counter for FPS
	gvTickLast = gvTicks;
	gvTicks = SDL_GetTicks();
	float fLength = gvTicks - gvTickLast;

	//Update last button state
	for(int i = 0; i < 5; i++){
		buttonlast[i] = buttonstate[i];
	};

	//Reset event-related globals
	gvQuit = 0;


	//Poll events
	while(SDL_PollEvent(&Event)){
		//Quit
		if(Event.type == SDL_QUIT) gvQuit = 1;

		//Mouse button
		if(Event.type == SDL_MOUSEBUTTONDOWN){
			if(Event.button.button == SDL_BUTTON_LEFT) buttonstate[0] = 1;
			if(Event.button.button == SDL_BUTTON_MIDDLE) buttonstate[1] = 1;
			if(Event.button.button == SDL_BUTTON_RIGHT) buttonstate[2] = 1;
			if(Event.button.button == SDL_BUTTON_X1) buttonstate[3] = 1;
			if(Event.button.button == SDL_BUTTON_X2) buttonstate[4] = 1;
		};

		if(Event.type == SDL_MOUSEBUTTONUP){
			if(Event.button.button == SDL_BUTTON_LEFT) buttonstate[0] = 0;
			if(Event.button.button == SDL_BUTTON_MIDDLE) buttonstate[1] = 0;
			if(Event.button.button == SDL_BUTTON_RIGHT) buttonstate[2] = 0;
			if(Event.button.button == SDL_BUTTON_X1) buttonstate[3] = 0;
			if(Event.button.button == SDL_BUTTON_X2) buttonstate[4] = 0;
		};
	};

	//Update screen
	SDL_RenderPresent(gvRender);
	xySetDrawColor(gvBackColor);
	SDL_RenderClear(gvRender);
	xySetDrawColor(gvDrawColor);

	//Update input
	keylast = keystate;
	SDL_PumpEvents();
	for(int i = 0; i < 322; i++){
		keystate[i] = sdlKeys[i];
	};

	SDL_GetMouseState(&gvMouseX, &gvMouseY);

	//Divide by scale
	float sx, sy;
	SDL_RenderGetScale(gvRender, &sx, &sy);

	if(sx == 0) sx = 1;
	if(sy == 0) sy = 1;

	gvMouseX /= sx;
	gvMouseY /= sy;

	gvFPS = 1000 / fLength;
	//Wait for FPS limit
	//		delay	4294967290	unsigned int
	Uint32 current_time = (static_cast<Uint32>(fLength) / gvMaxFPS);
	Uint32 max_delay = (1000 / gvMaxFPS);
	if (current_time < max_delay) {
		if (gvMaxFPS != 0) SDL_Delay(max_delay - current_time);
	}
	/*while(fLength < 1000 / gvMaxFPS){
		gvTicks = SDL_GetTicks();
		fLength = gvTicks - gvTickLast;
	};*/
	gvFPS = 1000 / fLength;
};

int xyGetOS(){
#ifdef _WIN32
	return 0;
#endif
#ifdef __gnu_linux__
	return 1;
#endif
#ifdef __ANDROID__
	return 2;
#endif
#ifdef __APPLE__
	return 3;
#endif
};
