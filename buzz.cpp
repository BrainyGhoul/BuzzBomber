
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <cmath>

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


bool allTrue(bool arr[], int size);
float moveInsideScreen(float coordinate, int extremeCoordinate, int jump);
bool areMidCoordinatesSame(int sprite1_x, int sprite1_y, int sprite1Width, int sprite1Height, int sprite2_x, int sprite2_y, int sprite2Width, int sprite2Height);
float midCoordinate(float startCoordinate, int jump);
bool moveSpriteToPoint(float &spriteX, float &spriteY, float destinationX, float destinationY, float stepValue);
float moveBeeX(float x_coordinate, bool isMovingRight, int beeMovementValue);
bool areColliding(int sprite1_x, int sprite1_y, int sprite1Width, int sprite1Height, int sprite2_x, int sprite2_y, int sprite2Width, int sprite2Height);
float startingIndexDifference(float startingOld, int jumpOld, int jumpNew);
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, float backgroundX, float backgroundY, int backgroundOffsetX, int backgroundOffsetY, int backgroundWidth, int backgroundHeight,  Sprite& backgroundSprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
int movePlayer(float player_x, int playerWidth, int playerMovementValue, bool isRight, float flowersCoords[][2], bool flowersPollinated[], int flowerWidth, int noOfFlowers);
void moveBees(Sprite bees[], int noOfBees, float beesCoords[][2], bool areBeesMovingRight[], Clock beesClock[], bool beesAlive[], bool hasPollinated[], bool honeycombCreated[], float honeycombCoords[][2], bool honeycombDestroyed[], bool areBeeHivesCreated[], float beeHiveCoords[][2], int beeMovementValue, float beeSpeed, int beeHeight, int beeWidth, bool areRegular, int honeycombHeight, int honeycombWidth, int noOfHoneycombs, float playerX, float playerY, int playerWidth, int playerHeight, int beeHiveWidth, int beeHiveHeight, int beeRowHeight, int groundY);
void moveBee(Sprite &bee, float &x_coordinate, float &y_coordinate, bool &isMovingRight, bool &isBeeAlive, bool &isBeeHiveCreated, float &beeHiveX, float &beeHiveY, float honeycombCoords[][2], bool havePollinated[], bool honeycombCreated[], bool honeycombDestroyed[], int beeMovementValue, int beeHeight, int beeWidth, int isRegular, int noOfBees, int honeycombHeight, int honeycombWidth, int noOfHoneycombs, float playerX, float playerY, int playerWidth, int playerHeight, int beeHiveWidth, int beeHiveHeight, int beeRowHeight, int groundY);
void spawnBees(Sprite bees[], float beesCoords[][2], Clock beesClock[], bool beesAlive[], bool areBeesMovingRight[], int &beesSpawned, int totalBees, float delay, float offset, int beeWidth, int beeHeight);
void drawBees(RenderWindow& window, int noOfBees, Sprite bees[], float beesCoords[][2], bool beesAlive[]);
bool killBee(Sprite bee, float bee_x_coordinate, float bee_y_coordinate, bool beeHaspollinated, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int bulletHeight, int ground_y_coordinate, bool bulletExists, bool &bulletCollided);
void killBees(Sprite bees[], bool beesAlive[], float beesCoords[][2], bool beesHavepollinated[], bool isHoneycombCreated[], bool isHoneycombDestroyed[], float honeycombCoords[][2], int beesSpawned, int beeWidth, int beeHeight, float bullet_x_coordinate, float &bullet_y_coordinate, int bulletWidth, int bulletHeight, bool bulletExists, int honeycombWidth, int honeycombHeight, int ground_y_coordinate);
void drawHoneycombs(RenderWindow &window, Sprite honeycombs[], float honeycombCoords[][2], bool isHoneycombCreated[], bool hasPollinated[], float beesCoords[][2], bool honeycombDestroyed[], int noOfHoneycombs, int beeHeight, int beeWidth, int honeycombHeight, int honeycombWidth);
void beePollinatesGround(Sprite bees[], bool beesAlive[], bool beesHavePollinated[], float beesCoords[][2], bool areBeesMovingRight[], int noOfBees, int beeHeight, int beeWidth, float flowerStartIndex[][2], bool isFlowerPollinated[], int totalFlowers, int flowerWidth, int flowerHeight, float &playerX, float &playerY, float &backgroundX, int playerWidth, int playerHeight, int groundY, int beeRowHeight);
void drawFlowers(RenderWindow& window, Sprite flowers[], float startIndex[][2], bool isFlowerPollinated[], int totalFlowers, int groundY, int flowerHeight);
void destroyObject(bool objectDestroyed[], float objectCoords[][2], bool objectCreated[], int noOfObjects, int objectWidth, int objectHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int bulletHeight, bool bulletExists);
void manageHummingBird(Sprite &hummingBird, float &hummingBirdX, float &hummingBirdY, int hummingBirdWidth, int hummingBirdHeight, int hummingBirdStepValue, bool &isHummingBirdSick, float honeycombCoords[][2], bool isHoneycombCreated[], int honeycombWidth, int honeycombHeight, int honeycombs, bool areBeesAlive[], bool hasPollinated[], bool honeycombDestroyed[], int &honeycombsSucked, float bulletX, float &bulletY, int bulletWidth, int bulletHeight, bool bulletExists, int hummingBirdHealingTime, float hummingBirdSpeed, float hummingBirdRestX, float hummingBirdRestY, int nectarSuckTime);
void drawHummingBird(RenderWindow &window, Sprite hummingBird, float hummingBirdX, float hummingBirdY);
void moveHummingBirdToPoint(Sprite &hummingBird, float &hummingBirdX, float &hummingBirdY, float destinationX, float destinationY, int hummingBirdWidth, int hummingBirdHeight, float stepValue);
void drawRemainingBottles(RenderWindow &window, float startingX, float startingY, Texture bottleTexture, int remainingBottles, int bottleWidth, int bottleHeight);
float adjustedStartingIndexDifference(float startingOld, int jumpOld, int jumpNew, int extremeCoordinate);
void drawBeeHives(RenderWindow &window, Sprite hiveSprites[], float beeHiveCoords[][2], bool isHiveCreated[], bool isHiveDestroyed[], int noOfBeeHives);
void initializeSprites(Texture &spriteTexture, Sprite sprites[], float spriteCoords[][2], int noOfSprites, int noOfAdditionalSprites, bool isSpriteCreated[], int spriteWidth, int spriteHeight, int groundY, int playerHeight);
bool staticSpritesCollision(float spriteX, float spriteY, int spriteWidth, int spriteHeight, bool destroyCollidedObject,  int spriteIndex, bool firstCall,  int honeycombheight, int honeycombwidth,  int noofregularhoneycombs, float regularhoneycombcoords[][2],  bool regularhoneycombcreated[], bool regularhoneycombdestroyed[],  int noofredhoneycombs, float redhoneycombcoords[][2],  bool redhoneycombcreated[], bool redhoneycombdestroyed[],  int noofbeehives, float beehivecoords[][2],  bool beehivecreated[], bool beehivedestroyed[], int beehivewidth, int beehiveheight);
bool isCollidingWithMultipleSprites(float sprite1X, float sprite1Y, int sprite1Width, int sprite1Height, float (*sprite2Coords)[2], bool *sprite2Created, bool *sprite2Destroyed, int sprite2Width, int sprite2Height, int sprite2Length, int sprite1Index, bool destroyCollidedObject);


int main()
{
	// levels
	const int LEVEL1_REGULAR = 100;
	const int LEVEL2_REGULAR = 15;
	const int LEVEL3_REGULAR = 20;

	const int LEVEL1_FAST = 0;
	const int LEVEL2_FAST = 5;
	const int LEVEL3_FAST = 10;

	const int LEVEL1_HONEYCOMB = 3;
	const int LEVEL2_HONEYCOMB = 9;
	const int LEVEL3_HONEYCOMB = 15;
	
	const int LEVEL1_BEEHIVES = 5;
	const int LEVEL2_BEEHIVES = 2;
	const int LEVEL3_BEEHIVES = 3;

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
	const float hummingBirdSpeed = 4;

	// other settings and variables
	int groundY = (gameRows - 2) * boxPixelsY;
	int playerMovementValue = boxPixelsX;
	int regularBeeMovementValue = 10;
	int beeRowHeight = boxPixelsY * 2;
	int hummingBirdHealingTime = 1000;
	int hummingBirdStepValue = boxPixelsX;
	int nectarSuckTime = 5;

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

	int initialSprayCans = 3;
	int spraysPerSprayCan = 56;
	int changeCanStateAfterSprays = 8;


	int spraysCansLeft = initialSprayCans - 1;
	int spraysLeft = spraysPerSprayCan;
	
	Texture playerTexture;
	Sprite playerSprite;
	playerTexture.loadFromFile("Textures/spray.png");
	playerSprite.setTexture(playerTexture);


	int playerHollowX = 21;
	int playerHollowY = 30;


	float playerBackgroundX = player_x + playerHollowX;
	float playerBackgroundY = player_y + playerHollowY;
	int playerBackgroundOffsetX = 0;
	int playerBackgroundOffsetY = 0;
	int playerBackgroundWidth = 21;
	int playerBackgroundHeight = 33;
	int playerBackgroundLevelHeight = playerBackgroundHeight / (spraysPerSprayCan / changeCanStateAfterSprays);

	// the white rectangle behind player
	Texture playerBackgroundTexture;
	Sprite playerBackground;
	playerBackgroundTexture.loadFromFile("Textures/playerBackground.jpg");
	playerBackground.setTexture(playerBackgroundTexture);
	playerBackground.setPosition(playerBackgroundX, playerBackgroundY);
	// playerBackground.setFillColor(Color::White);




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


	Texture regularBeeTexture;
	Clock regularBeesClock[LEVEL1_REGULAR];
	Sprite regularBees[LEVEL1_REGULAR];
	// column 0: x coordinate, column 1: y coordinate
	float regularBeesCoords[LEVEL1_REGULAR][2] = {};
	bool areRegularMovingRight[LEVEL1_REGULAR] = {};
	bool regularBeesAlive[LEVEL1_REGULAR] = {};
	bool regularBeesHavePollinated[LEVEL1_REGULAR] = {};

	regularBeeTexture.loadFromFile("Textures/Regular_bee.png");
	for (int i = 0; i < LEVEL1_REGULAR; i++) {
		regularBees[i].setTexture(regularBeeTexture);
		regularBees[i].setTextureRect(sf::IntRect(0, 0, regularBeeWidth, regularBeeHeight));
	}

	// fast bees
	int fastBeeWidth = 46;
	int fastBeeHeight = 22;
	int fastBeesSpawned = 0;


	Texture fastBeeTexture;
	Clock fastBeesClock[LEVEL1_FAST];
	Sprite fastBees[LEVEL1_FAST];
	// column 0: x coordinate, column 1: y coordinate
	float fastBeesCoords[LEVEL1_FAST][2] = {};
	bool areFastMovingRight[LEVEL1_FAST] = {};
	bool fastBeesAlive[LEVEL1_FAST] = {};
	bool fastBeesHavePollinated[LEVEL1_FAST] = {};

	fastBeeTexture.loadFromFile("Textures/Fast_bee.png");
	for (int i = 0; i < LEVEL1_FAST; i++) {
		fastBees[i].setTexture(fastBeeTexture);
		fastBees[i].setTextureRect(sf::IntRect(0, 0, fastBeeWidth, fastBeeHeight));
	}
	

	// honeycombs
	int honeycombHeight = 64;
	int honeycombWidth = 64;
	int noOfRegularHoneycombs = LEVEL1_REGULAR + LEVEL1_HONEYCOMB;

	Texture regularHoneycombTexture;
	Sprite regularHoneycombSprites[noOfRegularHoneycombs];

	bool isRegularHoneycombCreated[noOfRegularHoneycombs] = {};
	bool areRegularHoneycombsDestroyed[noOfRegularHoneycombs] = {};
	float regularHoneycombCoords[noOfRegularHoneycombs][2] = {};

	regularHoneycombTexture.loadFromFile("Textures/honeycomb.png");



	int noOfRedHoneycombs = LEVEL1_REGULAR;
	Texture redHoneycombTexture;
	Sprite redHoneycombSprites[noOfRegularHoneycombs];

	bool isRedHoneycombCreated[noOfRegularHoneycombs] = {};
	bool areRedHoneycombsDestroyed[noOfRegularHoneycombs] = {};
	float redHoneycombCoords[noOfRegularHoneycombs][2] = {};

	redHoneycombTexture.loadFromFile("Textures/honeycomb_red.png");



	// flowers
	int flowerWidth = 16;
	while (resolutionX % flowerWidth) {
		flowerWidth--;
	}
	int flowerHeight = 29;
	int totalFlowers = resolutionX / flowerWidth;

	Texture flowerTexture;
	Sprite flowerSprites[totalFlowers];
	bool isFlowerPollinated[totalFlowers] = {};

	float flowerStartIndex[totalFlowers][2];
	for (int i = 0; i < totalFlowers; i++) {
		// adding i because the next flower starts with the next index
		flowerStartIndex[i][0] = (i * flowerWidth);
		flowerStartIndex[i][1] = groundY - flowerHeight;
	}


	flowerTexture.loadFromFile("Textures/obstacles.png");
	for (int i = 0; i < totalFlowers; i++) {
		flowerSprites[i].setTexture(flowerTexture);
		flowerSprites[i].setTextureRect(sf::IntRect(0, 0, flowerWidth, flowerHeight));
	}


	// humming bird
	int hummingBirdWidth = 32;
	int hummingBirdHeight = 32;
	// fix this. need to use the difference index thing
	float hummingBirdRestX = resolutionX - hummingBirdWidth - 50;
	float hummingBirdRestY = 50;


	Texture hummingBirdTexture;
	Sprite hummingBirdSprite;
	bool isHummingBirdSick = false;
	bool isHummingBirdSuckingNectar = false;
	float hummingBirdX = hummingBirdRestX;
	float hummingBirdY = hummingBirdRestY;
	int regularHoneycombsSucked = 0;

	hummingBirdTexture.loadFromFile("Textures/bird.png");
	hummingBirdSprite.setTexture(hummingBirdTexture);
	hummingBirdSprite.setTextureRect(sf::IntRect(hummingBirdWidth, 0, hummingBirdWidth, hummingBirdHeight));
	hummingBirdSprite.setColor(Color::Blue);


	// bee hives
	int beeHiveWidth = 64;
	int beeHiveHeight = 64;
	
	int noOfBeeHives = LEVEL1_REGULAR + LEVEL1_BEEHIVES;

	Texture beeHiveTexture;
	Sprite beeHives[noOfBeeHives];
	bool areBeeHivesCreated[noOfBeeHives] = {};
	bool areBeeHivesDestroyed[noOfBeeHives] = {};
	float beeHiveCoords[noOfBeeHives][2];

	beeHiveTexture.loadFromFile("Textures/hive.png");

	// giving initial state to the function
	staticSpritesCollision(0, 0 , 0, 0, false, 0, true, honeycombHeight, honeycombWidth, noOfRegularHoneycombs, regularHoneycombCoords, isRegularHoneycombCreated, areRegularHoneycombsDestroyed, noOfRedHoneycombs, redHoneycombCoords, isRedHoneycombCreated, areRedHoneycombsDestroyed, noOfBeeHives, beeHiveCoords, areBeeHivesCreated, areBeeHivesDestroyed, beeHiveWidth, beeHiveHeight);
	float a, b;
	int c, d;
	bool *e, *f;
	findSpriteForHummingBird(a, b, c, d, e, f, true, honeycombHeight, honeycombWidth, noOfRegularHoneycombs, regularHoneycombCoords, isRegularHoneycombCreated, areRegularHoneycombsDestroyed, noOfRedHoneycombs, redHoneycombCoords, isRedHoneycombCreated, areRedHoneycombsDestroyed, noOfBeeHives, beeHiveCoords, areBeeHivesCreated, areBeeHivesDestroyed, beeHiveWidth, beeHiveHeight);


	initializeSprites(beeHiveTexture, beeHives, beeHiveCoords, noOfBeeHives, LEVEL1_BEEHIVES, areBeeHivesCreated, beeHiveWidth, beeHiveHeight, groundY, playerHeight);
	initializeSprites(redHoneycombTexture, redHoneycombSprites, redHoneycombCoords, noOfRedHoneycombs, 0, isRedHoneycombCreated, honeycombWidth, honeycombHeight, groundY, playerHeight);
	initializeSprites(regularHoneycombTexture, regularHoneycombSprites, regularHoneycombCoords, noOfRegularHoneycombs, LEVEL1_HONEYCOMB, isRegularHoneycombCreated, honeycombWidth, honeycombHeight, groundY, playerHeight);
	while (window.isOpen()) {

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				return 0;
			}
			else if (e.type == Event::KeyPressed) {
				// movement of the spray can
				float playerPreviousX = player_x;
				if (e.key.code == sf::Keyboard::Right || e.key.code == sf::Keyboard::Left) {
					bool isRight = true;
					if (e.key.code == sf::Keyboard::Left) {
						isRight = false;
					}
					player_x = movePlayer(player_x, playerWidth, playerMovementValue, isRight, flowerStartIndex, isFlowerPollinated, flowerWidth, totalFlowers);

				// shoot spray
				} else if (e.key.code == sf::Keyboard::Space) {
					if (!bullet_exists) {
						spraysLeft--;

						if (!((spraysPerSprayCan - spraysLeft) % changeCanStateAfterSprays)) {
							playerBackgroundOffsetY += playerBackgroundLevelHeight;
						}
					}

					bullet_exists = true;

					if (!spraysLeft) {
						spraysCansLeft--;
						spraysLeft = spraysPerSprayCan;
						playerBackgroundOffsetY = 0;
					}
					if (spraysCansLeft < 0) {
						// game over
					}
				}

				playerBackgroundX += player_x - playerPreviousX;
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
		manageHummingBird(hummingBirdSprite, hummingBirdX, hummingBirdY, hummingBirdWidth, hummingBirdHeight, hummingBirdStepValue, isHummingBirdSick, regularHoneycombsSucked, bullet_x, bullet_y, bulletWidth, bulletHeight, bullet_exists, hummingBirdHealingTime, hummingBirdSpeed, hummingBirdRestX, hummingBirdRestY, nectarSuckTime);
		spawnBees(regularBees, regularBeesCoords, regularBeesClock, regularBeesAlive, areRegularMovingRight, regularBeesSpawned, LEVEL1_REGULAR, LEVEL1_REGULAR_DELAY, LEVEL1_REGULAR_OFFSET, regularBeeWidth, regularBeeHeight);
		moveBees(regularBees, regularBeesSpawned, regularBeesCoords, areRegularMovingRight, regularBeesClock, regularBeesAlive, regularBeesHavePollinated, isRegularHoneycombCreated, regularHoneycombCoords, areRegularHoneycombsDestroyed, areBeeHivesCreated, beeHiveCoords, regularBeeMovementValue, regularSpeed, regularBeeHeight, regularBeeWidth, true, honeycombHeight, honeycombWidth, noOfRegularHoneycombs, player_x, player_y, playerWidth, playerHeight, beeHiveWidth, beeHiveHeight, beeRowHeight, groundY);
		killBees(regularBees, regularBeesAlive, regularBeesCoords, regularBeesHavePollinated, isRegularHoneycombCreated, areRegularHoneycombsDestroyed, regularHoneycombCoords, regularBeesSpawned, regularBeeWidth, regularBeeHeight, bullet_x, bullet_y, bulletWidth, bulletHeight, bullet_exists, honeycombWidth, honeycombHeight, groundY);
		destroyObject(areRegularHoneycombsDestroyed, regularHoneycombCoords, isRegularHoneycombCreated, noOfRegularHoneycombs, honeycombWidth, honeycombHeight, bullet_x, bullet_y, bulletWidth, bulletHeight, bullet_exists);
		destroyObject(areBeeHivesDestroyed, beeHiveCoords, areBeeHivesCreated, noOfBeeHives, beeHiveWidth, beeHiveHeight, bullet_x, bullet_y, bulletWidth, bulletHeight, bullet_exists);
		beePollinatesGround(regularBees, regularBeesAlive, regularBeesHavePollinated, regularBeesCoords, areRegularMovingRight, regularBeesSpawned, regularBeeHeight, regularBeeWidth, flowerStartIndex, isFlowerPollinated, totalFlowers, flowerWidth, flowerHeight, player_x, player_y, playerBackgroundX, playerWidth, playerHeight, groundY, beeRowHeight);
		drawBees(window, regularBeesSpawned, regularBees, regularBeesCoords, regularBeesAlive);


		drawFlowers(window, flowerSprites, flowerStartIndex, isFlowerPollinated, totalFlowers, groundY, flowerHeight);
		drawHoneycombs(window, regularHoneycombSprites, regularHoneycombCoords, isRegularHoneycombCreated, regularBeesHavePollinated, regularBeesCoords, areRegularHoneycombsDestroyed, noOfRegularHoneycombs, regularBeeHeight, regularBeeWidth, honeycombHeight, honeycombWidth);
		drawBeeHives(window, beeHives, beeHiveCoords, areBeeHivesCreated, areBeeHivesDestroyed, noOfBeeHives);
		drawHummingBird(window, hummingBirdSprite, hummingBirdX, hummingBirdY);
		drawPlayer(window, player_x, player_y, playerSprite, playerBackgroundX, playerBackgroundY, playerBackgroundOffsetX, playerBackgroundOffsetY, playerBackgroundWidth, playerBackgroundHeight, playerBackground);
		drawRemainingBottles(window, 0, groundY, playerTexture, spraysCansLeft, playerWidth, playerHeight);

		window.display();
		window.clear();
	}
}


// objects that are still e.g honeycomb and hive
// always use before setting the created field to true
// this function checks if any of these overlap
bool findSpriteForHummingBird(float &currentSpriteX, float &currentSpriteY, int &currentSpriteWidth, int &currentSpriteHeight, bool *&currentSpriteCreated, bool *&currentSpriteDestroyed, bool firstCall = false, int honeycombheight = 0, int honeycombwidth = 0, int noofregularhoneycombs = 0, float regularhoneycombcoords[][2] = nullptr, bool regularhoneycombcreated[] = nullptr, bool regularhoneycombdestroyed[] = nullptr, int noofredhoneycombs = 0, float redhoneycombcoords[][2] = nullptr, bool redhoneycombcreated[] = nullptr, bool redhoneycombdestroyed[] = nullptr, int noofbeehives = 0, float beehivecoords[][2] = nullptr, bool beehivecreated[] = nullptr, bool beehivedestroyed[] = nullptr, int beehivewidth = 0, int beehiveheight = 0) {
	static int honeycombHeight;
	static int honeycombWidth;
	static int noOfRegularHoneycombs;
	static float (*regularHoneycombCoords)[2];
	static bool *regularHoneycombCreated;
	static bool *regularHoneycombDestroyed;
	static int noOfRedHoneycombs;
	static float (*redHoneycombCoords)[2];
	static bool *redHoneycombCreated;
	static bool *redHoneycombDestroyed;
	static int beeHiveHeight;
	static int beeHiveWidth;
	static int noOfBeeHives;
	static float (*beeHiveCoords)[2];
	static bool *beeHiveCreated;
	static bool *beeHiveDestroyed;

	if (firstCall) {
		honeycombHeight = honeycombheight;
		honeycombWidth = honeycombwidth;
		beeHiveWidth = beehivewidth;
		beeHiveHeight = beehiveheight;

		noOfRegularHoneycombs = noofregularhoneycombs;
		noOfRedHoneycombs = noofredhoneycombs;
		noOfBeeHives = noofbeehives;

		regularHoneycombCoords = regularhoneycombcoords;
		redHoneycombCoords = redhoneycombcoords;
		beeHiveCoords = beehivecoords;

		regularHoneycombCreated = regularhoneycombcreated;
		regularHoneycombDestroyed = regularhoneycombdestroyed;
		redHoneycombCreated = redhoneycombcreated;
		redHoneycombDestroyed = redhoneycombdestroyed;
		beeHiveCreated = beehivecreated;
		beeHiveDestroyed = beehivedestroyed;
		return true;
	} else {
		for (int i = 0; i < noOfRegularHoneycombs; i++) {
			if (regularHoneycombCreated[i] && !regularHoneycombDestroyed[i]) {
				currentSpriteX = regularHoneycombCoords[i][0];
				currentSpriteY = regularHoneycombCoords[i][1];
				currentSpriteWidth = honeycombWidth;
				currentSpriteHeight = honeycombHeight;
				currentSpriteCreated = &regularHoneycombCreated[i];
				currentSpriteDestroyed = &regularHoneycombDestroyed[i];
				return true;
			}

		}

		for (int i = 0; i < noOfRedHoneycombs; i++) {
			if (redHoneycombCreated[i] && !redHoneycombDestroyed[i]) {
				currentSpriteX = redHoneycombCoords[i][0];
				currentSpriteY = redHoneycombCoords[i][1];
				currentSpriteWidth = honeycombWidth;
				currentSpriteHeight = honeycombHeight;
				currentSpriteCreated = &redHoneycombCreated[i];
				currentSpriteDestroyed = &redHoneycombDestroyed[i];
				return true;
			}
		}
		for (int i = 0; i < noOfBeeHives; i++) {
			if (beeHiveCreated[i] && !beeHiveDestroyed[i]) {
				currentSpriteX = beeHiveCoords[i][0];
				currentSpriteY = beeHiveCoords[i][1];
				currentSpriteWidth = beeHiveWidth;
				currentSpriteHeight = beeHiveHeight;
				currentSpriteCreated = &beeHiveCreated[i];
				currentSpriteDestroyed = &beeHiveDestroyed[i];
				return true;
			
			}
		}
		// logic here
		// need to change float &currentSpriteX, float &currentSpriteY, int &currentSpriteWidth, int &currentSpriteHeight, bool &currentSpriteCreated, bool &currentSpriteDestroyed
	}

	return false;
}







// objects that are still e.g honeycomb and hive
// always use before setting the created field to true
// this function checks if any of these overlap
bool staticSpritesCollision(float spriteX, float spriteY, int spriteWidth, int spriteHeight, bool destroyCollidedObject = false, int spriteIndex = -1, bool firstCall = false, int honeycombheight = 0, int honeycombwidth = 0, int noofregularhoneycombs = 0, float regularhoneycombcoords[][2] = nullptr, bool regularhoneycombcreated[] = nullptr, bool regularhoneycombdestroyed[] = nullptr, int noofredhoneycombs = 0, float redhoneycombcoords[][2] = nullptr, bool redhoneycombcreated[] = nullptr, bool redhoneycombdestroyed[] = nullptr, int noofbeehives = 0, float beehivecoords[][2] = nullptr, bool beehivecreated[] = nullptr, bool beehivedestroyed[] = nullptr, int beehivewidth = 0, int beehiveheight = 0) {
	static int honeycombHeight;
	static int honeycombWidth;
	static int noOfRegularHoneycombs;
	static float (*regularHoneycombCoords)[2];
	static bool *regularHoneycombCreated;
	static bool *regularHoneycombDestroyed;
	static int noOfRedHoneycombs;
	static float (*redHoneycombCoords)[2];
	static bool *redHoneycombCreated;
	static bool *redHoneycombDestroyed;
	static int beeHiveHeight;
	static int beeHiveWidth;
	static int noOfBeeHives;
	static float (*beeHiveCoords)[2];
	static bool *beeHiveCreated;
	static bool *beeHiveDestroyed;

	if (firstCall) {
		honeycombHeight = honeycombheight;
		honeycombWidth = honeycombwidth;
		beeHiveWidth = beehivewidth;
		beeHiveHeight = beehiveheight;

		noOfRegularHoneycombs = noofregularhoneycombs;
		noOfRedHoneycombs = noofredhoneycombs;
		noOfBeeHives = noofbeehives;

		regularHoneycombCoords = regularhoneycombcoords;
		redHoneycombCoords = redhoneycombcoords;
		beeHiveCoords = beehivecoords;

		regularHoneycombCreated = regularhoneycombcreated;
		regularHoneycombDestroyed = regularhoneycombdestroyed;
		redHoneycombCreated = redhoneycombcreated;
		redHoneycombDestroyed = redhoneycombdestroyed;
		beeHiveCreated = beehivecreated;
		beeHiveDestroyed = beehivedestroyed;
		return true;
	} else {

		if (isCollidingWithMultipleSprites(spriteX, spriteY, spriteWidth, spriteHeight, regularHoneycombCoords, regularHoneycombCreated, regularHoneycombDestroyed, honeycombWidth, honeycombHeight, noOfRegularHoneycombs, spriteIndex, destroyCollidedObject) ||
			isCollidingWithMultipleSprites(spriteX, spriteY, spriteWidth, spriteHeight, redHoneycombCoords, redHoneycombCreated, redHoneycombDestroyed, honeycombWidth, honeycombHeight, noOfRedHoneycombs, spriteIndex, destroyCollidedObject) ||
			isCollidingWithMultipleSprites(spriteX, spriteY, spriteWidth, spriteHeight, beeHiveCoords, beeHiveCreated, beeHiveDestroyed, beeHiveWidth, beeHiveHeight, noOfBeeHives, spriteIndex, destroyCollidedObject)){
			return true;
		}
	}

	return false;
}




// looks through an array of sprites to see if they collide with the sprite in question
// only works for arrays with same type of sprites
bool isCollidingWithMultipleSprites(float sprite1X, float sprite1Y, int sprite1Width, int sprite1Height, float (*sprite2Coords)[2], bool *sprite2Created, bool *sprite2Destroyed, int sprite2Width, int sprite2Height, int sprite2Length, int sprite1Index, bool destroyCollidedObject) {
	for (int i = 0; i < sprite2Length; i++) {
		
		// if same elemnt, no collision
		if (sprite1Width == sprite1Width && sprite1Width == sprite2Width && i == sprite1Index) {
			return false;

		} else if (*(sprite2Created + i) && !*(sprite2Destroyed + i) && areColliding(sprite1X, sprite1Y, sprite1Width, sprite1Height, **(sprite2Coords + i), *(*(sprite2Coords + i) + 1), sprite2Width, sprite2Height)) {
			if (destroyCollidedObject) {
				*sprite2Destroyed = true;
			}
			return true;
		}
	}
	return false;
}




// draws all the bee Hives
void drawBeeHives(RenderWindow &window, Sprite hiveSprites[], float beeHiveCoords[][2], bool isHiveCreated[], bool isHiveDestroyed[], int noOfBeeHives) {
	for (int i = 0; i < noOfBeeHives; i++) {
		if (isHiveCreated[i] && !isHiveDestroyed[i]) {
			hiveSprites[i].setPosition(beeHiveCoords[i][0], beeHiveCoords[i][1]);
			window.draw(hiveSprites[i]);
		}
	}
}




void drawRemainingBottles(RenderWindow &window, float startingX, float startingY, Texture bottleTexture, int remainingBottles, int bottleWidth, int bottleHeight) {
	for (int i = 0; i < remainingBottles; i++) {
		Sprite bottle;
		bottle.setTexture(bottleTexture);
		// bottle.setTextureRect(sf::IntRect(0, 0, bottleWidth, bottleHeight));
		bottle.setPosition(startingX, startingY);
		window.draw(bottle);
		startingX += bottleWidth;
	}
}


void drawHummingBird(RenderWindow &window, Sprite hummingBird, float hummingBirdX, float hummingBirdY) {
	hummingBird.setPosition(hummingBirdX, hummingBirdY);
	window.draw(hummingBird);
}


// this has all the functionality of hummingbird
// honeycombFound and onHoneycomb are used for both types of honeycombs and bee hives
void manageHummingBird(Sprite &hummingBird, float &hummingBirdX, float &hummingBirdY, int hummingBirdWidth, int hummingBirdHeight, int hummingBirdStepValue, bool &isHummingBirdSick, int &honeycombsSucked, float bulletX, float &bulletY, int bulletWidth, int bulletHeight, bool bulletExists, int hummingBirdHealingTime, float hummingBirdSpeed, float hummingBirdRestX, float hummingBirdRestY, int nectarSuckTime) {

	static int hitByBullet = 0;
	static Clock hummingBirdSickTimer;
	static Clock hummingBirdSuckingTimer;
	static Clock hummingBirdMoveTimer;
	static Clock hummingBirdHealingTimer;
	static int currentHoneycombIndex;
	static bool spriteFound = false;
	static bool onSprite = false;



	// states of the current sprite 
	static float spriteX;
	static float spriteY;
	static int spriteWidth;
	static int spriteHeight;
	static bool *spriteCreated = nullptr;
	static bool *spriteDestroyed = nullptr;

	// if sick, it doesn't do anything else
	if (isHummingBirdSick) {

		float outOfScreenX = resolutionX + 1;
		float outOfScreenY = resolutionY + 1;

		if (areColliding(hummingBirdX, hummingBirdY, hummingBirdWidth, hummingBirdHeight, 0, 0, resolutionX, resolutionY) && hummingBirdMoveTimer.getElapsedTime().asSeconds() >= 1 / hummingBirdSpeed) {
			moveHummingBirdToPoint(hummingBird, hummingBirdX, hummingBirdY, outOfScreenX, outOfScreenY, hummingBirdWidth, hummingBirdHeight, hummingBirdStepValue);
			if (!areColliding(hummingBirdX, hummingBirdY, hummingBirdWidth, hummingBirdHeight, 0, 0, resolutionX, resolutionY)) {
				hummingBirdHealingTimer.restart();
			}
			hummingBirdMoveTimer.restart();
		// if healed
		} else if (hummingBirdSickTimer.getElapsedTime().asSeconds() >= hummingBirdHealingTime) {
			hummingBird.setColor(Color::Blue);
			isHummingBirdSick = false;
			hitByBullet = 0;
		}
		return;
	}


	// if its colliding with a bullet, the counter is increased
	bool isCollidingWithBullet = areColliding(hummingBirdX, hummingBirdY, hummingBirdWidth, hummingBirdHeight, bulletX, bulletY, bulletWidth, bulletHeight);
	if (isCollidingWithBullet && bulletExists) {
		hitByBullet++;
		bulletY -= bulletHeight + hummingBirdHeight;

		if (hitByBullet == 3) {
			spriteFound = false;
			hummingBird.setColor(Color::Green);
			hummingBirdMoveTimer.restart();
			isHummingBirdSick = true;
			hummingBirdSickTimer.restart();
			return;
		}
	}

	// if bullet hits it or something happens where the honeycomb is not longer present
	if (spriteFound && *spriteDestroyed) {
		spriteFound = false;
	}

	// if no honeycombs, look for one
	if (!spriteFound) {
		if (findSpriteForHummingBird(spriteX, spriteY, spriteWidth, spriteHeight, spriteCreated, spriteDestroyed)) {
			// cout << spriteX << "\t" << spriteY << endl;
			spriteFound = true;
			onSprite = false;
					hummingBirdMoveTimer.restart();
			}

	}

	if (spriteFound) {
		if (onSprite) {
			if (hummingBirdSuckingTimer.getElapsedTime().asSeconds() > nectarSuckTime){
				// increase score here
				hummingBirdMoveTimer.restart();
				*spriteDestroyed = true;
				spriteFound = false;
				honeycombsSucked++;
			}
		} else {
			if (hummingBirdMoveTimer.getElapsedTime().asSeconds() >= 1 / hummingBirdSpeed) {
				float hummingBirdMidX = midCoordinate(hummingBirdX, hummingBirdWidth);
				float hummingBirdMidY = midCoordinate(hummingBirdY, hummingBirdHeight);

				moveHummingBirdToPoint(hummingBird, hummingBirdMidX, hummingBirdMidY , midCoordinate(spriteX, spriteWidth), midCoordinate(spriteY, spriteHeight), hummingBirdWidth, hummingBirdHeight, hummingBirdStepValue);
				hummingBirdX += hummingBirdMidX - midCoordinate(hummingBirdX, hummingBirdWidth);
				hummingBirdY += hummingBirdMidY - midCoordinate(hummingBirdY, hummingBirdHeight);
				hummingBirdMoveTimer.restart();

			}
			
			if (areMidCoordinatesSame(hummingBirdX, hummingBirdY, hummingBirdWidth, hummingBirdHeight, spriteX, spriteY, spriteWidth, spriteHeight)) {
				onSprite = true;
				hummingBirdSuckingTimer.restart();
			}
		}

		return;
	} else {
		if (hummingBirdMoveTimer.getElapsedTime().asSeconds() >= 1 / hummingBirdSpeed) { 
			moveHummingBirdToPoint(hummingBird, hummingBirdX, hummingBirdY, hummingBirdRestX, hummingBirdRestY, hummingBirdWidth, hummingBirdHeight, hummingBirdStepValue);
			hummingBirdMoveTimer.restart();
		}
	}


}


void moveHummingBirdToPoint(Sprite &hummingBird, float &hummingBirdX, float &hummingBirdY, float destinationX, float destinationY, int hummingBirdWidth, int hummingBirdHeight, float stepValue) {
	int textureStartIndex = hummingBirdWidth;
	if (moveSpriteToPoint(hummingBirdX, hummingBirdY , destinationX, destinationY, stepValue)) {
		textureStartIndex = 0;
	}
	hummingBird.setTextureRect(sf::IntRect(textureStartIndex, 0, hummingBirdWidth, hummingBirdHeight));
}


// used for honeycombs and beeHives
void destroyObject(bool objectDestroyed[], float objectCoords[][2], bool objectCreated[], int noOfObjects, int objectWidth, int objectHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int bulletHeight, bool bulletExists) {
	for (int i = 0; i < noOfObjects; i++) {
		if (!objectDestroyed[i] && objectCreated[i]) {
			bool isHoneycombNBulletColliding = areColliding(objectCoords[i][0], objectCoords[i][1], objectWidth, objectHeight, bullet_x_coordinate, bullet_y_coordinate, bulletWidth, bulletHeight);
			if (isHoneycombNBulletColliding && bulletExists) {
				objectDestroyed[i] = true;
			}
		}
	}
}

void drawFlowers(RenderWindow& window, Sprite flowers[], float startIndex[][2], bool isFlowerPollinated[], int totalFlowers, int groundY, int flowerHeight) {
	for (int i = 0; i < totalFlowers; i++) {
		if (isFlowerPollinated[i]) {
			flowers[i].setPosition(startIndex[i][0], startIndex[i][1]);
			window.draw(flowers[i]);
		}
	}
}


void beePollinatesGround(Sprite bees[], bool beesAlive[], bool beesHavePollinated[], float beesCoords[][2], bool areBeesMovingRight[], int noOfBees, int beeHeight, int beeWidth, float flowerStartIndex[][2], bool isFlowerPollinated[], int totalFlowers, int flowerWidth, int flowerHeight, float &playerX, float &playerY, float &backgroundX, int playerWidth, int playerHeight, int groundY, int beeRowHeight) {
	for (int i = 0; i < noOfBees; i++) {
		// if in the air, no pollination anyway or dead
		if (beesCoords[i][1] + beeHeight < groundY - beeRowHeight || !beesAlive[i]) {
			continue;
		}

		int flowerIndex;
		// looping throuhg flowers
		for (flowerIndex = 0; flowerIndex < totalFlowers; flowerIndex++) {
			bool isBeeCollidingWithFirstFlower = areColliding(beesCoords[i][0], beesCoords[i][1], beeWidth, beeHeight, flowerStartIndex[flowerIndex][0], groundY - beeRowHeight, flowerWidth, 1000);
			
			if (flowerIndex < totalFlowers - 1) {
				bool isBeeCollidingWithSecondFlower = areColliding(beesCoords[i][0], beesCoords[i][1], beeWidth, beeHeight, flowerStartIndex[flowerIndex + 1][0], groundY - beeRowHeight, flowerWidth, 1000);
				// if bee is colliding with flowers
			if (isBeeCollidingWithFirstFlower && isBeeCollidingWithSecondFlower) {
				if (isFlowerPollinated[flowerIndex + 1] && !isFlowerPollinated[flowerIndex]) {
					break;
				} else if (!isFlowerPollinated[flowerIndex] && !isFlowerPollinated[flowerIndex + 1]) {
					float beeMid = beesCoords[i][0] + (beeWidth / 2);
						if (beeMid > flowerStartIndex[flowerIndex + 1][0]) {
						flowerIndex++;
					}
					break;
				}
				}
			// is flower is not pollinated
			} else if (isBeeCollidingWithFirstFlower && !isFlowerPollinated[flowerIndex]) {
				break;
			}
		}

		// one pollination happens, they go back unless on the first or last flower
		if (flowerIndex < totalFlowers && !beesHavePollinated[i]) {
			isFlowerPollinated[flowerIndex] = true;
			if (!(flowerIndex == totalFlowers - 1 || !flowerIndex)) {
				beesHavePollinated[i] = true;
				areBeesMovingRight[i] = !areBeesMovingRight[i];
			}
		}	
	}

	bool falseArr[totalFlowers] = {};
	bool isPlayerCollidingWithFlowers = isCollidingWithMultipleSprites(playerX, playerY, playerWidth, playerHeight, flowerStartIndex, isFlowerPollinated, falseArr, flowerWidth, flowerHeight, totalFlowers, -1, false);

	// if the player is in the middle of the flowers, move
	if (isPlayerCollidingWithFlowers) {
		int pixel;
		for (pixel = 0; pixel < resolutionX - playerWidth; pixel++) {
			bool isFreeSpace = isCollidingWithMultipleSprites(pixel, playerY, playerWidth, playerHeight, flowerStartIndex, isFlowerPollinated, falseArr, flowerWidth, flowerHeight, totalFlowers, -1, false);
			if (isFreeSpace) {
				int temp = playerX;
				playerX = pixel;
				backgroundX += playerX - temp;
				break;
			}

		}

		// if couldn't move, then there was no space
		if (!(pixel < resolutionX - playerWidth - 1)) {
			for (int i = 0; i < totalFlowers; i++) {
				isFlowerPollinated[i] = false;
			}
		}
	}
}

// set the coordinates of the honeycombs and draw
void drawHoneycombs(RenderWindow &window, Sprite honeycombs[], float honeycombCoords[][2], bool isHoneycombCreated[], bool hasPollinated[], float beesCoords[][2], bool honeycombDestroyed[], int noOfHoneycombs, int beeHeight, int beeWidth, int honeycombHeight, int honeycombWidth) {
	for (int i = 0; i < noOfHoneycombs; i++) {
		if (isHoneycombCreated[i] && !honeycombDestroyed[i]) {
			// displaying honeycombs
			honeycombs[i].setPosition(honeycombCoords[i][0], honeycombCoords[i][1]);
			window.draw(honeycombs[i]);
		}
	}
}


// bees die if they touch the ground or a bullet
void killBees(Sprite bees[], bool beesAlive[], float beesCoords[][2], bool beesHavePollinated[], bool isHoneycombCreated[], bool isHoneycombDestroyed[], float honeycombCoords[][2], int beesSpawned, int beeWidth, int beeHeight, float bullet_x_coordinate, float &bullet_y_coordinate, int bulletWidth, int bulletHeight, bool bulletExists, int honeycombWidth, int honeycombHeight, int ground_y_coordinate) {
	for (int i = 0; i < beesSpawned; i++) {
		if (beesAlive[i]) {
			bool bulletCollided = false;
			beesAlive[i] = killBee(bees[i], beesCoords[i][0], beesCoords[i][1], beesHavePollinated[i], beeWidth, beeHeight, bullet_x_coordinate, bullet_y_coordinate, bulletWidth, bulletHeight, ground_y_coordinate, bulletExists, bulletCollided);
			isHoneycombCreated[i] = bulletCollided;


			// creating honeycomb if bullet has collided
			if (bulletCollided) {
				honeycombCoords[i][0] = adjustedStartingIndexDifference(beesCoords[i][0], beeWidth, honeycombWidth, resolutionX);
				honeycombCoords[i][1] = adjustedStartingIndexDifference(beesCoords[i][1], beeHeight, honeycombHeight, resolutionY);
				bullet_y_coordinate =  honeycombCoords[i][1] - bulletHeight;

				// checking if any honeycombs are overlapping with the current honeycomb. if they are, then honeycomb isn't created
				if (!staticSpritesCollision(honeycombCoords[i][0], honeycombCoords[i][1], honeycombWidth, honeycombHeight, false)) {
					isHoneycombCreated[i] = true;
				}
			}

		}
	}
}

// returns either true or false. true if bee dies. false if bee is alive
bool killBee(Sprite bee, float bee_x_coordinate, float bee_y_coordinate, bool beeHasPollinated, int beeWidth, int beeHeight, float bullet_x_coordinate, float bullet_y_coordinate, int bulletWidth, int bulletHeight, int ground_y_coordinate, bool bulletExists, bool &bulletCollided) {
	bool isCollidingWithBullet = areColliding(bee_x_coordinate, bee_y_coordinate, beeWidth, beeHeight, bullet_x_coordinate, bullet_y_coordinate, bulletWidth, bulletHeight);
	if (isCollidingWithBullet && bulletExists) {
			bulletCollided = true;
			return false;

		// of if it goes into the ground
	} else if (bee_y_coordinate + beeHeight > ground_y_coordinate) {
		return false;
	
	}
	return true;
}

// sets bee to alive and returns the initial x coordinate
void spawnBees(Sprite bees[], float beesCoords[][2], Clock beesClock[], bool beesAlive[], bool areBeesMovingRight[], int &beesSpawned, int totalBees, float delay, float offset, int beeWidth, int beeHeight) {
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
		// beesCoords[beesSpawned][0] = rand() % 2 ? 0: resolutionX - beeWidth; // based on a random number generated, the initial coordinate is set
		beesCoords[beesSpawned][0] = 0;
		beesCoords[beesSpawned][1] = 450;
		timeSinceLastSpawn.restart();

		// if at the top left corner, moving right otherwise moving left
		if (!beesCoords[beesSpawned][0]) {
			areBeesMovingRight[beesSpawned] = true;
			bees[beesSpawned].setTextureRect(sf::IntRect(0, 0, beeWidth, beeHeight));
		} else {
			bees[beesSpawned].setTextureRect(sf::IntRect(beeWidth, 0, -beeWidth, beeHeight));
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
void moveBees(Sprite bees[], int noOfBees, float beesCoords[][2], bool areBeesMovingRight[], Clock beesClock[], bool beesAlive[], bool hasPollinated[], bool honeycombCreated[], float honeycombCoords[][2], bool honeycombDestroyed[], bool areBeeHivesCreated[], float beeHiveCoords[][2], int beeMovementValue, float beeSpeed, int beeHeight, int beeWidth, bool areRegular, int honeycombHeight, int honeycombWidth, int noOfHoneycombs, float playerX, float playerY, int playerWidth, int playerHeight, int beeHiveWidth, int beeHiveHeight, int beeRowHeight, int groundY) {
	for (int i = 0; i < noOfBees; i++) {
		// if its the movement time
		if (beesAlive[i] && beesClock[i].getElapsedTime().asSeconds() >= 1 / beeSpeed) {
			moveBee(bees[i], beesCoords[i][0], beesCoords[i][1], areBeesMovingRight[i], beesAlive[i], areBeeHivesCreated[i], beeHiveCoords[i][0], beeHiveCoords[i][1], honeycombCoords, hasPollinated, honeycombCreated, honeycombDestroyed, beeMovementValue, beeHeight, beeWidth, areRegular, noOfBees, honeycombHeight, honeycombWidth, noOfHoneycombs, playerX, playerY, playerHeight, beeHiveWidth, beeHiveHeight, playerWidth, beeRowHeight, groundY);
			beesClock[i].restart();
		}

	}
}

// handles bee hive creation
void moveBee(Sprite &bee, float &x_coordinate, float &y_coordinate, bool &isMovingRight, bool &isBeeAlive, bool &isBeeHiveCreated, float &beeHiveX, float &beeHiveY, float honeycombCoords[][2], bool havePollinated[], bool honeycombCreated[], bool honeycombDestroyed[], int beeMovementValue, int beeHeight, int beeWidth, int isRegular, int noOfBees, int honeycombHeight, int honeycombWidth, int noOfHoneycombs, float playerX, float playerY, int playerWidth, int playerHeight, int beeHiveWidth, int beeHiveHeight, int beeRowHeight, int groundY) {
	bool directionChanged = false;

	float x_coordinateAfterMovement = moveBeeX(x_coordinate, isMovingRight, beeMovementValue);

	bool isBeeCollidingWithPlayer = areColliding(x_coordinateAfterMovement, y_coordinate, beeWidth, beeHeight, playerX, playerY, playerWidth, playerHeight);
	bool isBeeCollidingWithLeftWall = areColliding(x_coordinateAfterMovement, y_coordinate, beeWidth, beeHeight, -10, 0, 9, resolutionY);
	bool isBeeCollidingWithRightWall = areColliding(x_coordinateAfterMovement, y_coordinate, beeWidth, beeHeight, resolutionX + 1, 0, 1000, resolutionY);
	
	
	// looking for any collision with honeycombs
	bool isBeeCollidingWithObstacle = staticSpritesCollision(x_coordinateAfterMovement, y_coordinate, beeWidth, beeHeight);
	if (isBeeCollidingWithObstacle) {
		isMovingRight = !isMovingRight;


		int extremeX = isMovingRight ? resolutionX - beeWidth: 0;
		int changeinX = isMovingRight? beeMovementValue: -beeMovementValue;
		bool foundObstacle;
		int x;

		// colliding with honeycomb, looking for honeycombs below to see if the bee is trapped
		for (x = x_coordinate; extremeX? x <= extremeX: x >= 0; x += changeinX) {
			foundObstacle = false;
			for (int j = 0; j < noOfHoneycombs; j++) {
				if (areColliding(x, y_coordinate + beeRowHeight, beeWidth, beeHeight, honeycombCoords[j][0], honeycombCoords[j][1], honeycombWidth, honeycombHeight)) {
					foundObstacle = true;
					break;
				}
			}

			if (!foundObstacle) {
				break;
			}

		}

		// if no way out is found, foundObstacle is true. If foundObstacle is true,
		// program looks for walls
		if (foundObstacle) {
			bool leftWall = areColliding(x + changeinX, y_coordinate, beeWidth, beeHeight, -1000, 0, 1000, resolutionY);
			bool rightWall = areColliding(x + changeinX, y_coordinate, beeWidth, beeHeight, resolutionX + 1, 0, 1000, resolutionY);
			// if bee is also surrounded by wall, hive is formed
			if (leftWall || rightWall) {
				isBeeAlive = false;
				
				beeHiveX = adjustedStartingIndexDifference(x_coordinate, beeWidth, beeHiveWidth, resolutionX);
				beeHiveY = adjustedStartingIndexDifference(y_coordinate, beeHeight, beeHiveHeight, resolutionY);
				staticSpritesCollision(beeHiveX, beeHiveY, beeHiveWidth, beeHiveHeight, true);

				isBeeHiveCreated = true;
			}
			return;
		}

		// direction is changed regardless of be is trapped or not
		directionChanged = true;

	}


	if (isBeeCollidingWithRightWall && isMovingRight) {
		directionChanged = true;
		isMovingRight = !isMovingRight;
		x_coordinate = resolutionX;

	} else if (isBeeCollidingWithLeftWall && !isMovingRight) {
		directionChanged = true;
		isMovingRight = !isMovingRight;
		x_coordinate = -beeWidth;
	// } else if (isBeeCollidingWithPlayer) {
	// 	directionChanged = true;
	// 	isMovingRight = !isMovingRight;
	// 	return;
	}


	x_coordinateAfterMovement = moveBeeX(x_coordinate, isMovingRight, beeMovementValue);


	// isBeeCollidingWithPlayer = areColliding(x_coordinateAfterMovement, y_coordinate, beeWidth, beeHeight, playerX, playerY, playerWidth, playerHeight);
	isBeeCollidingWithPlayer = false;


	// if direction is changed and player is not hit move down
	if (directionChanged) {
		y_coordinate += beeRowHeight;
	} else {
		x_coordinate = x_coordinateAfterMovement;
	}

	if (isMovingRight) {
		bee.setTextureRect(sf::IntRect(beeWidth, 0, -beeWidth, beeHeight));
	} else {
		bee.setTextureRect(sf::IntRect(0, 0, beeWidth, beeHeight));
	}


}

float moveBeeX(float x_coordinate, bool isMovingRight, int beeMovementValue) {
	if (isMovingRight) {
		return x_coordinate + beeMovementValue;
	} else {
		return x_coordinate - beeMovementValue;
	}
}

// governs the movmeent of the player. stops player from moving where they shouldn't
int movePlayer(float player_x, int playerWidth, int playerMovementValue, bool isRight, float flowersCoords[][2], bool flowersPollinated[], int flowerWidth, int noOfFlowers) {
	float valueAfterMovement = player_x;
	if (isRight) {
		// adding playerWidth so that we get the right border of the sprite
		if (player_x + playerWidth + playerMovementValue > resolutionX) {
			valueAfterMovement = resolutionX - playerWidth;
		} else {
			valueAfterMovement += playerMovementValue;
		}
	} else {
		if (player_x - playerMovementValue < 0) {
			valueAfterMovement = 0;
		} else {
			valueAfterMovement -= playerMovementValue;
		}
	}
	bool falseArr[noOfFlowers] = {};
	bool isPlayerCollidingWithFlowers = isCollidingWithMultipleSprites(valueAfterMovement, 0, playerWidth, resolutionY, flowersCoords, flowersPollinated, falseArr, flowerWidth, resolutionY, noOfFlowers, -1, false);
	// if player is moving in the flowers, don't let them
	if (isPlayerCollidingWithFlowers) {
		return player_x;
	}
	return valueAfterMovement;

}


bool allTrue(bool arr[], int size) {
	for (int i = 0; i < size; i++) {
		if (!arr[i]) {
			return false;
	}
	}
	return true;
}

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, float backgroundX, float backgroundY, int backgroundOffsetX, int backgroundOffsetY, int backgroundWidth, int backgroundHeight,  Sprite& backgroundSprite) {
	playerSprite.setPosition(player_x, player_y);
	window.draw(playerSprite);
	backgroundSprite.setPosition(backgroundX + backgroundOffsetX, backgroundY + backgroundOffsetY);
	backgroundSprite.setTextureRect(sf::IntRect(backgroundOffsetX, backgroundOffsetY, backgroundWidth - backgroundOffsetX, backgroundHeight - backgroundOffsetY));
	window.draw(backgroundSprite);
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


// this function adjusts the coordinate index and also handles edge cases
float adjustedStartingIndexDifference(float startingOld, int jumpOld, int jumpNew, int extremeCoordinate) {
	float startingIndex = startingIndexDifference(startingOld, jumpOld, jumpNew);

	return moveInsideScreen(startingIndex, extremeCoordinate, jumpNew);
}


float moveInsideScreen(float coordinate, int extremeCoordinate, int jump) {
	if (coordinate < 0) {
		coordinate = 0;
	} else if (coordinate + jump > extremeCoordinate) {
		coordinate = extremeCoordinate - jump;
	}

	return coordinate;
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

bool areMidCoordinatesSame(int sprite1_x, int sprite1_y, int sprite1Width, int sprite1Height, int sprite2_x, int sprite2_y, int sprite2Width, int sprite2Height) {
	if (midCoordinate(sprite1_x, sprite1Width) == midCoordinate(sprite2_x, sprite2Width)) {
		if (midCoordinate(sprite1_y, sprite1Height) == midCoordinate(sprite2_y, sprite2Height)) {
			return true;
		}
	}

	return false;
}


float midCoordinate(float startCoordinate, int jump) {
	return startCoordinate + (jump / 2);
}

// returns false if reached the destination
bool moveSpriteToPoint(float &spriteX, float &spriteY, float destinationX, float destinationY, float stepValue) {
	float totalChangeInX = destinationX - spriteX;
	float totalChangeInY = destinationY - spriteY;
	float distance = sqrt(pow(totalChangeInX, 2) + pow(totalChangeInY, 2));
	float unitVectorX = totalChangeInX / distance;
	float unitVectorY = totalChangeInY / distance;


	if (distance) {
		if (distance < stepValue) {
			spriteX = destinationX;
			spriteY = destinationY;
			return false;
		} else {
			spriteX += stepValue * unitVectorX;
			spriteY += stepValue * unitVectorY;
		}
		return true;
	}

	return false;
}


// when game is initialized with honeycombs or hives, this function is used
void initializeSprites(Texture &spriteTexture, Sprite sprites[], float spriteCoords[][2], int noOfSprites, int noOfAdditionalSprites, bool isSpriteCreated[], int spriteWidth, int spriteHeight, int groundY, int playerHeight) {
	for (int i = 0; i < noOfSprites; i++) {
		sprites[i].setTexture(spriteTexture);
		sprites[i].setTextureRect(sf::IntRect(0, 0, spriteWidth, spriteHeight));
		
		int additionalSpritesIndex = noOfSprites - noOfAdditionalSprites;
		if (i >= additionalSpritesIndex) {
			bool collidingWithOtherSprites, isAboveTheGroundEnough;
			do {
				spriteCoords[i][0] = moveInsideScreen((rand() % resolutionX - (2 * spriteWidth)) + spriteWidth, resolutionX, spriteWidth);
				spriteCoords[i][1] = moveInsideScreen((rand() % resolutionY - (2 * spriteHeight)) + spriteWidth, resolutionY, spriteWidth);
				collidingWithOtherSprites = staticSpritesCollision(spriteCoords[i][0], spriteCoords[i][1], spriteWidth, spriteHeight, false);
				isAboveTheGroundEnough = !areColliding(spriteCoords[i][0], spriteCoords[i][1], spriteWidth, spriteHeight, 0, groundY - playerHeight - 70, resolutionX, resolutionY - (groundY - playerHeight));
			} while (collidingWithOtherSprites || !isAboveTheGroundEnough);
			isSpriteCreated[i] = true;
		}
	}

}