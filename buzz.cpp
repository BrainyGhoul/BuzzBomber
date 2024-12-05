
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
void moveBees(Sprite bees[], int noOfBees, float beesCoords[][2], bool areBeesMovingRight[], Clock beesClock[], int beeMovementValue, float beeSpeed, int regularBeeWidth);
void moveBee(Sprite &bee, float &x_coordinate, float &y_coordinate, bool &isMovingRight, int beeMovementValue, int beeWidth);
void spawnBees(Clock beesClock[],int &beesSpawned, int totalBees, float delay, float offset);
void drawBees(RenderWindow& window, int noOfBees, Sprite bees[], float beesCoords[][2]);


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

	const float LEVEL1_REGULAR_DELAY = 1;
	const float LEVEL2_REGULAR_DELAY = 1;
	const float LEVEL3_REGULAR_DELAY = 1;

	const float LEVEL1_FAST_DELAY = 1;
	const float LEVEL2_FAST_DELAY = 1;
	const float LEVEL3_FAST_DELAY = 1;

	const float LEVEL1_REGULAR_OFFSET = 1;
	const float LEVEL2_REGULAR_OFFSET = 1;
	const float LEVEL3_REGULAR_OFFSET = 1;

	const float LEVEL1_FAST_OFFSET = 1;
	const float LEVEL2_FAST_OFFSET = 1;
	const float LEVEL3_FAST_OFFSET = 1;

	// speeds. this speed is number of boxPixels per second
	const float regularSpeed = 5;
	const float fastSpeed = 5;

	// other settings and variables
	int groundY = (gameRows - 2) * boxPixelsY;
	int playerMovementValue = boxPixelsX;
	int regularBeeMovementValue = playerMovementValue;

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
	bool bullet_exists = false;

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


	// bees
	int regularBeeWidth = 46;
	int regularBeeHeight = 22;
	int beesSpawned = 0;

	Texture beeTexture;
	Clock regularBeesClock[LEVEL1_REGULAR];
	Sprite regularBees[LEVEL1_REGULAR];
	// column 0: x coordinate, column 1: y coordinage
	float regularBeesCoords[LEVEL1_REGULAR][2] = {};
	bool areRegularMovingRight[LEVEL1_REGULAR] = {};

	beeTexture.loadFromFile("Textures/Regular_bee.png");
	for (int i = 0; i < LEVEL1_REGULAR; i++) {
		regularBees[i].setTexture(beeTexture);
		regularBees[i].setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	}
	// TODO spawn bee at either right or left

	while (window.isOpen()) {

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				return 0;
			}
			else if (e.type == Event::KeyPressed) {
				// movement of the spray can
				if (e.key.code == sf::Keyboard::Right) {
					player_x = movePlayer(player_x, playerWidth, playerMovementValue, true);

				} else if (e.key.code == sf::Keyboard::Left) {
					player_x = movePlayer(player_x, playerWidth, playerMovementValue, false);

				// shoot spray
				} else if (e.key.code == sf::Keyboard::Space) {
					bullet_exists = true;
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

		spawnBees(regularBeesClock, beesSpawned, LEVEL1_REGULAR, LEVEL1_REGULAR_DELAY, LEVEL1_REGULAR_OFFSET);
		drawBees(window, beesSpawned, regularBees, regularBeesCoords);
		moveBees(regularBees, beesSpawned, regularBeesCoords, areRegularMovingRight, regularBeesClock, regularBeeMovementValue, regularSpeed, regularBeeWidth);

		drawPlayer(window, player_x, player_y, playerSprite);
		window.draw(groundRectangle);
		window.display();
		window.clear();
	}
}

// setting the position of the bees at the time of the spawn
void spawnBees(Clock beesClock[], int &beesSpawned, int totalBees, float delay, float offset) {
	bool toSpawn = false;
	// if all the bees have already been spawned
	if (beesSpawned == totalBees) {
		return;
	} else if (beesSpawned == 0) {
		if (beesClock[beesSpawned].getElapsedTime().asSeconds() >= offset) {
			cout << beesClock[beesSpawned].getElapsedTime().asSeconds();
			toSpawn = true;
		}
	} else if (beesClock[beesSpawned - 1].getElapsedTime().asSeconds() >= delay) {
		toSpawn = true;
	}

	if (toSpawn) {
		// starting clock from 0 and incrementing beesSpawned
		beesClock[beesSpawned].restart();
		beesSpawned++;
	}

}

void drawBees(RenderWindow& window, int noOfBees, Sprite bees[], float beesCoords[][2]) {
	for (int i = 0; i < noOfBees; i++) {
		bees[i].setPosition(beesCoords[i][0], beesCoords[i][1]);
		window.draw(bees[i]);
	}
}

void moveBees(Sprite bees[], int noOfBees, float beesCoords[][2], bool areBeesMovingRight[], Clock beesClock[], int beeMovementValue, float beeSpeed, int beeWidth) {
	for (int i = 0; i < noOfBees; i++) {
		// if its the movement time
		if (beesClock[i].getElapsedTime().asSeconds() >= 1 / beeSpeed) {
			moveBee(bees[i], beesCoords[i][0], beesCoords[i][1], areBeesMovingRight[i], beeMovementValue, beeWidth);
			beesClock[i].restart();
		}

	}
}

// TODO check pollination
// TODO IF THE BEE IS AT THE GROUND
void moveBee(Sprite &bee, float &x_coordinate, float &y_coordinate, bool &isMovingRight, int beeMovementValue, int beeWidth) {
	bool directionChanged = false;

	//  if at the left or right boundaries, setting the x coordinates at the boundaries too
	if ((isMovingRight && x_coordinate + beeWidth + beeMovementValue >= resolutionX)) {
		directionChanged = true;
		x_coordinate = resolutionX - beeWidth;

	} else if ((!isMovingRight && x_coordinate - beeMovementValue <= 0)) {
		directionChanged = true;
		x_coordinate = 0;
	}


	if (directionChanged) {
		isMovingRight = !isMovingRight;

		// if not at the first block
		if (x_coordinate || y_coordinate){
			y_coordinate += boxPixelsY;
		}

	} else {
		if (isMovingRight) {
			x_coordinate += beeMovementValue;
		} else {
			x_coordinate -= beeMovementValue;
		}
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

