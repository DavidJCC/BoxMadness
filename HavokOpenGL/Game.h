#pragma once
#include "BaseGame.h"
#include "DebugPrint.h"
#include <string>
#include "BFont.h"
#include "Timer.h"
#include "HavokInit.h"
#include "Box.h"
#include "Sphere.h"
#include "OGL_Box.h"
#include "OGL_Sphere.h"
#include "Skybox.h"
#include "AudioPlayer.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "random.h"
#include "key_codes.h"

using namespace random;
using namespace std;
using namespace timer;

const float VIEW_ANGLE = 50.0f; // field of view in the Y direction
const float NEAR_CLIPPING = 0.05f; // near clipping distance (5cm)
const float FAR_CLIPPING = 1000.0f; // far clipping distance (10m)

const float ANGLE_LIMIT = 8.0f * HK_REAL_PI/180.0f; //8 degrees limit

const int NUM_BOXES = 4;
const float MOVE_SPD = 10.0f;
const float CAM_MAX = 30.0f;
const float CAM_MIN = 10.0f;
const float MOVE_BY = 2.0f;

const int NUM_LIVES = 3;

const float PLR_INIT_XZ = 7.0f;
const float PLR_INIT_Y = 0.9f;

const float BALL_INIT_Y = 1.0f;

enum StateEnum{ gameSplash = 0, gamePlaying, gameOver };

/**
The class inherits from BaseGame and provides the game data model and the game logic
*/
class Game : public BaseGame{
	// Allow event handler direct access to these attributes
	// (copied from BaseGame since friendship is not inherited)
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	// HERE WE DECLARE ANY FONTS, TIMERS OR OTHER VARIABLES WE WANT
	BFont* font1;
	char text[256];
	Timer* timer;
	int mouseX, mouseY;
	float camX, camY, camZ, camRad;
	float angEW, angNS;
	float cft, lft, tbf, fps, avgFps, ranX, ranZ;
	unsigned int fCount;
	bool updatePhysics, lockCam, gamePaused, moveBack;
	int lives, score, gameState, boxSelected;

	// HERE WE DECLARE ANY GAME OBJECTS AND CREATE THEM IN THE INITIALISE function
	hkpWorld* m_world;		// Havok persistent objects
	#ifdef _DEBUG
		hkVisualDebugger* m_vdb;
		hkpPhysicsContext* m_physicsContext;
	#endif


	Box *pForm, *goal, *box[NUM_BOXES];
	Sphere* ball;
	OGL_Box *oPlatform, *oBox[NUM_BOXES], *oGoal;
	OGL_Sphere *oBall;
	Timer* gameTimer;
	Skybox* skybox;
	AudioPlayer* audioPl;

public:
	Game(void);
	virtual ~Game(void);

	/**
	Use this method to perform any first time initialisation of OpenGL
	*/
	void InitOpenGL();

	/**
	Use this method to create timers, fonts and all game objects. Do NOT 
	execute any OpenGL commands in this method, use initOpenGL instead.
	*/
	void Initialise();

	/**
	Use this method to perform any clean up of objects created for 
	the game - including fonts and timers.
	*/
	void Shutdown();

	/**
	Use this method to update the game's state, but do not use it for rendering
	*/
	void Game::Update();

	/**
	The main rendering method - renders a single frame
	*/
	void Game::Render();

	/**
	Used to alter Camera x, y and z positions
	*/
	void CameraPos();

	/*
	To be called in cameraPos to ensure the radius doesnt exceed a specified amount.
	*/
	void ConstrainCam();

	/*
	Locks the camera in place
	*/
	void CameraLock();

	/**
	Used to display text messages in game display area
	*/
	void RenderHUD();

	/*
	Initialise the physics objects in the workd
	*/
	void InitPhysicsObjects();

	/*
	Check if the game is over
	*/
	void IsGameOver();

	/*
	Resets the boxes, platform, ball, timers, score and lives
	*/
	void Reset();

	/*
	Guess what this function does?
	*/
	void PauseGame();

	/*
	Updates the frames per second
	*/
	void UpdateFps();

	/*
	Resets the ball position after it falls off the platform
	*/
	void ResetBall();

	/*
	Process key inputs
	*/
	void ProcessInput();
	
	/*
	Set move back to true so the boxes can move back
	*/
	void setMoveBack(bool mvBack);

	/*
	Stops the boxes from moving
	*/
	void StopMotion();
};
