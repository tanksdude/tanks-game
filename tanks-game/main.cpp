#pragma once
#include <iostream>
#include <vector>
#include <time.h>
#include "colorvalueholder.h"
#include "tank.h"
#include "cannonpoint.h"
#include "wall.h"
#include "bullet.h"

//classes with important handling functions:
#include "collisionhandler.h"
#include "resetthings.h"
#include "bulletpriorityhandler.h"
#include "colormixer.h"
#include "powerfunctionhelper.h"

//levels:
#include "randomlevel.h"
#include "emptylevel.h"

//powers:
#include "inheritedpowercommon.h"
#include "powersquare.h"
#include "speedtankpower.h"
#include "speedbulletpower.h"
#include "speedpower.h"
#include "wallhacktankpower.h"
#include "wallhackbulletpower.h"
#include "wallhackpower.h"
#include "multishottankpower.h"
#include "multishotbulletpower.h"
#include "multishotpower.h"
#include "bouncetankpower.h"
#include "bouncebulletpower.h"
#include "bouncepower.h"

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using namespace std;

unordered_map<unsigned char, bool> normalKeyStates;
unordered_map<int, bool> specialKeyStates;

Tank* tank1 = new Tank(20, 160, 0, 0, "WASD");
Tank* tank2 = new Tank(620, 160, PI, 1, "Arrow Keys");
int tank_dead = 0;

bool leftMouse = false;
bool rightMouse = false;

void doThing() {
	/*
	ColorValueHolder* thing = new ColorValueHolder(255, 255, 255);
	std::cout << thing->getHex() << std::endl;
	delete thing;
	*/
}

int width = 1200;
int height = 600;

//-------------------------------------------------------
// A function to draw the scene
//-------------------------------------------------------
void appDrawScene() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set background color to black
	glClearColor(0, 0, 0, 1);

	//background rectangle
	glColor3f(backColor.getRf(), backColor.getGf(), backColor.getBf());
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(GAME_WIDTH, 0, 0);
	glVertex3f(GAME_WIDTH, GAME_HEIGHT, 0);
	glVertex3f(0, GAME_HEIGHT, 0);
	glEnd();

	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


    
	for (int i = 0; i < powerups.size(); i++) {
		powerups[i]->draw();
	}
	for (int i = 0; i < walls.size(); i++) {
		walls[i]->draw();
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->draw();
	}
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->drawName();
	}
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->draw();
	}
    

	// We have been drawing everything to the back buffer
	// Swap the buffers to see the result of what we drew
	glFlush();
	glutSwapBuffers();
}

//-------------------------------------------------------
// A function to convert window coordinates to scene
// We use it when a mouse event is handled
// Arguments: 	
//	x, y - the coordinates to be updated
//-------------------------------------------------------
void windowToScene(float& x, float& y) {
	x = (2.0f*(x / float(width))) - 1.0f;
	y = 1.0f - (2.0f*(y / float(height)));
}

//-------------------------------------------------------
// A function to handle window resizing
// Called every time the window is resized
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appReshapeFunc(int w, int h) {
	// Window size has changed
	width = w;
	height = h;

	double scale, center;
	double winXmin, winXmax, winYmin, winYmax;

	// Define x-axis and y-axis range
	const double appXmin = 0.0;
	const double appXmax = 640.0; //GAME_WIDTH
	const double appYmin = 0.0;
	const double appYmax = 320.0; //GAME_HEIGHT

	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);

	// Set up the projection matrix using a orthographic projection that will
	// maintain the aspect ratio of the scene no matter the aspect ratio of
	// the window, and also set the min/max coordinates to be the disired ones
	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) {
		scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		center = 0;
		winXmin = center - (center - appXmin)*scale;
		winXmax = center + (appXmax - center)*scale;
		winYmin = appYmin;
		winYmax = appYmax;
	}
	else {
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = 0;
		winYmin = center - (center - appYmin)*scale;
		winYmax = center + (appYmax - center)*scale;
		winXmin = appXmin;
		winXmax = appXmax;
	}

	// Now we use glOrtho to set up our viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}

/*
void appReshapeFunc(int w, int h) {
// Window size has changed
width = w;
height = h;

double scale, center;
double winXmin, winXmax, winYmin, winYmax;

// Define x-axis and y-axis range
const double appXmin = -1.0;
const double appXmax = 1.0;
const double appYmin = -1.0;
const double appYmax = 1.0;

// Define that OpenGL should use the whole window for rendering
glViewport(0, 0, w, h);

// Set up the projection matrix using a orthographic projection that will
// maintain the aspect ratio of the scene no matter the aspect ratio of
// the window, and also set the min/max coordinates to be the disered ones
w = (w == 0) ? 1 : w;
h = (h == 0) ? 1 : h;

if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) {
scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
center = (appXmax + appXmin) / 2;
winXmin = center - (center - appXmin)*scale;
winXmax = center + (appXmax - center)*scale;
winYmin = appYmin;
winYmax = appYmax;
}
else {
scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
center = (appYmax + appYmin) / 2;
winYmin = center - (center - appYmin)*scale;
winYmax = center + (appYmax - center)*scale;
winXmin = appXmin;
winXmax = appXmax;
}

// Now we use glOrtho to set up our viewing frustum
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}
*/


//-------------------------------------------------------
// A function to handle mouse clicks
// Called every time the mouse button goes up or down
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
/*
void appMouseFunc(int b, int s, int x, int y) {
	// Convert from Window to Scene coordinates
	float mx = (float)x;
	float my = (float)y;

	windowToScene(mx, my);
    if (s == 1){
        if (mx > 0){
            if (my > 0){
                cout << "Top-Right" << endl;
            }
            else {
                cout << "Bottom-Right" << endl;
            }
        }
        else{
            if (my > 0){
                cout << "Top-Left" << endl;
            }
            else {
                cout << "Bottom-Left" << endl;
            }
        }
    }
	// Redraw the scene by calling appDrawScene above
	// so that the point we added above will get painted
	//glutPostRedisplay();
}
*/

//-------------------------------------------------------
// A function to handle mouse dragging
// Called every time mouse moves while button held down
// Arguments: 	
//	x, y - current coordinates of the mouse
//-------------------------------------------------------
void appMotionFunc(int x, int y) {
	if (leftMouse) {
		if (!rightMouse) { //tank 1
			tanks[0]->giveX() = (x / float(width)) * GAME_WIDTH;
			tanks[0]->giveY() = (1 - y / float(height)) * GAME_HEIGHT;
		}
		else { //tank 2
			tanks[1]->giveX() = (x / float(width)) * GAME_WIDTH;
			tanks[1]->giveY() = (1 - y / float(height)) * GAME_HEIGHT;
		}
	}

	// Again, we redraw the scene
	//glutPostRedisplay();
}

void mouse_func(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			leftMouse = true;
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			rightMouse = !rightMouse;
		}
	}
	else {
		if (button == GLUT_LEFT_BUTTON) {
			leftMouse = false;
		}
	}
}

//-------------------------------------------------------
// A function to handle keyboard events
// Called every time a key is pressed on the keyboard
// Arguments: 	
//	key  - the key that was pressed
//	x, y - coordinates of the mouse when key is pressed
//-------------------------------------------------------
void keyboard_down(unsigned char key, int x, int y) {
	normalKeyStates[key] = true;
	
    
	// After all the state changes, redraw the scene
	//glutPostRedisplay();
}

void special_keyboard_down(int key, int x, int y) {
	specialKeyStates[key] = true;

	// After all the state changes, redraw the scene
	//glutPostRedisplay();
}

void keyboard_up(unsigned char key, int x, int y) {
	normalKeyStates[key] = false;

	// After all the state changes, redraw the scene
	//glutPostRedisplay();
}

void special_keyboard_up(int key, int x, int y) {
	specialKeyStates[key] = false;

	// After all the state changes, redraw the scene
	//glutPostRedisplay();
}


void tick(int){ //pass in physics rate eventually

	//temporary: need to figure out better implementation
	tanks[0]->forward = normalKeyStates['w'];
	tanks[0]->turnL = normalKeyStates['a'];
	tanks[0]->turnR = normalKeyStates['d'];
	tanks[0]->shooting = normalKeyStates['s'];
	//still temporary
	tanks[1]->forward = specialKeyStates[GLUT_KEY_UP];
	tanks[1]->turnL = specialKeyStates[GLUT_KEY_LEFT];
	tanks[1]->turnR = specialKeyStates[GLUT_KEY_RIGHT];
	tanks[1]->shooting = specialKeyStates[GLUT_KEY_DOWN];

	//each tick portion needs to go in its own separate function; that way a level can override some parts of it without having to rewrite everything

	//move everything
	for(int i = 0; i < tanks.size(); i++){
		tanks[i]->move();
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->move();
	}

	//do the special things
	for (int i = powerups.size() - 1; i >= 0; i--) {
		for (int j = 0; j < tanks.size(); j++) {
			if (CollisionHandler::partiallyCollided(powerups[i], tanks[j])) {
				powerups[i]->givePower(tanks[j]);
				delete powerups[i];
				powerups.erase(powerups.begin() + i);
				break;
			}
		}
	}

	//cout << WallhackPower::getClassColor().getBf() << endl;

	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->powerCalculate();
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->powerCalculate();
	}

	for(int i = 0; i < tanks.size(); i++){
		tanks[i]->shoot();
	}

	//tank to wall collision:
	for (int i = 0; i < tanks.size(); i++) {
		bool modifiedWallCollision = false;
		bool shouldBeKilled = false; //maybe the walls are poison with a certain powerup? I dunno, but gotta have it as an option

		for (int j = 0; j < walls.size(); j++) {
			for (int k = 0; k < tanks[i]->tankPowers.size(); k++) {
				bool (*temp)(Tank*, Wall*) = tanks[i]->tankPowers[k]->modifiedCollisionWithWall;
				if (temp != nullptr) {
					modifiedWallCollision = true;
					bool check_temp = temp(tanks[i], walls[j]);
					if (check_temp) {
						shouldBeKilled = true;
					}
				}
			}

			if (modifiedWallCollision) {
				continue;
			}

			if (CollisionHandler::partiallyCollided(tanks[i], walls[j])) {
				CollisionHandler::pushMovableAwayFromImmovable(tanks[i], walls[j]);
			}
		}

		if (shouldBeKilled) {
			tank_dead = 1; //TODO: proper implementation
		}
	}
	
	//tank collision (temporary? yes because additional tanks):
	if (CollisionHandler::partiallyCollided(tanks[0], tanks[1])) {
		CollisionHandler::pushMovableAwayFromMovable(tanks[0], tanks[1]);
	}

	//bullet to edge collision:
	for (int i = bullets.size() - 1; i >= 0; i--) {
		if (bullets[i]->isFullyOutOfBounds()) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			continue;
		}
	}

	//bullet to wall collision:
	for (int i = bullets.size() - 1; i >= 0; i--) {
		bool modifiedWallCollision = false;
		bool shouldBeDeleted = false;

		for (int j = 0; j < walls.size(); j++) {
			for (int k = 0; k < bullets[i]->bulletPowers.size(); k++) {
				bool (*temp)(Bullet*, Wall*) = bullets[i]->bulletPowers[k]->modifiedCollisionWithWall;
				//cout << temp << " " << (temp==nullptr) << endl;
				if (temp != nullptr) {
					modifiedWallCollision = true;
					//cout << temp(bullets[i], walls[j]) << endl;
					bool check_temp = temp(bullets[i], walls[j]);
					//cout << check_temp << endl;
					if (check_temp) {
						shouldBeDeleted = true;
					}
				}
			}

			if (modifiedWallCollision) {
				continue;
			}

			if (CollisionHandler::partiallyCollided(bullets[i], walls[j])) {
				shouldBeDeleted = true;
				break;
			}
		}

		if (shouldBeDeleted) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			//continue;
		}
	}

	//bullet collision:
	for (int i = bullets.size() - 1; i >= 0; i--) {
		for (int j = bullets.size() - 1; j >= 0; j--) { //could start at i-1?
			if (bullets[i]->getID() == bullets[j]->getID()) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(bullets[i], bullets[j])) {
				//powers aren't fully implemented yet so this doesn't get to shine
				//but they will be soon
				
				Bullet* result = BulletPriorityHandler::determinePriority(bullets[i], bullets[j]); //TODO: returns number, not pointer, because that makes more sense
				if (result == nullptr) {
					Bullet* temp1 = bullets[i];
					Bullet* temp2 = bullets[j];
					if (i > j) {
						bullets.erase(bullets.begin() + i);
						bullets.erase(bullets.begin() + j);
						i--;
					}
					else {
						bullets.erase(bullets.begin() + j);
						bullets.erase(bullets.begin() + i);
					}
					delete temp1;
					delete temp2;
					break;
				} else if (result == junkBullet) {
					//it's a draw, so neither dies
					//continue;
				} else {
					if (bullets[i] == result) { //index = i
						delete bullets[i];
						bullets.erase(bullets.begin() + i);
						break;
					} else { //index = j
						delete bullets[j];
						bullets.erase(bullets.begin() + j);
						continue; //not needed
					}
					//delete result;
				}

				//break;
			}
		}
	}

	//tank to edge collision: (move later) (to where?)
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->edgeConstrain();
	}

	//bullet to tank collision:
	for (int i = 0; i < tanks.size(); i++) {
		for (int j = 0; j < bullets.size(); j++) {
			if (bullets[j]->getID() == tanks[i]->getID()) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(tanks[i], bullets[j])) {
				//dead
				tank_dead = 1;
				//CollisionHandler::pushMovableAwayFromImmovable(tanks[i], bullets[j]);
			}
		}
	}

	//edge constrain tanks again in case bullet decides to move tank
	//don't edge constrain if said tank is dead //fix: implement
	/*
	for (int i = 0; i < tanks.size(); i++) {
		//if (tanks[i]->isOutOfBounds())) {
			tanks[i]->edgeConstrain();
		//}
	}
	*/
    
    glutPostRedisplay();

	if (tank_dead == 0) {
		glutTimerFunc(10, tick, tank_dead);
	} else {
		tank_dead = 0;
		glutTimerFunc(500, ResetThings::reset, 0);
		glutTimerFunc(510, tick, tank_dead);
	}
}
void tick() { tick(tank_dead); }


int main(int argc, char** argv) {

	srand(time(NULL));

	normalKeyStates.insert({ 'w', false });
	normalKeyStates.insert({ 'a', false });
	normalKeyStates.insert({ 's', false });
	normalKeyStates.insert({ 'd', false });
	specialKeyStates.insert({ GLUT_KEY_UP, false });
	specialKeyStates.insert({ GLUT_KEY_LEFT, false });
	specialKeyStates.insert({ GLUT_KEY_RIGHT, false });
	specialKeyStates.insert({ GLUT_KEY_DOWN, false });

	levelLookup.insert({ "random", new RandomLevel()});
	levelLookup.insert({ "empty", new EmptyLevel()});

	powerLookup.insert({ "speed",  SpeedPower::factory });
	powerLookup.insert({ "wallhack",  WallhackPower::factory });
	powerLookup.insert({ "bounce",  BouncePower::factory });
	powerLookup.insert({ "multishot", MultishotPower::factory });

	tanks.push_back(tank1);
	tanks.push_back(tank2);
	
	/*
	for (int i = 0; i < 4; i++) {
		walls.push_back(new Wall(320 - 240*(((3-i)/2) * 2 - 1) - 32*((((3-i)/2) + 1) % 2), i%2 * (320-128), 32, 128, ColorValueHolder(255,0,255)));
	}
	*/

	levelLookup["random"]->initialize();

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	// Setup window position, size, and title
	glutInitWindowPosition(60, 60);
	glutInitWindowSize(width*1.25, height*1.25);
	glutCreateWindow("Tanks Test");

    glPointSize(2);
    
	// Setup some OpenGL options
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_DEPTH_TEST);
    


	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing th window
	glutReshapeFunc(appReshapeFunc);

	// Set callback to handle mouse clicks
	//glutMouseFunc(appMouseFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(appMotionFunc);

	// Set callback to handle keyboard events
	glutKeyboardFunc(keyboard_down);

	//callback for keyboard up events
	glutKeyboardUpFunc(keyboard_up);

	//special keyboard down
	glutSpecialFunc(special_keyboard_down);

	//special keyboard up
	glutSpecialUpFunc(special_keyboard_up);

    // Set callback for the idle function
    //glutIdleFunc(tick);

	//framelimiter
	glutTimerFunc(10, tick, tank_dead);

	//mouse function
	glutMouseFunc(mouse_func);

	// Start the main loop
	glutMainLoop();

}
