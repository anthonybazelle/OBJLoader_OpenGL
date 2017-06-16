#include "Input.h"
#include "Scene.h"

Input* Input::currentInstance = nullptr;

void Input::keyboardCallBack(unsigned char touche, int x, int y)
{
	Input::currentInstance->checkKeyboardInputs(touche, x, y);
}

void Input::keyboardUpCallBack(unsigned char touche, int x, int y)
{
	Input::currentInstance->checkKeyboardUpInputs(touche, x, y);
}

void Input::mouseCallBack(int button, int state, int x, int y)
{
	Input::currentInstance->checkMouseClicks(button, state, x, y);
}

void Input::mouseMoveCallBack(int x, int y)
{
	Input::currentInstance->checkMouseMoves(x, y);
}

Input::Input(Scene *s)
{
	Input::currentInstance = this;
	scene = s;
	mouseX=0, mouseY=0;
	mouseMove = false;
	relMouseX = 0, relMouseY = 0;
	cameraCanMove = true;
	for (int i = 0; i < 26; i++)
	{
		letters[i] = NONE;
	}

	mouseButtons = new ButtonState[3];
	for (int i = 0; i < 3; i++)
		mouseButtons[i] = NONE;
}


Input::~Input()
{
}

void Input::init()
{
	glutKeyboardFunc(Input::keyboardCallBack);
	glutKeyboardUpFunc(Input::keyboardUpCallBack);
	glutMouseFunc(Input::mouseCallBack);
	glutPassiveMotionFunc(Input::mouseMoveCallBack);

	relMouseX = relMouseY = 0;
	mouseX = scene->getWidth()/2;
	mouseY = scene->getHeight()/2;
	mouseMove = true;

}

bool Input::mouseHasMove()
{
	return mouseMove;
}

void Input::SetMouseMove(bool b)
{
	mouseMove=b;
}

float Input::getRelMouseX()
{
	return relMouseX;
}

float Input::getRelMouseY()
{
	return relMouseY;
}


bool Input::isLetterPressed(char letter)
{
	int nb = letter - (int)'a';
	if(nb<0 || nb>25)
	return false;
	return (letters[nb] == PRESSED);
}

bool Input::isLetterPressed(RegisteredKey key)
{
	/*int nb = letter - (int)'A';
	if (nb<0 || nb>25)
		return false;
	return (letters[nb] == PRESSED);*/
	return false;
}

bool Input::isLetterClicked(char letter)
{
	int nb = letter - (int)'a';
	if (nb<0 || nb>25)
		return false;
	return (letters[nb] == CLICKED);
}

bool Input::isMouseButtonPressed(int button)
{
	if (button<0 || button>2)
		return false;
	return (mouseButtons[button] == PRESSED);
}

bool Input::isMouseButtonClicked(int button)
{
	if (button<0 || button>2)
		return false;
	return (mouseButtons[button] == CLICKED);
}

void Input::checkKeyboardInputs(unsigned char  touche, int x, int y)
{
	int nb = touche - (int)'a';
	if (nb<0 || nb>25)
		return ;
	letters[nb] = PRESSED;

	switch (touche)
	{
	case 'a':
		std::cout << "change state : ENTER_POINTS" << std::endl;
		scene->changeState(ENTER_POLYGON);
		glutPostRedisplay();
		break;
	case 'e':
		std::cout << "change state : DRAW" << std::endl;
		scene->changeState(DRAW);
		glutPostRedisplay();
		break;
	case 'w':
		std::cout << "ACTIVATE TRANSLATION" << std::endl;
		scene->changeActiveTransformation(TRANSLATION);
		glutPostRedisplay();
		break;
	case 'x':
		std::cout << "ACTIVATE ROTATION" << std::endl;
		scene->changeActiveTransformation(ROTATION);
		glutPostRedisplay();
		break;
	case 'c':
		std::cout << "ACTIVATE SCALE" << std::endl;
		scene->changeActiveTransformation(SCALE);
		glutPostRedisplay();
		break;
	case 'u':
		std::cout << "DRAW SIMPLE EXTRUDE" << std::endl;
		scene->changeIntermediateState(EXTRUDE);
		glutPostRedisplay();
		break;
	case 'r':
		std::cout << "DRAW REVOLUTION EXTRUDE" << std::endl;
		scene->changeIntermediateState(REVOLUTION);
		glutPostRedisplay();
		break;
	case 'g':
		std::cout << "DRAW GENERALIZE EXTRUDE" << std::endl;
		scene->changeIntermediateState(GENERALIZE);
		glutPostRedisplay();
		break;
	case 'v':
		std::cout << "UNATIVE ALL" << std::endl;
		scene->changeActiveTransformation(NO_TRANS);
		glutPostRedisplay();
		break;
	case 'z':
		scene->applyTransformation(touche);
		glutPostRedisplay();
		break;
	case 'q':
		scene->applyTransformation(touche);
		glutPostRedisplay();
		break;
	case 's':
		scene->applyTransformation(touche);
		glutPostRedisplay();
		break;
	case 'd':
		scene->applyTransformation(touche);
		glutPostRedisplay();
		break;
	case 'l':
		std::cout << "link" << std::endl;
		scene->linkOtherCurve();
		glutPostRedisplay();
		break;
	case '+':
		scene->changeBezierRecursion(1);
		glutPostRedisplay();
		break;
	case '-':
		scene->changeBezierRecursion(-1);
		glutPostRedisplay();
		break;
	default:
		break;
	}
	
}

void Input::checkKeyboardUpInputs(unsigned char  touche, int x, int y)
{
	int nb = touche - (int)'a';
	if (nb<0 || nb>25)
		return;
	if(letters[nb] == CLICKED)
		letters[nb] = NONE;
	else
		letters[nb] = CLICKED;

	/*if (touche == 'w')
	{
		scene->getCamera()->setState(FREE);
		std::cout << "Etat camera : '" << "FREE" << "'" << std::endl;
	}
		
	else if (touche == 'x')
	{
		scene->getCamera()->setState(FPS);
		std::cout << "Etat camera : '" << "FPS" << "'" << std::endl;
	}
	else if (touche == 'c')
	{
		scene->getCamera()->setState(ORBITALE);
		std::cout << "Etat camera : '" << "ORBITALE" << "'" << std::endl;
	}*/

		

}

void Input::checkMouseClicks(int button, int state, int x, int y)
{
	int width = scene->getWidth();
	int height = scene->getHeight();
	x -= width / 2;
	y -= height / 2;
	x = -x;
	y = -y;
	State sceneState = scene->getState();
	if (sceneState == DRAW && button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseButtons[0] = PRESSED;
			if (scene->hasSelectedPoint())
			{
				scene->unselectPoint();
			}
			else
			{
				if (scene->isPointSelected(x, y))
				{
					std::cout << "point selected" << std::endl;
				}
			}


			glutPostRedisplay();
			std::cout << "mouseButton pressed" << std::endl;
		}
		else
		{
			mouseButtons[0] = CLICKED;
			glutPostRedisplay();
			std::cout << "mouseButton up" << std::endl;
		}

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (sceneState == ENTER_POLYGON || sceneState == ENTER_WINDOW)) {
		maths::Point p;
		std::cout << "clic x=" << x << "  y=" << y << endl;
		p.x = x;
		p.y = y;
		p.z = 0;
		scene->addPoint(p);
		//onMouse = 1;
		glutPostRedisplay();
	}

	/*if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && scene->getState() == DRAW)
	{
	maths::Point p;
	p.x = x;
	p.y = y;

	scene->cursorInPolygon(p);
	}*/
}

void Input::checkMouseMoves(int x, int y)
{
	if (cameraCanMove)
	{
		mouseMove = true;
		relMouseX = x - mouseX;
		relMouseY = y - mouseY;
		mouseX = x;
		mouseY = y;
	}

	mouseMove = true;

	float width = scene->getWidth();
	float height = scene->getHeight();

	x -= width / 2;
	y -= height / 2;
	x = -x;
	y = -y;

	mouseX = x;
	mouseY = y;

	if (scene->hasSelectedPoint())
		scene->moveSelectedPoint(mouseX, mouseY);
	
}
