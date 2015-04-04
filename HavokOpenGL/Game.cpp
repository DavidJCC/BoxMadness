#include "Game.h"

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::InitOpenGL()
{
	DebugOut("Game::InitOpenGL being called");
	Set2D();
	glEnable(GL_POLYGON_SMOOTH);  //set best smoothing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void Game::Initialise()
{
	DebugOut("Game::Initialise being called");

	//setup attributes, timers and fonts
	gameState = gameSplash;
	mouseX = mouseY = 0;
	camX = 0.0f;
	camY = 1.0f;
	camZ = 1.0f;
	camRad = 20.0f;
	fCount = 0;
	lives = NUM_LIVES;
	score = 0;
	lockCam = moveBack = false;
	gamePaused = updatePhysics = true;
	ranX = rnd.number(-3.0f, 3.0f); ranZ = rnd.number(-3.0f, 3.0f); //setting random x and z for ball
	boxSelected = 0;
	timer = new Timer();
	font1 = new BFont(hDC, "courier", 16);

	//initialise physics objects
	InitPhysicsObjects();

	//init skybox and load texture
	skybox = new Skybox();
	skybox->loadTextures();

	//init platform
	oPlatform = new OGL_Box(pForm, "images/platTex.bmp");
	oPlatform->setRGB(0.3f, 0.3f, 0.3f);

	//init oBall
	oBall = new OGL_Sphere(ball, "images/skybox.bmp");
	oBall->setRGB(1.0f, 1.0f, 0.5);

	for(int i = 0; i < NUM_BOXES; i++)
	{
		oBox[i] = new OGL_Box(box[i], "images/skybox.bmp");
		oBox[i]->setRGB(1.0f, 1.0f, 1.0f);
	}

	oGoal = new OGL_Box(goal, "images/skybox.bmp");
	oGoal->setRGB(0.0f, 0.5f, 0.0f);

	//init audio
	string BACKGROUND_LOOP = "sounds/background.wav";
	audioPl = new AudioPlayer(BACKGROUND_LOOP);
	audioPl->playSound();

	//sets the mouse cursor to the center of the game window when started
	POINT p;
	p.x = m_width/2; p.y = m_height/2;
	ClientToScreen(hWnd, &p);
	SetCursorPos(p.x, p.y);

	//hides the mouse cursor
	ShowCursor(false);
}

void Game::Shutdown()
{
	DebugOut("Game::Shutdown being called");
	delete timer;
	delete font1;
	delete skybox;
	delete oPlatform;;
	delete oBall;
	for(int i = 0; i < NUM_BOXES; i++)
		delete oBox[i];
	delete audioPl;
	delete oGoal;
}

void Game::Update()
{
	if(gameState == gamePlaying)
	{
		if(updatePhysics)
		{
			m_world->stepDeltaTime(tbf); //update physics engine
			#ifdef _DEBUG
				m_vdb->step(tbf);	// update VDB when running
			#endif
		}

		if(oBall->getHavokObj()->getPos().y < -15.0)//to stop error
		{
			ResetBall();
			PauseGame();

			if(lives < 1) 
			{
				PauseGame();
				gameState = gameOver;
			}
		}

		//update the objects
		oPlatform->update();
		oBall->update();
		oGoal->update();
		for(int i = 0; i < NUM_BOXES; i++)
		{
			oBox[i]->update();
			box[i]->update(tbf);
			
			if(box[i]->getPos().y < -15.0)
			{
				PauseGame();
				lives = 0;
			}
		}

		//really messy collision detection
		if (oBall->getHavokObj()->getPos().x >= oGoal->getHavokObj()->getPos().x -1.0f &&
			oBall->getHavokObj()->getPos().x <= oGoal->getHavokObj()->getPos().x +1.0f &&
			oBall->getHavokObj()->getPos().z >= oGoal->getHavokObj()->getPos().z -1.0f &&
			oBall->getHavokObj()->getPos().z <= oGoal->getHavokObj()->getPos().z +1.0f)
		{
			if(lives > 0)
			{
				if(lives == 1)
					score += 200;
				else if(lives == 2)
					score += 300;
				else if(lives == 3)
					score += 400;
			}

			gameState = gameOver;
		}

		CameraPos();
	}

	ProcessInput();
	UpdateFps();
	IsGameOver();
}

void Game::RenderHUD()
{
	Set2D(0, m_width, m_height, 0); // Change to 2D view and use 1:1 pixel resolution with [0,0] origin being at the top-left corner.
	glDisable(GL_DEPTH_TEST);  // Disable depth testing so the HUD will not be hidden by the 3D graphics
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	font1->setColor(0.2f, 0.2f, 0.2f);//set font to black

	//screen output
	if(gameState == gameSplash)
	{
		sprintf(text, "----Box Madness!----");
		font1->printString(400, 50, text);
		sprintf(text, "OBJECTIVE: Move the boxes to tilt");
		font1->printString(340, 150, text);
		sprintf(text, " the platform and get the Ball in the goal!");
		font1->printString(290, 180, text);
		sprintf(text, "You have 5 lives, if a box falls off the platform it's automatic game over");
		font1->printString(150, 230, text);
		sprintf(text, "so be careful");
		font1->printString(m_width/2-30, 260, text);
		sprintf(text, "KEYS: W moves box inwards. S moves outwards");
		font1->printString(300, 400, text);
		sprintf(text, "-Press 1, 2, 3 or 4 to switch the controlled box");
		font1->printString(280, 430, text);
		sprintf(text, "-SPACE pauses the game");
		font1->printString(410, 460, text);
		sprintf(text, "-L locks the camera in place");
		font1->printString(390, 490, text);
		sprintf(text, "NUMPAD + and - increases/decreases music volume");
		font1->printString(300, 520, text);
		sprintf(text, "Press SPACE to continue");
		font1->printString(390, 600, text);
	}

	if(gameState == gamePlaying)
	{
		if(gamePaused)
		{
			sprintf(text, "Game paused! Press SPACE to continue");
			font1->printString(m_width/3, 40, text);	
		}
		
		sprintf(text, "%6.1f FPS", avgFps);
		font1->printString(m_width - 115, 15, text);

		if(lockCam) //output camera locked if lockCam is true
		{
			sprintf(text, "Camera locked");	
			font1->printString(650, 15, text);
		}
		sprintf(text, "Score: %i    Lives: %i", score, lives);
		font1->printString(4, 20, text);		
	}

	if(gameState == gameOver)
	{
		if(lives > 0)
		{
			sprintf(text, "Congratulations! You won!");
			font1->printString(m_width/3+20, 50, text);
			sprintf(text, "Your final score: %i", score);	
			font1->printString(m_width/3+20, 70, text);
			sprintf(text, "Press R to restart or ESC to exit");	
			font1->printString(m_width/3-20, 200, text);
		}
		else
		{
			sprintf(text, "GAME OVER!!");	
			font1->printString(m_width/2-80, 50, text);
			sprintf(text, "You suck!");	
			font1->printString(m_width/2-80, 70, text);
			sprintf(text, "Press R to restart or ESC to exit");	
			font1->printString(m_width/3-20, 200, text);
		}
	}

	Set3D(VIEW_ANGLE, NEAR_CLIPPING, FAR_CLIPPING); // Set back to 3D
}

// Render the objects in their current state.
void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skybox->render(20.0f, camX, camY, camZ);
	if(gameState == gamePlaying)
	{
		gluLookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		/******************************
		 Game objects are rendered here
		******************************/
		oPlatform->render();
		oBall->render();
		oGoal->render();
		for(int i = 0; i <  NUM_BOXES; i++)
		{
			oBox[i]->render();
			oBox[boxSelected]->setRGB(1, 0, 0); //sets the selected box rbg to red so the player knows which box they have selected
			if(i != boxSelected)
				oBox[i]->setRGB(0.5, 0.5f, 0.5f); 
		}		
		/******************************
		*******************************/

		fCount++;
	}
	RenderHUD();
}

void Game::CameraPos()
{
	angNS = ((mouseY + 1) / m_height) * (float)M_PI;  // Map the mouse position to two angles
	angEW = (mouseX / m_width) * 2 * (float)M_PI;
	
	float sinNS = sin(angNS);  // Calculate the sines and cosines of these angles
	float cosNS = cos(angNS);
	float sinEW = sin(angEW);
	float cosEW = cos(angEW);
	
	camZ = camRad * sinNS * cosEW;  // calculate the camera coordinate
	camY = camRad * cosNS;
	camX= camRad * sinNS * sinEW;

	ConstrainCam();
}

void Game::InitPhysicsObjects()
{
	#ifdef _DEBUG
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initChecking( hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(2*1024*1024) );
#else
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault( hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(1024*1024) );
#endif
	hkBaseSystem::init( memoryRouter, errorReport);
	hkpWorldCinfo worldInfo;	// Create the simulation world1
	worldInfo.m_gravity.set(0.0f,-9.8f,0.0f);
	worldInfo.setBroadPhaseWorldSize(100.0f);	//
	m_world = new hkpWorld(worldInfo); 
	m_world->lock();	//****
	// This is needed to detect collisionss
	hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
#ifdef _DEBUG
	m_physicsContext = new hkpPhysicsContext();//Connect VDB
	m_physicsContext->addWorld(m_world); // add all worlds
	hkpPhysicsContext::registerAllPhysicsProcesses();
	hkArray<hkProcessContext*> contexts;
	contexts.pushBack(m_physicsContext );  
	m_vdb = new hkVisualDebugger( contexts );
	m_vdb->serve();
#endif

	/************************************
	*********PHYSIC OBJECTS INIT*********
	************************************/

	//platform setup
	pForm = new Box(10.0f, 0.3f, 10.0f);
	pForm->setPos(Vector(0.0f, 0.0f, 0.0f));
	pForm->setDensity(10.0f); //set density	
	
	//boxes setup
	for(int i = 0; i < NUM_BOXES; i++)
	{
		box[i] = new Box(0.5f, 0.5f, 0.5f);
		box[i]->setDensity(1000.0f);
	}

	//boxes setup
	box[0]->setPos(Vector(PLR_INIT_XZ, PLR_INIT_Y, 0.0f));
	box[1]->setPos(Vector(0.0f, PLR_INIT_Y, PLR_INIT_XZ));
	box[2]->setPos(Vector(-PLR_INIT_XZ, PLR_INIT_Y, 0.0f));
	box[3]->setPos(Vector(0.0f, PLR_INIT_Y, -PLR_INIT_XZ));
	
	//goal setup
	goal = new Box(1.0f, 0.001f, 1.0f);
	goal->setPos(Vector(rnd.number(3.0f, 8.0f), pForm->getPos().y + pForm->getSy() + goal->getSy(), rnd.number(3.0f, 8.0f)));
	goal->setDensity(1.0f);
	goal->setFriction(10.0f);

	//ball init
	ball = new Sphere(ranX, BALL_INIT_Y, ranZ); //set the ball to a random x and z value
	ball->setDensity(1.0f);

	//initialise the physics objects in the physics world
	pForm->init(m_world);
	ball->init(m_world);
	goal->init(m_world);
	for(int i = 0; i < NUM_BOXES; i++)
		box[i]->init(m_world);

	hkpGenericConstraintData* data = new hkpGenericConstraintData();	// Create constraint on the level
	hkpConstraintConstructionKit kit;
	kit.begin(data);
	{
		kit.setPivotA(hkVector4(0.0f, 0.0f, 0.0f));	
		kit.setPivotB(hkVector4(0.0f, 0.0f, 0.0f));
		kit.constrainAllLinearDof();
		kit.setAngularBasisABodyFrame();
		kit.setAngularBasisBBodyFrame();

		kit.setAngularLimit(0, -ANGLE_LIMIT, ANGLE_LIMIT);
		// do not limit rotation around Y axis
		kit.setAngularLimit(2, -ANGLE_LIMIT, ANGLE_LIMIT);
	}
	kit.end();

	hkpConstraintInstance* constraint = new hkpConstraintInstance(pForm->getRigidBody(), NULL, data);
	m_world->addConstraint(constraint);
	data->removeReference();
	constraint->removeReference();

	m_world->unlock();
}

void Game::ConstrainCam()
{
	if(camRad < CAM_MIN)
		camRad = CAM_MIN;
	if(camRad > CAM_MAX)
		camRad = CAM_MAX;
}

void Game::CameraLock()
{
	if(lockCam)
		lockCam = false;
	else
		lockCam = true;
}

void Game::IsGameOver()
{
	if(lives == 0)
		gameState = gameOver;

	for(int i = 0; i < NUM_BOXES; i++)
	{
		if(box[i]->getPos().y < -15.0f)
			gameState = gameOver;
	}
}

void Game::Reset()
{
	gameState = gamePlaying; //resets the game state to playing

	//platform reset position, rotation, angular/linear velocities
	pForm->getRigidBody()->setPositionAndRotation(hkVector4(0.0f, 0.0f, 0.0f), hkQuaternion::getIdentity());
	pForm->getRigidBody()->setLinearVelocity(hkVector4(0.0f, 0.0f, 0.0f));
	pForm->getRigidBody()->setAngularVelocity(hkVector4(0.0f, 0.0f, 0.0f));

	//reset the ball pos and linear velocities
	ranX = rnd.number(4.0f); ranZ = rnd.number(4.0f); //setting random x and z for ball
	ball->getRigidBody()->setPosition(hkVector4(ranX, BALL_INIT_Y, ranZ));
	ball->getRigidBody()->setLinearVelocity(hkVector4(0.0f, 0.0f, 0.0f));

	//reset the boxes position and velocities
	box[0]->getRigidBody()->setPositionAndRotation(hkVector4(PLR_INIT_XZ, PLR_INIT_Y, 0.0f), hkQuaternion::getIdentity());
	box[1]->getRigidBody()->setPositionAndRotation(hkVector4(0.0f, PLR_INIT_Y, PLR_INIT_XZ), hkQuaternion::getIdentity());
	box[2]->getRigidBody()->setPositionAndRotation(hkVector4(-PLR_INIT_XZ, PLR_INIT_Y, 0.0f), hkQuaternion::getIdentity());
	box[3]->getRigidBody()->setPositionAndRotation(hkVector4(0.0f, PLR_INIT_Y, -PLR_INIT_XZ), hkQuaternion::getIdentity());
	
	//reset the goal area
	goal->getRigidBody()->setPositionAndRotation(hkVector4(rnd.number(3.0f, 8.0f), pForm->getPos().y + pForm->getSy() + goal->getSy(), rnd.number(3.0f, 8.0f)), hkQuaternion::getIdentity());
	goal->getRigidBody()->setLinearVelocity(hkVector4(0.0f, 0.0f, 0.0f));
	goal->getRigidBody()->setAngularVelocity(hkVector4(0.0f, 0.0f, 0.0f));

	for(int i = 0; i < NUM_BOXES; i++)
	{
		box[i]->getRigidBody()->setLinearVelocity(hkVector4(0.0f, 0.0f, 0.0f));
		box[i]->getRigidBody()->setAngularVelocity(hkVector4(0.0f, 0.0f, 0.0f));
	}
	
	score = 0; //resets the score
	lives = NUM_LIVES; //resets the lives

	Update(); //updates the objects after positions and velocities reset

	PauseGame(); //pauses the game after reset
}

void Game::PauseGame()
{
	if(gameState == gameSplash)
	{
		gameState = gamePlaying;		
		StopMotion();
	}

	if(gameState == gamePlaying)
	{
		if(gamePaused)
			gamePaused = false;
		else
			gamePaused = true;

		if(gamePaused)
			updatePhysics = false;
		else
			updatePhysics = true;
	}
}

void Game::UpdateFps()
{
	cft = timer->getElapsedTime();
	tbf = cft - lft;
	lft = cft;

	fps = 1.0f / tbf;
	avgFps = fCount / cft;
}

void Game::ResetBall()
{
	ball->getRigidBody()->setPosition(hkVector4(ranX, 1.0f, ranZ));
	ball->getRigidBody()->setLinearVelocity(hkVector4(0.0f, 0.0f, 0.0f));
	ball->getRigidBody()->setAngularVelocity(hkVector4(0.0f, 0.0f, 0.0f));
	score -= 50;
	lives--;
}

void Game::ProcessInput()
{
	if( gameState == gamePlaying && !gamePaused )
	{
		if(GetAsyncKeyState(VK_W))
		{
			if(boxSelected == 0)
				box[0]->setVel(-MOVE_BY, pForm->getPos().y, 0);

			if(boxSelected == 1)
				box[1]->setVel(0, pForm->getPos().y, -MOVE_BY);

			if(boxSelected == 2)
				box[2]->setVel(MOVE_BY, pForm->getPos().y, 0);

			if(boxSelected == 3)
				box[3]->setVel(0, pForm->getPos().y, MOVE_BY);
		}
		if(GetAsyncKeyState(VK_S))
		{
			if(boxSelected == 0)
				box[0]->setVel(MOVE_BY, pForm->getPos().y, 0);

			if(boxSelected == 1)
				box[1]->setVel(0, pForm->getPos().y, MOVE_BY);

			if(boxSelected == 2)
				box[2]->setVel(-MOVE_BY, pForm->getPos().y, 0);

			if(boxSelected == 3)
				box[3]->setVel(0, pForm->getPos().y, -MOVE_BY);
		}
	}
}

void Game::StopMotion()
{
	for(int i = 0; i < NUM_BOXES; i++)
	{
		box[i]->setVel(0,0,0);
		box[i]->getRigidBody()->setLinearVelocity(hkVector4(0,0,0));
		box[i]->getRigidBody()->setAngularVelocity(hkVector4(0,0,0));
	}

	goal->getRigidBody()->setLinearVelocity(hkVector4(0.0f, 0.0f, 0.0f));
	goal->getRigidBody()->setAngularVelocity(hkVector4(0.0f, 0.0f, 0.0f));

}
