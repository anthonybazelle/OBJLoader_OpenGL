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
	cameraCanMove = false;
	for (int i = 0; i < 26; i++)
	{
		letters[i] = NONE;
	}
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

	if (touche == 'w')
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
	}
		

}

void Input::checkMouseClicks(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {

		glutPostRedisplay();
	}

	
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
	
}
