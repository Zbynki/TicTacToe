#include "SFML/Graphics.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <iostream>
#include <string.h>
#include <math.h>
#include <time.h>

using namespace std;

void mouseClickEvents(sf::Event event);

void mouseClickEventsMenu(sf::Event event);
void mouseClickEventsGame(sf::Event event);

void renderMenu(sf::RenderWindow* window, sf::Vector2i mouseLoc);
void renderGame(sf::RenderWindow* window, sf::Vector2i mouseLoc);

// font credit: https://www.dafont.com/upheaval.font

// https://www.youtube.com/watch?v=dQw4w9WgXcQ

// textures loading

void loadAssets();

void initMenuButtons();
void initGameButtons();

/*
** Util structs/functions
*/

struct Button
{
	float width;
	float height;
	float x;
	float y;
	sf::Sprite* sprite;
	sf::Sprite* overSprite;

	Button(float _x, float _y, float _width, float _height, sf::Texture* texture, sf::Texture* overTexture) : x(_x), y(_y), width(_width), height(_height)
	{

		sprite = new sf::Sprite(*texture);
		(*sprite).setPosition(x, y);

		overSprite = new sf::Sprite(*overTexture);
		(*overSprite).setPosition(x, y);
	}

	void render(sf::RenderWindow* window, sf::Vector2i mouseLoc)
	{
		if (isMouseOver(mouseLoc))
			(*window).draw(*overSprite);
		else
			(*window).draw(*sprite);
	}

	bool isMouseOver(sf::Vector2i mouseLoc)
	{
		int mx = mouseLoc.x;
		int my = mouseLoc.y;

		return (mx >= x && mx <= width + x) && (my >= y && my <= height + y);

	}
	bool isMouseOver(sf::Event event)
	{
		int mx = event.mouseButton.x;
		int my = event.mouseButton.y;

		return (mx >= x && mx <= width + x) && (my >= y && my <= height + y);
	}
};

bool inMenu = true;
bool shouldCloseWindow = false;
bool pauseMenu = false;
bool win = false;
bool draw = false;

#define TURN_O true
#define TURN_X false
#define CELL_EMPTY 0
#define CELL_O 1
#define CELL_X 2

bool turn = TURN_O;
char cells[20][16];

int moveCount = 0;

int main()
{	
	sf::RenderWindow window(sf::VideoMode(1140, 640), "TicTacToe", sf::Style::Titlebar | sf::Style::Close);
	loadAssets();
	initMenuButtons();
	initGameButtons();

	sf::Image icon;
	icon.loadFromFile("assets/image.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	cout << "Steve The Greatest was here." << endl;
	sf::Event event;
	
	// Main window loop

	while (window.isOpen())
	{
		// Window events

		while (window.pollEvent(event))
		{
			// Close event
			if (event.type == sf::Event::Closed || shouldCloseWindow)
				window.close();

			// Mouse click events
			if (event.type == sf::Event::MouseButtonPressed)
				mouseClickEvents(event);
		}

		sf::Vector2i localPosition = sf::Mouse::getPosition(window);

		// Rendering menu, game
		// Background color

		window.clear(sf::Color(255, 255, 255, 255));
		if (inMenu)
			renderMenu(&window, localPosition);
		else
			renderGame(&window, localPosition);
		window.display();
	}
	return 0;
}

/*
** Textures
*/

sf::Texture playButtonTexture;
sf::Texture overPlayButtonTexture;
sf::Texture exitButtonTexture;
sf::Texture overExitButtonTexture;
sf::Texture titleTexture;
sf::Texture versionTexture;
sf::Texture titleATexture;
sf::Texture emptyTexture;

sf::Texture fieldTexture;
sf::Texture circleTexture;
sf::Texture crossTexture;
sf::Texture hoverTexture;
sf::Texture pauseButtonTexture;
sf::Texture overPauseButtonTexture;
sf::Texture restartButtonTexture;
sf::Texture overRestartButtonTexture;
sf::Texture fieldBorderTexture;
sf::Texture oTurnTexture;
sf::Texture xTurnTexture;

sf::Texture pauseMenuTexture;
sf::Texture yesButtonTexture;
sf::Texture overYesButtonTexture;
sf::Texture noButtonTexture;
sf::Texture overNoButtonTexture;

sf::Texture oWinTexture;
sf::Texture xWinTexture;
sf::Texture tieTexture;
sf::Texture playAgainButtonTexture;
sf::Texture overPlayAgainButtonTexture;

void loadAssets()
{
	playButtonTexture.loadFromFile("assets/playButton.png");
	overPlayButtonTexture.loadFromFile("assets/overPlayButton.png");
	exitButtonTexture.loadFromFile("assets/exitButton.png");
	overExitButtonTexture.loadFromFile("assets/overExitButton.png");
	titleTexture.loadFromFile("assets/title.png");
	versionTexture.loadFromFile("assets/version.png");
	emptyTexture.loadFromFile("assets/empty.png");
	titleATexture.loadFromFile("assets/titleA.png");

	fieldTexture.loadFromFile("assets/field.png");
	circleTexture.loadFromFile("assets/circle.png");
	crossTexture.loadFromFile("assets/cross.png");
	hoverTexture.loadFromFile("assets/hover.png");
	pauseButtonTexture.loadFromFile("assets/pauseButton.png");
	overPauseButtonTexture.loadFromFile("assets/overPauseButton.png");
	fieldBorderTexture.loadFromFile("assets/fieldBorder.png");
	oTurnTexture.loadFromFile("assets/oTurn.png");
	xTurnTexture.loadFromFile("assets/xTurn.png");

	pauseMenuTexture.loadFromFile("assets/pauseMenu.png");
	yesButtonTexture.loadFromFile("assets/yesButton.png");
	overYesButtonTexture.loadFromFile("assets/overYesButton.png");
	noButtonTexture.loadFromFile("assets/noButton.png");
	overNoButtonTexture.loadFromFile("assets/overNoButton.png");
	restartButtonTexture.loadFromFile("assets/restartButton.png");
	overRestartButtonTexture.loadFromFile("assets/overRestartButton.png");
	oWinTexture.loadFromFile("assets/oWin.png");
	xWinTexture.loadFromFile("assets/xWin.png");
	tieTexture.loadFromFile("assets/tie.png");
	playAgainButtonTexture.loadFromFile("assets/playAgainButton.png");
	overPlayAgainButtonTexture.loadFromFile("assets/overPlayAgainButton.png");
}


/*
** Main handler for mouse click events
*/

void mouseClickEvents(sf::Event event)
{
	// left button only

	if (event.mouseButton.button == sf::Mouse::Left)
	{

#ifdef _DEBUG
		cout << "The left button was pressed" << endl;
		cout << "mouse x: " << event.mouseButton.x << endl;
		cout << "mouse y: " << event.mouseButton.y << endl;
#endif // _DEBUG

		// Menu and Game separate click event handlers
		if (inMenu)
			mouseClickEventsMenu(event);
		else
			mouseClickEventsGame(event);
	}
}

/*
** Menu
*/

// Menu buttons

Button *playButton;
Button *exitButton;
Button *titleA;

void initMenuButtons()
{
	playButton = new Button(370, 200, 400, 150, &playButtonTexture, &overPlayButtonTexture);
	exitButton = new Button(370, 400, 400, 150, &exitButtonTexture, &overExitButtonTexture);
	titleA = new Button(538, 88, 64, 59, &emptyTexture , &titleATexture);
}

//Menu click event handler

void mouseClickEventsMenu(sf::Event event)
{
	if ((*playButton).isMouseOver(event))
	{

#ifdef _DEBUG
		cout << "Pressed a button: " << endl;
#endif // _DEBUG

		inMenu = false;
	}

	if ((*exitButton).isMouseOver(event))
	{

#ifdef _DEBUG
		cout << "Pressed a button: " << endl;
#endif // _DEBUG

		shouldCloseWindow = true;
	}
}

// Menu rendering

void renderMenu(sf::RenderWindow* window, sf::Vector2i mouseLoc)
{
	(*playButton).render(window, mouseLoc);
	(*exitButton).render(window, mouseLoc);
	
	static sf::Sprite title(titleTexture);
	title.setPosition(269, 80);
	(*window).draw(title);

	static sf::Sprite version(versionTexture);
	version.setPosition(21, 617);
	(*window).draw(version);

	(*titleA).render(window, mouseLoc);
}

/*
** Game
*/

// Game buttons

Button* pauseButton;
Button* yesButton;
Button* noButton;
Button* restartButton;
Button* playAgainButton;

void initGameButtons()
{
	pauseButton = new Button(822, 57, 239, 86, &pauseButtonTexture, &overPauseButtonTexture);
	yesButton = new Button(292, 281, 261, 118, &yesButtonTexture, &overYesButtonTexture);
	noButton = new Button(621, 281, 261, 118, &noButtonTexture, &overNoButtonTexture);
	restartButton = new Button(292, 421, 590, 118, &restartButtonTexture, &overRestartButtonTexture);
	playAgainButton = new Button(825, 454, 236, 129, &playAgainButtonTexture, &overPlayAgainButtonTexture);
}

// Game click event handler

void mouseClickEventsGame(sf::Event event)
{
	int cellX = ((event.mouseButton.x + 8) >> 5) - 3;
	int cellY = (event.mouseButton.y >> 5) - 2;

	if (pauseMenu == false && cellX >= 0 && cellX <= 19 && cellY >= 0 && cellY <= 15 && win == false && draw == false)
	{
		
#ifdef _DEBUG
			cout << "The left button was pressed" << endl;
			cout << "cell x: " << cellX << endl;
			cout << "cell y: " << cellY << endl;
#endif // _DEBUG

		// Ternary operator
		// Kdo je na tahu, jestli vyhral
		char turnCh = turn == TURN_O ? CELL_O : CELL_X;
		
		if (cells[cellX][cellY] == CELL_EMPTY)
		{
			if (turn == TURN_O)
			{
				cells[cellX][cellY] = CELL_O;
			}

			if (turn == TURN_X)
			{
				cells[cellX][cellY] = CELL_X;
			}

			//switches turn

			turn = !turn;
			moveCount++;
		}

		//row

		int inLine = 0;
		for (int i =  cellX - 4; i < cellX + 5; i++)
		{
			if (i < 0 || i > 20)
				continue;

			if (cells[i][cellY] != turnCh)
				inLine = 0;
			else
				inLine++;

			if (inLine >= 5)
			{
				win = true;
				break;
			}
		}

		//collum

		inLine = 0;
		for (int i = cellY - 4; i < cellY + 5; i++)
		{
			if (i < 0 || i > 16)
				continue;

			if (cells[cellX][i] != turnCh)
				inLine = 0;
			else
				inLine++;

			if (inLine>= 5)
			{
				win = true;
				break;
			}
		}
		
		//diagonal

		inLine = 0;
		for (int i = -4; i < 5; i++)
		{
			if (i + cellX < 0 || i + cellX > 20)
				continue;

			if (i + cellY < 0 || i + cellY > 16)
				continue;

			if (cells[i + cellX][i + cellY] != turnCh)
				inLine = 0;
			else
				inLine++;

			if (inLine >= 5)
			{
				win = true;
				break;
			}
		}

		//diagonal2

		inLine = 0;
		for (int i = -4; i < 5; i++)
		{
			if (i + cellX < 0 || i + cellX > 20)
				continue;

			if (cellY - i < 0 || cellY - i > 16)
				continue;

			if (cells[i + cellX][cellY - i] != turnCh)
				inLine = 0;
			else
				inLine++;

			if (inLine >= 5)
			{
				win = true;
				break;
			}
		}

		//check draw

		if (moveCount == 20*16)
		{
			draw = true;
		}
	}

	if ((*pauseButton).isMouseOver(event))
	{

#ifdef _DEBUG
		cout << "Pressed a button: " << endl;
#endif // _

		pauseMenu = true;
	}

	if (pauseMenu)
	{
		if ((*yesButton).isMouseOver(event))
		{

#ifdef _DEBUG
			cout << "Pressed a button: " << endl;
#endif // _DEBUG

			pauseMenu = false;
			inMenu = true;
			moveCount = 0;
			win = false;
			draw = false;
			turn = TURN_O;

			for (int i = 0; i < 20; i++)
			{

#ifdef _DEBUG
				cout << "i: " << i << endl;
#endif // _DEBUG

				for (int j = 0; j < 16; j++)
				{
					cells[i][j] = CELL_EMPTY;
				}
			}
		}

		if ((*noButton).isMouseOver(event))
		{

#ifdef _DEBUG
			cout << "Pressed a button: " << endl;
#endif // _DEBUG

			pauseMenu = false;
		}

		if ((*restartButton).isMouseOver(event))
		{

#ifdef _DEBUG
			cout << "Pressed a button: " << endl;
#endif // _DEBUG

			pauseMenu = false;
			moveCount = 0;
			win = false;
			draw = false;
			turn = TURN_O;

			for (int i = 0; i < 20; i++)
			{

#ifdef _DEBUG
				cout << "i: " << i << endl;
#endif // _DEBUG

				for (int j = 0; j < 16; j++)
				{
					cells[i][j] = CELL_EMPTY;
				}
			}
		}
	}
	if ((*playAgainButton).isMouseOver(event))
	{

#ifdef _DEBUG
		cout << "Pressed a button: " << endl;
#endif // _DEBUG

		moveCount = 0;
		win = false;
		draw = false;
		turn = TURN_O;

		for (int i = 0; i < 20; i++)
		{

#ifdef _DEBUG
			cout << "i: " << i << endl;
#endif // _DEBUG

			for (int j = 0; j < 16; j++)
			{
				cells[i][j] = CELL_EMPTY;
			}
		}
	}
}

// Game rendering

void renderGame(sf::RenderWindow* window, sf::Vector2i mouseLoc)
{
	int cellX = ((mouseLoc.x + 8) >> 5)-3;
	int cellY = (mouseLoc.y >> 5)-2;

	static sf::Sprite field(fieldTexture);
	field.setPosition(0, 0);
	(*window).draw(field);

	if (!pauseMenu && cellX >= 0 && cellX <= 19 && cellY >= 0 && cellY <= 15)
	{
		static sf::Sprite overCell(hoverTexture);
		overCell.setPosition(cellX * 32 + 89, cellY * 32 + 65);
		(*window).draw(overCell);
	}

	static sf::Sprite fieldBorder(fieldBorderTexture);
	fieldBorder.setPosition(0, 0);
	(*window).draw(fieldBorder);
	(*pauseButton).render(window, mouseLoc);

	static sf::Sprite circle(circleTexture);
	static sf::Sprite cross(crossTexture);

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			

			if (cells[i][j] == CELL_O)
			{
				circle.setPosition(89 + i * 32, 65 + j * 32);
				(*window).draw(circle);
			}

			if (cells[i][j] == CELL_X)
			{
				cross.setPosition(89 + i * 32, 65 + j * 32);
				(*window).draw(cross);
			}
		}
	}

	if (turn == TURN_X)
	{
		static sf::Sprite xTurn(xTurnTexture);
		xTurn.setPosition(879, 274);
		(*window).draw(xTurn);
	}
	else
	{
		static sf::Sprite oTurn(oTurnTexture);
		oTurn.setPosition(879, 274);
		(*window).draw(oTurn);
	}

	// Game end screen

	// Ternary operator (kdo je na tahu)
	char turnCh = !turn == TURN_O ? CELL_O : CELL_X;

#ifdef _DEBUG
	cout << "turn " << turnCh << "win: " << win << endl;
#endif // _DEBUG

	// Kdo vyhral
	if (win == true && turnCh == CELL_O)
	{
		static sf::Sprite oWin(oWinTexture);
		oWin.setPosition(738, 124);
		(*window).draw(oWin);

		(*playAgainButton).render(window, mouseLoc);
	}

	if (win == true && turnCh == CELL_X)
	{
		static sf::Sprite xWin(xWinTexture);
		xWin.setPosition(738, 124);
		(*window).draw(xWin);

		(*playAgainButton).render(window, mouseLoc);
	}

	if (draw == true)
	{
		static sf::Sprite tie(tieTexture);
		tie.setPosition(738, 146);
		(*window).draw(tie);

		(*playAgainButton).render(window, mouseLoc);
	}

	// Pause menu
	if (pauseMenu)
	{
		static sf::Sprite pauseMenu(pauseMenuTexture);
		pauseMenu.setPosition(0, 0);
		(*window).draw(pauseMenu);

		(*yesButton).render(window, mouseLoc);
		(*noButton).render(window, mouseLoc);
		(*restartButton).render(window, mouseLoc);
	}
}