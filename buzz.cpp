
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

float moveBeeX(float x_coordinate, bool isMovingRight, int beeMovementValue);
bool areColliding(int sprite1_x, int sprite1_y, int sprite1Width, int sprite1Height, int sprite2_x, int sprite2_y, int sprite2Width, int sprite2Height);
float startingIndexDifference(float startingOld, int jumpOld, int jumpNew);
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
int movePlayer(float player_x, int playerWidth, int playerMovementValue, bool isRight);
void moveBees(Sprite bees[], int noOfBees, float beesCoords[][2], bool areBeesMovingRight[], Clock beesClock[], bool beesAlive[], bool hasPollinated[], bool areBeesAlive[], float honeycombCoords[][2], int beeMovementValue, float beeSpeed, int beeHeight, int beeWidth, bool areRegular, int honeycombHeight, int honeycombWidth, int beeRowHeight);
void moveBee(Sprite &bee, float &x_coordinate, float &y_coordinate, bool &isMovingRight,float honeycombCoords[][2], bool havePollinated[], bool areBeesAlive[], int beeMovementValue, int beeHeight, int beeWidth, int isRegular, int noOfBees, int honeycombHeight, int honeycombWidth, int beeRowHeight);
void spawnBees(float beesCoords[][2], Clock beesClock[], bool beesAlive[], bool areBeesMovingRight[], int &beesSpawned, int totalBees, float delay, float offset, int beeWidth);
void drawBees(RenderWindow& window, int noOfBees, Sprite bees[], float beesCoords[][2], bool beesAlive[]);
bool killBee(Sprite bee, float bee_x_coordinate, float bee_y_coordinate, bool beeHaspollinated, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int ground_y_coordinate);
void killBees(Sprite bees[], bool beesAlive[], float beesCoords[][2], bool beesHavepollinated[], int beesSpawned, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int ground_y_coordinate);
void drawHoneycombs(RenderWindow &window, Sprite honeycombs[], float honeycombCoords[][2], bool beesAlive[], bool hasPollinated[], float beesCoords[][2], bool areDestroyed[], int noOfBees, int beeHeight, int beeWidth, int honeycombHeight, int honeycombWidth);
void beePollinatesGround(Sprite bees[], bool beesAlive[], bool beesHavePollinated[], float beesCoords[][2], int noOfBees, int beeHeight, int beeWidth, int flowerStartIndex[], bool isFlowerPollinated[], int totalFlowers, int flowerWidth, int flowerHeight, int groundY, int beeRowHeight);
void drawFlowers(RenderWindow& window, Sprite flowers[], int startIndex[], bool isFlowerPollinated[], int totalFlowers, int groundY, int flowerHeight);



int main()
{
	// levels
	const int LEVEL1_REGULAR = 200;
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
	int beeRowHeight = boxPixelsY * 2;

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
	bool regularBeesHavePollinated[LEVEL1_REGULAR] = {};

	beeTexture.loadFromFile("Textures/Regular_bee.png");
	for (int i = 0; i < LEVEL1_REGULAR; i++) {
		regularBees[i].setTexture(beeTexture);
		regularBees[i].setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	}

	// honeycombs
	int honeycombHeight = 64;
	int honeycombWidth = 64;

	Texture regularHoneycombTexture;
	Sprite regularHoneycombSprites[LEVEL1_REGULAR];

	bool areRegularHoneycombsDestroyed[LEVEL1_REGULAR] = {};
	float regularHoneycombCoords[LEVEL1_REGULAR][2] = {};

	regularHoneycombTexture.loadFromFile("Textures/honeycomb.png");
	for (int i = 0; i < LEVEL1_REGULAR; i++) {
		regularHoneycombSprites[i].setTexture(regularHoneycombTexture);
		regularHoneycombSprites[i].setTextureRect(sf::IntRect(0, 0, honeycombHeight, honeycombWidth));
	}


	// flowers
	int flowerWidth = 32;
	int flowerHeight = 29;
	int totalFlowers = resolutionX / flowerWidth + 1;

	Texture flowerTexture;
	Sprite flowerSprites[totalFlowers];
	bool isFlowerPollinated[totalFlowers] = {};

	int flowerStartIndex[totalFlowers];
	for (int i = 0; i < totalFlowers; i++) {
		// adding i because the next flower starts with the next index
		flowerStartIndex[i] = (i * flowerWidth) + i;
	}


	flowerTexture.loadFromFile("Textures/obstacles.png");
	for (int i = 0; i < totalFlowers; i++) {
		flowerSprites[i].setTexture(flowerTexture);
		flowerSprites[i].setTextureRect(sf::IntRect(0, 0, flowerHeight, flowerWidth));
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

		spawnBees(regularBeesCoords, regularBeesClock, regularBeesAlive, areRegularMovingRight, regularBeesSpawned, LEVEL1_REGULAR, LEVEL1_REGULAR_DELAY, LEVEL1_REGULAR_OFFSET, regularBeeWidth);
		moveBees(regularBees, regularBeesSpawned, regularBeesCoords, areRegularMovingRight, regularBeesClock, regularBeesAlive, regularBeesHavePollinated, regularBeesAlive, regularHoneycombCoords, regularBeeMovementValue, regularSpeed, regularBeeHeight, regularBeeWidth, true, honeycombHeight, honeycombWidth, beeRowHeight);
		killBees(regularBees, regularBeesAlive, regularBeesCoords, regularBeesHavePollinated, regularBeesSpawned, regularBeeWidth, regularBeeHeight, bullet_x, bullet_y, bulletWidth, groundY);
		beePollinatesGround(regularBees, regularBeesAlive, regularBeesHavePollinated, regularBeesCoords, regularBeesSpawned, regularBeeHeight, regularBeeWidth, flowerStartIndex, isFlowerPollinated, totalFlowers, flowerWidth, flowerHeight, groundY, beeRowHeight);
		drawBees(window, regularBeesSpawned, regularBees, regularBeesCoords, regularBeesAlive);

		drawFlowers(window, flowerSprites, flowerStartIndex, isFlowerPollinated, totalFlowers, groundY, flowerHeight);
		drawHoneycombs(window, regularHoneycombSprites, regularHoneycombCoords, regularBeesAlive, regularBeesHavePollinated, regularBeesCoords, areRegularHoneycombsDestroyed, regularBeesSpawned, regularBeeHeight, regularBeeWidth, honeycombHeight, honeycombWidth);
		drawPlayer(window, player_x, player_y, playerSprite);
		window.display();
		window.clear();
	}
}


void drawFlowers(RenderWindow& window, Sprite flowers[], int startIndex[], bool isFlowerPollinated[], int totalFlowers, int groundY, int flowerHeight) {
	for (int i = 0; i < totalFlowers; i++) {
		if (isFlowerPollinated[i]) {
			flowers[i].setPosition(startIndex[i], groundY - flowerHeight);
			window.draw(flowers[i]);
		}
	}
}


void beePollinatesGround(Sprite bees[], bool beesAlive[], bool beesHavePollinated[], float beesCoords[][2], int noOfBees, int beeHeight, int beeWidth, int flowerStartIndex[], bool isFlowerPollinated[], int totalFlowers, int flowerWidth, int flowerHeight, int groundY, int beeRowHeight) {
	for (int i = 0; i < noOfBees; i++) {
		// if in the air, no pollination anyway
		if (beesCoords[i][1] + beeHeight < groundY - beeRowHeight !! !beesAlive[i]) {
			continue;
		}

		int flowerIndex;
		// if the be
		for (flowerIndex = 0; flowerIndex < totalFlowers - 1; flowerIndex++) {
			bool isBeeCollidingWithFirstFlower = areColliding(beesCoords[i][0], beesCoords[i][1], beeWidth, beeHeight, flowerStartIndex[flowerIndex], groundY - beeRowHeight, flowerWidth, 1000);
			bool isBeeCollidingWithSecondFlower = areColliding(beesCoords[i][0], beesCoords[i][1], beeWidth, beeHeight, flowerStartIndex[flowerIndex + 1], groundY - beeRowHeight, flowerWidth, 1000);
			if (isBeeCollidingWithFirstFlower && isBeeCollidingWithSecondFlower) {
				if (isFlowerPollinated[flowerIndex + 1] && !isFlowerPollinated[flowerIndex]) {
					break;
				} else if (!isFlowerPollinated[flowerIndex] && !isFlowerPollinated[flowerIndex + 1]) {
					float beeMid = beesCoords[i][0] + (beeWidth / 2);
					if (beeMid > flowerStartIndex[flowerIndex + 1]) {
						flowerIndex++;
					}
					break;
				}
			} else if (isBeeCollidingWithFirstFlower && !isFlowerPollinated[flowerIndex]) {
				break;
			}
		}

		if (flowerIndex < totalFlowers - 1) {
			beesAlive[i] = false;
			beesHavePollinated[i] = true;
			isFlowerPollinated[flowerIndex] = true;
		}
		
	}

}

// set the coordinates of the honeycombs and draw
void drawHoneycombs(RenderWindow &window, Sprite honeycombs[], float honeycombCoords[][2], bool beesAlive[], bool hasPollinated[], float beesCoords[][2], bool areDestroyed[], int noOfBees, int beeHeight, int beeWidth, int honeycombHeight, int honeycombWidth) {
	for (int i = 0; i < noOfBees; i++) {
		if (!beesAlive[i] && !hasPollinated[i]) {
			honeycombCoords[i][0] = startingIndexDifference(beesCoords[i][0], beeWidth, honeycombWidth);
			honeycombCoords[i][1] = startingIndexDifference(beesCoords[i][1], beeHeight, honeycombHeight);

			// taking care of edge cases
			if (honeycombCoords[i][0] < 0) {
				honeycombCoords[i][0] = 0;
			} else if (honeycombCoords[i][0] + honeycombWidth > resolutionX) {
				honeycombCoords[i][0] = resolutionX - honeycombWidth;
			}

			if (honeycombCoords[i][1] < 0) {
				honeycombCoords[i][1] = 0;
			}

			// displaying honeycombs
			honeycombs[i].setPosition(honeycombCoords[i][0], honeycombCoords[i][1]);
			window.draw(honeycombs[i]);
		}
	}
}


// bees die if they touch the ground or a bullet
void killBees(Sprite bees[], bool beesAlive[], float beesCoords[][2], bool beesHavePollinated[], int beesSpawned, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int ground_y_coordinate) {
	for (int i = 0; i < beesSpawned; i++) {
		if (beesAlive[i]) {
			beesAlive[i] = killBee(bees[i], beesCoords[i][0], beesCoords[i][1], beesHavePollinated[i], beeWidth, beeHeight, bullet_x_coordinate, bullet_y_coordinate, bulletWidth, ground_y_coordinate);
		}
	}
}

// returns either true or false. Also sets the beeHasPollinated attribute
bool killBee(Sprite bee, float bee_x_coordinate, float bee_y_coordinate, bool beeHasPollinated, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int ground_y_coordinate) {

	if (bullet_y_coordinate < bee_y_coordinate + beeHeight     // if bullet is above the bottom boundary of bee
		&& bullet_y_coordinate > bee_y_coordinate) {            // if bullet is below the top boundary of bee

		if (bullet_x_coordinate + bulletWidth > bee_x_coordinate     // if bullet is to the right of left boundary of bee
			&& bullet_x_coordinate < bee_x_coordinate + beeWidth) {  // if bullet is to the left of right boundary of bee
			return false;
		}

	} else if 
		// dies if it pollinates
		(beeHasPollinated ||
		// of if it goes into the ground
		bee_y_coordinate + beeHeight > groundY) {
		
		return false;
	
	}
	return true;
}

// sets bee to alive and returns the initial x coordinate
void spawnBees(float beesCoords[][2], Clock beesClock[], bool beesAlive[], bool areBeesMovingRight[], int &beesSpawned, int totalBees, float delay, float offset, int beeWidth) {
	bool toSpawn = false;
	static Clock timeSinceLastSpawn;
	// if all the bees have already been spawned
	if (beesSpawned == totalBees) {
		return;
	} else if (beesSpawned == 0) {
		if (beesClock[beesSpawned].getElapsedTime().asSeconds() >= offset) {
			toSpawn = true;
		}
	} else if (timeSinceLastSpawn.getElapsedTime().asSeconds() >= delay) {
		toSpawn = true;
	}

	if (toSpawn) {
		// starting clock from 0 and incrementing beesSpawned
		beesClock[beesSpawned].restart();
		beesAlive[beesSpawned] = true;
		beesCoords[beesSpawned][0] = rand() % 2 ? 0: resolutionX - beeWidth; // based on a random number generated, the initial coordinate is set
		timeSinceLastSpawn.restart();

		// if at the top left corner, moving right otherwise moving left
		if (!beesCoords[beesSpawned][0]) {
			areBeesMovingRight[beesSpawned] = true;
		}
		beesSpawned++;
	}

}

void drawBees(RenderWindow& window, int noOfBees, Sprite bees[], float beesCoords[][2], bool beesAlive[]) {
	for (int i = 0; i < noOfBees; i++) {
		if (beesAlive[i]) {
			bees[i].setPosition(beesCoords[i][0], beesCoords[i][1]);
			window.draw(bees[i]);
		}
	}
}
void moveBees(Sprite bees[], int noOfBees, float beesCoords[][2], bool areBeesMovingRight[], Clock beesClock[], bool beesAlive[], bool hasPollinated[], bool areBeesAlive[], float honeycombCoords[][2], int beeMovementValue, float beeSpeed, int beeHeight, int beeWidth, bool areRegular, int honeycombHeight, int honeycombWidth, int beeRowHeight) {
	for (int i = 0; i < noOfBees; i++) {
		// if its the movement time
		if (beesAlive[i] && beesClock[i].getElapsedTime().asSeconds() >= 1 / beeSpeed) {
			moveBee(bees[i], beesCoords[i][0], beesCoords[i][1], areBeesMovingRight[i], honeycombCoords, hasPollinated, areBeesAlive, beeMovementValue, beeHeight, beeWidth, areRegular, noOfBees, honeycombHeight, honeycombWidth, beeRowHeight);
			beesClock[i].restart();
		}

	}
}


void moveBee(Sprite &bee, float &x_coordinate, float &y_coordinate, bool &isMovingRight,float honeycombCoords[][2], bool havePollinated[], bool areBeesAlive[], int beeMovementValue, int beeHeight, int beeWidth, int isRegular, int noOfBees, int honeycombHeight, int honeycombWidth, int beeRowHeight) {
	bool directionChanged = false;

	float x_coordinateAfterMovement = moveBeeX(x_coordinate, isMovingRight, beeMovementValue);

	// looking for any collision with honeycombs
	for (int i = 0; i < noOfBees; i++) {
		if (!havePollinated[i] && !areBeesAlive[i]){
			bool isBeeCollidingWithHoneycomb = areColliding(x_coordinateAfterMovement, y_coordinate, beeWidth, beeHeight, honeycombCoords[i][0], honeycombCoords[i][1], honeycombWidth, honeycombHeight);
			if (isBeeCollidingWithHoneycomb) {
				isMovingRight = !isMovingRight;
				directionChanged = true;
				break;
			}
		}
	}

	x_coordinateAfterMovement = moveBeeX(x_coordinate, isMovingRight, beeMovementValue);

	//  if at the left or right boundaries, setting the x coordinates at the boundaries too
	bool isBeeCollidingWithLeftWall = areColliding(x_coordinateAfterMovement, y_coordinate, beeWidth, beeHeight, -10, 0, 9, resolutionY);
	bool isBeeCollidingWithRightWall = areColliding(x_coordinateAfterMovement, y_coordinate, beeWidth, beeHeight, resolutionX + 1, 0, 1000, resolutionY);
	if (isBeeCollidingWithRightWall) {
		directionChanged = true;
		isMovingRight = !isMovingRight;
		x_coordinate = resolutionX - beeWidth;

	} else if (isBeeCollidingWithLeftWall) {
		directionChanged = true;
		isMovingRight = !isMovingRight;
		x_coordinate = 0;
	}


	x_coordinateAfterMovement = moveBeeX(x_coordinate, isMovingRight, beeMovementValue);

	if (directionChanged) {
		y_coordinate += beeRowHeight;

	} else {
		x_coordinate = x_coordinateAfterMovement;
	}

}

float moveBeeX(float x_coordinate, bool isMovingRight, int beeMovementValue) {
	if (isMovingRight) {
		return x_coordinate + beeMovementValue;
	} else {
		return x_coordinate - beeMovementValue;
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


// this function returns the subrataction from the index you need to make an object centered based
// on the position of another object
// new is the object to be placed and priv is the object to be replaced
float startingIndexDifference(float startingOld, int jumpOld, int jumpNew) {
	if (jumpOld < jumpNew) {
		return startingOld - ((jumpNew - jumpOld) / 2.0f);
	}
	return startingOld + ((jumpOld - jumpNew) / 2.0f);
}

// this function checks if any of the pixels of the objects are overlapping. if they are,
// it returns true, otherwise returns false
bool areColliding(int sprite1_x, int sprite1_y, int sprite1Width, int sprite1Height, int sprite2_x, int sprite2_y, int sprite2Width, int sprite2Height) {
    if (sprite1_x + sprite1Width > sprite2_x && sprite2_x + sprite2Width > sprite1_x) {
        if (sprite1_y + sprite1Height > sprite2_y && sprite2_y + sprite2Height > sprite1_y) {
            return true;
        }
    }
    return false;
}