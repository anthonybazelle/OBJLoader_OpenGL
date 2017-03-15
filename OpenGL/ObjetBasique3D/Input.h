#ifndef DEF_INPUT
#define DEF_INPUT

#include <iostream>

enum ButtonState {
	PRESSED, CLICKED, NONE
};

enum RegisteredKey {
	AVANT, ARRIERE, GAUCHE, DROITE
};

class Scene;

class Input
{

	static Input* currentInstance;
	ButtonState* mouseButtons;
	ButtonState letters[26];
	Scene* scene;
	bool mouseMove = false;
	float mouseX=0, mouseY=0;
	float relMouseX = 0, relMouseY = 0;
	bool cameraCanMove = false;

	static void keyboardCallBack(unsigned char  touche, int x, int y);
	static void keyboardUpCallBack(unsigned char  touche, int x, int y);
	static void mouseCallBack(int button, int state, int x, int y);
	static void mouseMoveCallBack(int x, int y);

public:
	Input(Scene *scene);
	~Input();
	void init();
	bool mouseHasMove();
	float getRelMouseX();
	float getRelMouseY();
	bool isLetterPressed(char letter);
	bool isLetterPressed(RegisteredKey key);
	bool isLetterClicked(char letter);
	bool isMouseButtonPressed(int button);
	bool isMouseButtonClicked(int button);
	void SetMouseMove(bool b);
	void checkKeyboardUpInputs(unsigned char  touche, int x, int y);
	void checkKeyboardInputs(unsigned char  touche, int x, int y);
	void checkMouseClicks(int button, int state, int x, int y);
	void checkMouseMoves(int x, int y);
};

#endif




