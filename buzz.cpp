
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>

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
void moveBees(Sprite bees[], int noOfBees, float beesCoords[][2], bool areBeesMovingRight[], Clock beesClock[], bool beesAlive[], int beeMovementValue, float beeSpeed, int regularBeeWidth);
void moveBee(Sprite &bee, float &x_coordinate, float &y_coordinate, bool &isMovingRight, int beeMovementValue, int beeWidth);
void spawnBees(float beesCoords[][2], Clock beesClock[], bool beesAlive[], int &beesSpawned, int totalBees, float delay, float offset);
void drawBees(RenderWindow& window, int noOfBees, Sprite bees[], float beesCoords[][2]);
bool isBeeAlive(Sprite bee, float bee_x_coordinate, float bee_y_coordinate, bool &beeHasPolinated, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int ground_y_coordinate);
void killBees(Sprite bees[], bool beesAlive[], float beesCoords[][2], bool beesHavePolinated[], int beesSpawned, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int ground_y_coordinate);


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
	const float regularSpeed = 20;
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
	int bulletWidth = 16;
	int bulletHeight = 16;
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
	int regularBeesSpawned = 0;

	Texture beeTexture;
	Clock regularBeesClock[LEVEL1_REGULAR];
	Sprite regularBees[LEVEL1_REGULAR];
	// column 0: x coordinate, column 1: y coordinate
	float regularBeesCoords[LEVEL1_REGULAR][2] = {};
	bool areRegularMovingRight[LEVEL1_REGULAR] = {};
	bool regularBeesAlive[LEVEL1_REGULAR] = {};
	bool regularBeeHasPolinated[LEVEL1_REGULAR] = {};

	beeTexture.loadFromFile("Textures/Regular_bee.png");
	for (int i = 0; i < LEVEL1_REGULAR; i++) {
		regularBees[i].setTexture(beeTexture);
		regularBees[i].setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	}

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
		window.draw(groundRectangle);

		spawnBees(regularBeesCoords, regularBeesClock, regularBeesAlive, regularBeesSpawned, LEVEL1_REGULAR, LEVEL1_REGULAR_DELAY, LEVEL1_REGULAR_OFFSET);
		moveBees(regularBees, regularBeesSpawned, regularBeesCoords, areRegularMovingRight, regularBeesClock, regularBeesAlive, regularBeeMovementValue, regularSpeed, regularBeeWidth);
		drawBees(window, regularBeesSpawned, regularBees, regularBeesCoords);
		killBees(regularBees, regularBeesAlive, regularBeesCoords, regularBeeHasPolinated, regularBeesSpawned, regularBeeWidth, regularBeeHeight, bullet_x, bullet_y, bulletWidth, groundY);



		drawPlayer(window, player_x, player_y, playerSprite);
		window.display();
		window.clear();
	}
}

// bees die if they touch the ground or a bullet
void killBees(Sprite bees[], bool beesAlive[], float beesCoords[][2], bool beesHavePolinated[], int beesSpawned, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int ground_y_coordinate) {
	for (int i = 0; i < beesSpawned; i++) {
		if (beesAlive[i]) {
			beesAlive[i] = isBeeAlive(bees[i], beesCoords[i][0], beesCoords[i][1], beesHavePolinated[i], beeWidth, beeHeight, bullet_x_coordinate, bullet_y_coordinate, bulletWidth, ground_y_coordinate);
		}
	}
}

bool isBeeAlive(Sprite bee, float bee_x_coordinate, float bee_y_coordinate, bool &beeHasPolinated, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int ground_y_coordinate) {

	if (bullet_y_coordinate < bee_y_coordinate + beeHeight     // if bullet is above the bottom boundary of bee
		&& bullet_y_coordinate > bee_y_coordinate) {            // if bullet is below the top boundary of bee

		if (bullet_x_coordinate + bulletWidth > bee_x_coordinate     // if bullet is to the right of left boundary of bee
			&& bullet_x_coordinate < bee_x_coordinate + beeWidth) {  // if bullet is to the left of right boundary of bee
			return false;
		}

	// if the bee is in the ground
	} else if (bee_y_coordinate + beeHeight > ground_y_coordinate) {
		beeHasPolinated = true;
		return false;
	}
	return true;
}

// ssets bee to alive and returns the initial x coordinate
void spawnBees(float beesCoords[][2], Clock beesClock[], bool beesAlive[], int &beesSpawned, int totalBees, float delay, float offset) {
	bool toSpawn = false;
	static Clock timeSinceLastSpawn;
	// if all the bees have already been spawned
	if (beesSpawned == totalBees) {
		return;
	} else if (beesSpawned == 0) {
		if (beesClock[beesSpawned].getElapsedTime().asSeconds() >= offset) {
			cout << beesClock[beesSpawned].getElapsedTime().asSeconds();
			toSpawn = true;
		}
	} else if (timeSinceLastSpawn.getElapsedTime().asSeconds() >= delay) {
		toSpawn = true;
	}

	if (toSpawn) {
		// starting clock from 0 and incrementing beesSpawned
		beesClock[beesSpawned].restart();
		beesAlive[beesSpawned] = true;
		beesCoords[beesSpawned][0] = rand() % 2 ? 0: resolutionX; // based on a random number generated, the initial coordinate is set
		timeSinceLastSpawn.restart();
		beesSpawned++;
	}

}

void drawBees(RenderWindow& window, int noOfBees, Sprite bees[], float beesCoords[][2]) {
	for (int i = 0; i < noOfBees; i++) {
		bees[i].setPosition(beesCoords[i][0], beesCoords[i][1]);
		window.draw(bees[i]);
	}
}

void moveBees(Sprite bees[], int noOfBees, float beesCoords[][2], bool areBeesMovingRight[], Clock beesClock[], bool beesAlive[], int beeMovementValue, float beeSpeed, int beeWidth) {
	for (int i = 0; i < noOfBees; i++) {
		// if its the movement time
		if (beesAlive[i] && beesClock[i].getElapsedTime().asSeconds() >= 1 / beeSpeed) {
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

