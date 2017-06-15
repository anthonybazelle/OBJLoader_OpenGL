#ifndef DEF_INPUT
#define DEF_INPUT

#include <iostream>

enum ButtonState {
	PRESSED, CLICKED, NONE
};

enum RegisteredKey {
	AVANT, ARRIERE, GAUCHE, DROITE
};

enum Transformation {
	TRANSLATION, ROTATION, SCALE, NO_TRANS
};

enum State {
	ENTER_POLYGON, ENTER_WINDOW, DRAW, FILL, COLOR
};

class Scene;

class Input
{

	static Input* currentInstance;
	ButtonState* mouseButtons;
	ButtonState letters[26];
	Scene* scene;
	bool mouseMove;
	float mouseX, mouseY;
	float relMouseX, relMouseY;
	bool cameraCanMove;

	static void keyboardCallBack(unsigned char  touche, int x, int y);
	static void keyboardUpCallBack(unsigned char  touche, int x, int y);
	static void mouseCallBack(int button, int state, int x, int y);
	static void mouseMoveCallBack(int x, int y);

public:
	Input(Scene *scene);
	~Input();
	void init();
	float getRelMouseX();
	float getRelMouseY();
	bool mouseHasMove();
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




