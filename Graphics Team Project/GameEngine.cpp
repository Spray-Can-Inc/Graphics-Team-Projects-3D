/*
*
* Demonstrates how to load and display an Wavefront OBJ file.
* Using triangles and normals as static object. No texture mapping.
*
* OBJ files must be triangulated!!!
* Non triangulated objects wont work!
* You can use Blender to triangulate
*
Look into the main method to change the path to the moel file (shoud have no path) and the model should be at the
same location of this file
*/

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "world.h"
#include "mmsystem.h"
#define KEY_ESCAPE 27

using namespace std;

typedef struct {
	int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

World world("world.setup");
World world2("simpleWorld.setup");
glutWindow win;
Camera cam;
int mouse_x, mouse_y;
bool isFirstUpdate = true;
Object3D* can = world.getObjectByName("Player");
int worldOneCount = 0;
//TextureObj obj("cude.obj3d","sand.bmp");
int worldTwoCount = -1;
void checkColorCollision(const Object3D* obj);
void checkWorld2ColorCollision(const Object3D* obj);

//IITTT WOOOOORRRRKKKKSSSS
//ANOOTHER LINNNEE ITTT WOOOORRKKKS
void bgm(void) {
	PlaySound(TEXT("UpbeatFunk.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

/***************************************************************************
* Program code
Spray can added
***************************************************************************/

void initObjects(){
	//world one
	Object3D* objs = world.getObjects();
	for (int i = 0; i < world.objectCount(); i++) {
		if (strcmp(objs[i].getName(), "no color") == 0) {
			objs[i].setColorRender(false);
		}
		
	}
	world.getObjectByName("ground")->setColorRender(false);
	//world two
	objs = world2.getObjects();
	for (int i = 0; i < world2.objectCount(); i++) {
		if (strcmp(objs[i].getName(), "no color") == 0) {
			objs[i].setColorRender(false);
		}

	}
	world2.getObjectByName("ground")->setColorRender(false);
}


void updateCamera() {
	if (can) {//check for null pointer
		cam.lookAt((*can).xPos, (*can).yPos, (*can).zPos);
		if (worldOneCount < 6) {
			if (cam.distance(*can) > 5) {
				cam.move(.1);
			}
			if (cam.distance(*can) < 3) {
				cam.move(-.1);
			}
			if (cam.yPos < .5) {
				cam.setLocation(cam.xPos, .5, cam.zPos);
			}
		
			if (cam.yPos > .9) {
				cam.setLocation(cam.xPos, .9, cam.zPos);
			}
		}
		else {
			move(-.05);
			cam.yPos += .1;
		}
		if (cam.yPos > 100) {
			worldOneCount = 0;
			worldTwoCount = 0;
			cam.yPos = .9;
			can = world2.getObjectByName("Player");
		}

	}
}

/***************************************************************************
* Game Logic
Now we need to keep the camera's focus on the spray can(Player)
***************************************************************************/
void updateGame(){
	//This code runs once to setup camera
	if (isFirstUpdate){
		cam.setLocation(0, 1, 4);
		cam.lookAt(0, 0, 0);
		isFirstUpdate = false;
	}
	//=======sample code on how to select object out of world=========//
	updateCamera();
	if (worldTwoCount == -1)
		checkColorCollision(can);
	else
		checkWorld2ColorCollision(can);
	
	/*int count = 0;
	Object3D* objects = world.getIntersectingObjects(can,&count);
	for (int i = 0; i < count; i++) {
		if (strcmp(objects[i].getName(), "no color") == 0) {
			
		}
	}
	//delete the array of pointers
	free(objects);*/
	//========== can name file in the .config file using (n) ==========//
	//= objects can be given a name using (*obj).setName("new name"); =//

	glutPostRedisplay(); // must be last line of code
}
void checkWorld2ColorCollision(const Object3D* obj) {
	Object3D* list = world2.getObjects();
	//Object3D* start = list;
	for (int i = 0; i < world2.objectCount(); i++) {
		//if not the same object
		if (obj != &list[i]) {
			bool intersect = list[i].isIntersecting(*obj);
			//if (strcmp(objects[i].getName(), "Player") == 0) {
			if (intersect) {
				if (strcmp(list[i].getName(), "no color") == 0) {
					if (list[i].colorOn == false) {
						list[i].setColorRender(true);
						worldTwoCount++;
					}
					if (worldTwoCount >= 1) {
						world2.getObjectByName("ground")->setColorRender(true);
						//worldOneCount++;
					}
				}
			}
			//}
		}
	}
}
void checkColorCollision(const Object3D* obj) {
	Object3D* list = world.getObjects();
	//Object3D* start = list;
	for (int i = 0; i < world.objectCount(); i++) {
		//if not the same object
		if (obj != &list[i]) {
			bool intersect = list[i].isIntersecting(*obj);
			//if (strcmp(objects[i].getName(), "Player") == 0) {
			if (intersect) {
				if (strcmp(list[i].getName(), "no color") == 0) {
					if (list[i].colorOn == false) {
						list[i].setColorRender(true);
						worldOneCount++;
					}
					if (worldOneCount >= 6) {
						world.getObjectByName("ground")->setColorRender(true);
						worldOneCount++;
					}
				}
			}
			//}
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // camera functionality must be in display method
	cam.Update();//MUST UPDATE CAMERA BEFORE DRAWING OBJECTS
	//obj.Draw();
	if (worldTwoCount == -1)
		world.Draw(); //VERY IMPORTANT
	else
		world2.Draw();
	glutSwapBuffers();
}


void mouseInfo(int button, int state, int x, int y)
{
	mouse_x = x; //place current mouse pos in mouse_x
	mouse_y = y;
}

void mousePosUpdate(int x, int y)
{
	mouse_x = x; //place current mouse pos in mouse_x
	mouse_y = y;
}

void initialize()
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat)win.width / win.height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.2f, 0.2f, 0.8f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.1, 0.1, .1, 1.0 };
	//GLfloat amb_light[] = { 0.7, 0.7, .7, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	//glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	bgm();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case KEY_ESCAPE:
			exit(0);
			break;
		case 'w':
			//(*can).zPos -= 0.05;
			can->move(-.05);
			break;
		case 's':
			//(*can).zPos += 0.05;
			can->move(.05);
			break;
		case 'a':
			//(*can).xPos -= 0.05;
			can->rotY += 5;
			break;
		case 'd':
			//(*can).xPos += 0.05;
			can->rotY -= 5;
			break;
		default:
			break;
	}
}

void SpecialKey(int key, int x, int y)
{

	switch (key) {
	case GLUT_KEY_UP:
		cam.rotate(1, 1,0,0);
		break;
	case GLUT_KEY_DOWN:
		cam.rotate(-1, 1, 0, 0);
		break;
	case GLUT_KEY_LEFT:
		cam.rotate(1, 0, 1, 0);
		break;
	case GLUT_KEY_RIGHT:
		cam.rotate(-1, 0, 1, 0);
		break;
	case GLUT_KEY_PAGE_UP:
		cam.setLocation(cam.xPos, cam.yPos + .1, cam.zPos);
		break;
	case GLUT_KEY_PAGE_DOWN:
		cam.setLocation(cam.xPos, cam.yPos - .1, cam.zPos);
		break;
	}
}

int main(int argc, char **argv)
{
	// set window values
	win.width = 1200;
	win.height = 800;
	win.title = "GameEngine"; //Changed the name back to default for some reason, so I .... changed it back.
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  // Display Mode
	glutInitWindowSize(win.width, win.height);					// set window size
	glutCreateWindow(win.title);								// create Window
	glutDisplayFunc(display);									// register Display Function
	glutIdleFunc(updateGame);									// register Idle Function
	glutKeyboardFunc(keyboard);								// register Keyboard Handler
	glutSpecialFunc(SpecialKey);
	glutPassiveMotionFunc(mousePosUpdate);              //register mouse tracking
	glutMouseFunc(mouseInfo);

	initialize();
	initObjects();
	
	glutMainLoop();												// run GLUT mainloop
	return 0;
}