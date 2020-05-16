#include <iostream>
#include <sstream> // для строк в потоке
#include <SFML/Graphics.hpp>
#include "Platformer.h"
#include "camera.h"
#include "speech.h"

using namespace sf;
using namespace std;
/*
class Character {
public: 
	float dx, dy, w, h,x,y,speed,moveTimer;
	int w, h, health;
	bool life, playerOnground;
	Texture texture;
	Sprite sprite;
	String name;
	Character(Image &image,  float X, float Y, int W, int H, String Name) {
		x = X;
		y = y;
		w = W;
		h = H;
		name = Name;
		moveTimer = 0;
		speed = 0;
		health = 100;
		dx = 0; dy = 0;
		life = true;
		playerOnground = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
};*/

class Player { // класс игрока
private:
	float x, y; // координаты игрока,используются только через сеттеры
public:
	float dx, dy, w, h; // перемещение по осям , высота и ширина спрайта игрока
	float speed = 0 ; // изначальная скорость игрока
	int score; // счет 
	bool life;
	bool playerOnGround;
	int health;
	enum  statePlayer
	{
		left,right,up,down,jump,stay
	};
	statePlayer state;
	String File; // здесь хранится имя файла
	Sprite sprite; // спрайт игрока
	Image image; // картинка для спрайта
	Texture texture; // текстура для спрайта
	Player(String F, int X, int Y, float W, float H) { //класс конкструктор

		File = F;
		w = W;
		h = H;
		score = 0;
		health = 100;
		dx = 0;
		dy = 0;
		life = true;
		playerOnGround = false;
		state = stay;
		image.loadFromFile("../../Sprites/PNG/" + File);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X;
		y = Y;
		sprite.setTextureRect(IntRect(0,8, w, h)); // создаем персонажа 
		sprite.setOrigin(w / 2, h / 2);
	}
	void update(float time) { // функция обновления картинки игры, привязана к сфмл времени, не к процессору, что бы избежать разной скорости игры
		
		playercontrol();


		switch (state)
		{
		case right: dx = speed; 
			break;
		case left: dx = -speed;
			break;
		case up: 
			break;
		case down:
			break;
		case stay: 
			break;
		
		}
		
	
		x += dx * time; // перемещение координаты х за время,когда задаем расстояние перемещения за время, получаем новую координату
		checkCollision(dx, 0);
		y += dy * time; // то же, что и с иксом
		checkCollision(0, dy);
		sprite.setPosition(x + w / 2, y + h / 2); // запоминаем координаты
		if (health <= 0)
		{
			life = false;
		}
		speed = 0; //после того как двигались - возвращаем скорость 0
	
		dy = dy + 0.0015 * time;

		
		//physicsmap(); // функция коллизии игрока и предметов
	}
	void playercontrol() {
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			state = left;
			speed = 0.1;
			//currentframe += 0.007 * time;
			//if (currentframe > 3) currentframe -= 3;
			//Dino.sprite.setTextureRect(IntRect(66 * int(currentframe) + 66, 113, -66, 91));

		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			state = right;
			speed = 0.1;
			//currentframe += 0.005 * time;
			//if (currentframe > 3) currentframe -= 3;
			//Dino.sprite.setTextureRect(IntRect(66 * int(currentframe), 113, 66, 91));

		}
		if ((Keyboard::isKeyPressed(Keyboard::W)) && (playerOnGround))
		{
			
			state = jump;
			dy = -0.5;
			playerOnGround = false;
			//currentframe += 0.005 * time;
			//if (currentframe > 5) currentframe -= 5;
			//Dino.sprite.setTextureRect(IntRect(74 * int(currentframe), 222, 74, 96));

		}
		
	}
	float getplayerx() { // сеттер на координату х для камеры и игрока
		return x;
	}
	float getplayery() { //сеттер на координату у для камеры и игрока
		return y;
	}
	float getplayerdx() {
		return dx;
	}
	float getplayerdy() {
		return dy;
	}
	void checkCollision(float Dx, float Dy) {
		for (int i = y / 64; i < (y + h) / 64; i++) // проходимся по всем игрикам, /64 потому что берем самый левый квадратик, потом самый правый при условии меньше
			for (int j = x / 64; j < (x + w) / 64; j++) // тоже самое,но для иксов
			{
				if (TileMap[i][j] == 'b' || TileMap [i][j] == '0')  // если элемент карты такой-то
				{
					if (Dy > 0)
					{
						y = i * 64 - h; // не даем войти в текстуру,когда перемещаемся вниз, просто отбрасывая персонажа на его высоту
						dy = 0;
						playerOnGround = true;

					}

					if (Dy < 0)
					{
						y = i * 64 + 64; //тоже самое дальше для всех направлений
						dy = 0;
					}
					if (Dx > 0)
					{
						x = j * 64 - w;
					}
					if (Dx < 0)
					{
						x = j * 64 + 64;
					}
					
				}
				if (TileMap[i][j] == 'c')
				{
					
					score++;
					TileMap[i][j] = ' ';
					
				}
				if (TileMap[i][j] == 'h')
				{
					health = health + 20;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'p')
				{
					health = health - 40;
					TileMap[i][j] = ' ';
				}
			}
	}

	
	};

int main()
{
	RenderWindow window(VideoMode(1280, 960), "Dinotaur");
	camera.reset(FloatRect(0, 0, 1280, 960));
	Image map_image;
	map_image.loadFromFile("../../Sprites/PNG/Mainsprites.png");
	Font pixelfont;
	pixelfont.loadFromFile("slkscreb.ttf");
	Text textscore("",pixelfont, 25);
	Text texthealth("", pixelfont, 25);
	Text textmission("", pixelfont, 15);
	textmission.setFillColor(Color::Black);

	

	
	Texture map;
	map.loadFromImage(map_image);
	Sprite mainmap;
	mainmap.setTexture(map);

	Image speechBubble;
	speechBubble.loadFromFile("../../Sprites/PNG/speech.png");
	Texture speechBubbleTexture;
	speechBubbleTexture.loadFromImage(speechBubble);
	Sprite speechBubbleSprite;
	speechBubbleSprite.setTexture(speechBubbleTexture);
	speechBubbleSprite.setTextureRect(IntRect(139,74,128,122));


	Player Dino("DinoSpriteDoux.png",65, 295, 65.0, 90.0);
	float currentframe = 0;
	bool showLeveltext = true;
	Clock gametime; // привязка ко времени сфмл, а не к процессору

	while (window.isOpen()) // основной цикл программы
	{
		float time = gametime.getElapsedTime().asMicroseconds();
		gametime.restart();
		time = time / 500;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Q)
				{

						switch (showLeveltext)
						{

						case true: {
							ostringstream task;
							task << textlevel(currentlevel(Dino.getplayerx()));
							textmission.setString(task.str());
							textmission.setPosition(camera.getCenter().x, camera.getCenter().y);
							speechBubbleSprite.setPosition(camera.getCenter().x, camera.getCenter().y);
							showLeveltext = false;
							break;
						}

						case false: {
							textmission.setString("");
							showLeveltext = true;
							break;
						}
						}
					
			} 

		}
		float coordinatePlayerX, coordinatePlayerY;
		coordinatePlayerX = Dino.getplayerx();
		coordinatePlayerY = Dino.getplayery();
		if (Dino.life == true)
		{
			getPlayerView(Dino.getplayerx(), Dino.getplayery());
		}

		if (Keyboard::isKeyPressed(Keyboard::A)) {
			currentframe += 0.007 * time;
		if (currentframe > 3) currentframe -= 3;
		Dino.sprite.setTextureRect(IntRect(66 * int(currentframe) + 66, 113, -66, 91));
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			currentframe += 0.007 * time;
			if (currentframe > 3) currentframe -= 3;
			Dino.sprite.setTextureRect(IntRect(66 * int(currentframe), 113, 66, 91));
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			currentframe += 0.001 * time;
			if (currentframe > 5) currentframe -= 5;
			Dino.sprite.setTextureRect(IntRect(74 * int(currentframe), 222, 74, 96));
		}
		if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::D))
		{
			currentframe += 0.0007 * time;
			if (currentframe > 5) currentframe -= 5;
			Dino.sprite.setTextureRect(IntRect(74 * int(currentframe), 222, 74, 96));
			

		}
		if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::A))
		{
			currentframe += 0.0007 * time;
			if (currentframe > 5) currentframe -= 5;
			Dino.sprite.setTextureRect(IntRect(74 * int(currentframe)+74, 222, -74, 96));


		}
		
		Dino.update(time);
		window.setView(camera);
			
		window.clear();

		for (int i = 0; i < height_map; i++) //отрисовываем карту
			for (int j = 0; j < width_map; j++)
			{
				if (TileMap[i][j] == ' ')  mainmap.setTextureRect(IntRect(0, 0, 64, 64)); //задаем текстуры
				if (TileMap[i][j] == '0') mainmap.setTextureRect(IntRect(64, 48, 64, 64));
				if (TileMap[i][j] == 'b') mainmap.setTextureRect(IntRect(144, 48, 64, 64));
				if (TileMap[i][j] == 'h') mainmap.setTextureRect(IntRect(232,68,32,32));
				if (TileMap[i][j] == 'c') mainmap.setTextureRect(IntRect(297,81,42,30));
				if (TileMap[i][j] == 'p') mainmap.setTextureRect(IntRect(363,62,36,36));

				mainmap.setPosition(j * 64, i * 64); //выводим на экран карту

				window.draw(mainmap);
			}

		if (!showLeveltext)
		{
			textmission.setPosition(Dino.getplayerx() + 70, Dino.getplayery() -90);
			speechBubbleSprite.setPosition(Dino.getplayerx() + 60,Dino.getplayery() - 100);
			window.draw(speechBubbleSprite);
			window.draw(textmission);
		}
		ostringstream scorestring;
		scorestring << Dino.score;
		textscore.setString("Dinocoins collected:"+ scorestring.str());
		textscore.setPosition(camera.getCenter().x+150, camera.getCenter().y-450);
		ostringstream Dinohealth;
		Dinohealth << Dino.health;
		texthealth.setString("Health:" + Dinohealth.str());
		texthealth.setPosition(camera.getCenter().x + 150, camera.getCenter().y - 420);
		window.draw(textscore);
			window.draw(texthealth);
		window.draw(Dino.sprite); //отрисовка персонажа
		window.display();
	}

	return 0;
}