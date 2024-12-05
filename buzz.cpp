
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.

const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionY / boxPixelsY; // Total rows on grid
const int gameColumns = resolutionX / boxPixelsX; // Total columns on grid


// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};


void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
int movePlayer(float player_x, int playerWidth, int playerMovementValue, bool isRight);


int main()
{
	// levels
	const int LEVEL1_REGULAR = 20;
	const int LEVEL2_REGULAR = 15;
	const int LEVEL3_REGULAR = 20;

	const int LEVEL1_FAST = 0;
	const int LEVEL2_FAST = 5;
	const int LEVEL3_FAST = 10;

	const int LEVEL1_HONEYCOMB = 3;
	const int LEVEL2_HONEYCOMB = 9;
	const int LEVEL3_HONEYCOMB = 15;

	// other settings and variables
	int groundY = (gameRows - 2) * boxPixelsY;
	int playerMovementValue = boxPixelsX;

	srand(time(0));

	// Declaring RenderWindow.
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);

	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(Vector2i(500, 200));

	// Initializing Background Music.
	Music bgMusic;
	if (!bgMusic.openFromFile("Music/Music3.ogg")) {
    cout << "Error: Could not load music file!" << endl;
	}
	bgMusic.setVolume(50);
	bgMusic.setLoop(true);
	bgMusic.play();

	// Initializing Player and Player Sprites.
	float player_x = (gameRows / 2) * boxPixelsX;
	// 2 * boxPixels becuase the can is 64 pixels in height
	float player_y = groundY - 2 * (boxPixelsY);


	int playerWidth = 2 * boxPixelsX;
	int playerHeight = 2 * boxPixelsY;
	
	Texture playerTexture;
	Sprite playerSprite;
	playerTexture.loadFromFile("Textures/spray.png");
	playerSprite.setTexture(playerTexture);
	// playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

		// Initializing Bullet and Bullet Sprites
		// Data for bullet / Spray pellet

	float bullet_x = player_x;
	float bullet_y = player_y;
	bool bullet_exists = true;

	Clock bulletClock;
	Texture bulletTexture;
	Sprite bulletSprite;

	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setScale(3, 3);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// The ground on which player moves

	RectangleShape groundRectangle(Vector2f(resolutionX, boxPixelsY * 2));
	groundRectangle.setPosition(0, groundY);
	groundRectangle.setFillColor(Color::Green);

	while (window.isOpen()) {

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				return 0;
			}
			else if (e.type == Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::Right) {
					player_x = movePlayer(player_x, playerWidth, playerMovementValue, true);

				} else if (e.key.code == sf::Keyboard::Left) {
					player_x = movePlayer(player_x, playerWidth, playerMovementValue, false);
				}
			}
		}




		if (bullet_exists == true)
		{
			moveBullet(bullet_y, bullet_exists, bulletClock);
			drawBullet(window, bullet_x, bullet_y, bulletSprite);
		}
		else
		{
			bullet_x = player_x;
			bullet_y = player_y;
		}

		drawPlayer(window, player_x, player_y, playerSprite);
		window.draw(groundRectangle);
		window.display();
		window.clear();
	}
}


int movePlayer(float player_x, int playerWidth, int playerMovementValue, bool isRight) {
	if (isRight) {
		// adding playerWidth so that we get the right border of the sprite
		if (player_x + playerWidth + playerMovementValue > resolutionX) {
			return resolutionX - playerWidth;
		}
		return player_x + playerMovementValue;
	} else {
		// 
		if (player_x - playerMovementValue < 0) {
			return 0;
		}
		return player_x - playerMovementValue;
	}
}

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite) {
	playerSprite.setPosition(player_x, player_y);
	window.draw(playerSprite);
}


void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
	// bullet moves every 20 milliseconds. so if that time hasn't elapsed, the bullet is rendered at the same space
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;

	bulletClock.restart();
	bullet_y -= 10;
	if (bullet_y < -32)
		bullet_exists = false;
}


void drawBullet(sf::RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet_x, bullet_y);
	window.draw(bulletSprite);
}

