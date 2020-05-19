#include <iostream>
#include <sstream> // для строк в потоке
#include <SFML/Graphics.hpp>
#include "camera.h"
#include "speech.h"
#include "level.h"
#include <vector>
#include "tinyxml/tinyxml.h"
// осталось реализовать меню стартовое,меню после смерти,коллизию врага и музыку,и добавить диалоги
using namespace sf;
using namespace std;

class Enemy { // класс игрока
private:

public:
	vector<Object> obj;
	float x, y; // координаты игрока,используются только через сеттеры
	float dx, dy, w, h; // перемещение по осям , высота и ширина спрайта игрока
	float speed = 0; // изначальная скорость игрока
	int score; // счет
	bool life;
	bool playerOnGround;
	int health;
	enum  statePlayer
	{
		left, right, up, down, jump, stay
	};
	statePlayer state;
	String File; // здесь хранится имя файла
	Sprite sprite; // спрайт игрока
	Image image; // картинка для спрайта
	Texture texture; // текстура для спрайта
	Enemy(String F, Level& lev, float X, float Y, int W, int H) { //класс конкструктор
		obj = lev.GetAllObjects();
		File = F;
		w = W;
		h = H;
		score = 0;
		health = 100;
		dx = 0.1;
		dy = 0;
		life = true;
		playerOnGround = false;
		state = stay;
		image.loadFromFile("../../Sprites/PNG/" + File);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X;
		y = Y;
		sprite.setTextureRect(IntRect(39, 44, w, h)); // создаем персонажа
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}
	void checkCollision(float Dx, float Dy) {
		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (Dy > 0)
				{
					y = obj[i].rect.top - h; // не даем войти в текстуру,когда перемещаемся вниз, просто отбрасывая персонажа на его высоту
					dy = 0;
					playerOnGround = true;
				}

				if (Dy < 0)
				{
					y = obj[i].rect.top + obj[i].rect.height; //тоже самое дальше для всех направлений
					dy = 0;
				}
				if (Dx > 0)
				{
					x = obj[i].rect.left - w;
					dx = -0.1;
					sprite.scale(-1, 1);
				}
				if (Dx < 0)
				{
					x = obj[i].rect.left + obj[i].rect.width;
					dx = 0.1;
					sprite.scale(-1, 1);
				}
			}
	}
	void update(float time) {
		x += dx * time; // перемещение координаты х за время,когда задаем расстояние перемещения за время, получаем новую координату
		checkCollision(dx, 0);
		y += dy * time; // то же, что и с иксом
		checkCollision(0, dy);
		sprite.setPosition(x + w / 2, y + h / 2); // запоминаем координаты
		if (health <= 0)
		{
			life = false;
		}
		dy = dy + 0.0015 * time; // притяжение к земле
	}
};

class Player { // класс игрока
public:
	vector<Object> obj;
	float x, y; // координаты игрока,используются только через сеттеры
	float dx, dy, w, h; // перемещение по осям , высота и ширина спрайта игрока
	float speed = 0; // изначальная скорость игрока
	int score; // счет
	bool life;
	bool playerOnGround;
	bool move;
	int health;
	enum  statePlayer
	{
		left, right, up, down, jump, stay, animstay
	};
	statePlayer state;
	String File; // здесь хранится имя файла
	Sprite sprite; // спрайт игрока
	Image image; // картинка для спрайта
	Texture texture; // текстура для спрайта
	Player(String F, Level& lev, float X, float Y, int W, int H) { //класс конкструктор
		obj = lev.GetAllObjects();
		File = F;
		w = W;
		h = H;
		score = 0;
		health = 100;
		dx = 0;
		dy = 0;
		life = true;
		playerOnGround = false;
		move = false;
		state = stay;
		image.loadFromFile("../../Sprites/PNG/" + File);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X;
		y = Y;
		sprite.setTextureRect(IntRect(0, 8, w, h)); // создаем персонажа
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect() {
		return FloatRect(x, y, w, h);
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
		speed = 0;
		dy = dy + 0.0015 * time; // притяжение к земле
		
	}
	void playercontrol() {
		if (life == true) {
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				state = left;
				speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				state = right;
				speed = 0.1;
			}
			if ((Keyboard::isKeyPressed(Keyboard::W)) && (playerOnGround))
			{
				state = jump;
				dy = -0.7;
				playerOnGround = false;
			}
		}
	}
	float getplayerx() { // геттер на координату х для камеры и игрока
		return x;
	}
	float getplayery() { //геттер на координату у для камеры и игрока
		return y;
	}
	float getplayerdx() { //геттер на перемещение
		return dx;
	}
	float getplayerdy() { // геттер на перемещение
		return dy;
	}
	void checkCollision(float Dx, float Dy) {
		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (Dy > 0)
					{
						y = obj[i].rect.top - h; // не даем войти в текстуру,когда перемещаемся вниз, просто отбрасывая персонажа на его высоту
						dy = 0;
						playerOnGround = true;
					}

					if (Dy < 0)
					{
						y = obj[i].rect.top + obj[i].rect.height; //тоже самое дальше для всех направлений
						dy = 0;
					}
					if (Dx > 0)
					{
						x = obj[i].rect.left - w;
					}
					if (Dx < 0)
					{
						x = obj[i].rect.left + obj[i].rect.width;
					}
				}
				if (obj[i].name == "water")
				{
					life = false;

					if (Dy > 0)
					{
						y = obj[i].rect.top - h; // не даем войти в текстуру,когда перемещаемся вниз, просто отбрасывая персонажа на его высоту
						dy = 0;
						playerOnGround = true;
					}

					if (Dy < 0)
					{
						y = obj[i].rect.top + obj[i].rect.height; //тоже самое дальше для всех направлений
						dy = 0;
					}
					if (Dx > 0)
					{
						x = obj[i].rect.left - w;
					}
					if (Dx < 0)
					{
						x = obj[i].rect.left + obj[i].rect.width;
					}
				}
				if (obj[i].name == "coin")
				{
					if (getRect().intersects(obj[i].rect))
					{
						score++;
						obj[i].name = "null";
					}
					
				}
				if (obj[i].name == "health")
				{
					health = health + 20;
					obj[i].name = "null";
				}
				if (obj[i].name == "potion")
				{
					health = health - 40;
					obj[i].name = "null";
				}
			}
	}
};

int main()
{
	RenderWindow window(VideoMode(1280, 960), "Dinotaur");
	camera.reset(FloatRect(0, 0, 1280, 960));
	Level lvl;
	lvl.LoadFromFile("test.tmx");
	Font pixelfont;
	pixelfont.loadFromFile("slkscreb.ttf");
	Text textscore("", pixelfont, 25);
	Text texthealth("", pixelfont, 25);
	Text textmission("", pixelfont, 15);
	textscore.setFillColor(Color::Black);
	textmission.setFillColor(Color::Black);

	Image speechBubble;
	speechBubble.loadFromFile("../../Sprites/PNG/speech.png");
	Texture speechBubbleTexture;
	speechBubbleTexture.loadFromImage(speechBubble);
	Sprite speechBubbleSprite;
	speechBubbleSprite.setTexture(speechBubbleTexture);
	speechBubbleSprite.setTextureRect(IntRect(139, 74, 128, 122));
	Object player = lvl.GetObject("player");
	Object enemy = lvl.GetObject("enemy");

	Player Dino("DinoSpriteDoux.png", lvl, player.rect.left, player.rect.top, 64.0, 90.0);
	Enemy Rex("Dinoenemy.png", lvl, enemy.rect.left, enemy.rect.top, 64, 90);
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
			if (event.type == Event::KeyReleased)
			{
				if (event.key.code == Keyboard::D)
				{
					Dino.sprite.setTextureRect(IntRect(1, 6, 64, 90));
				}
				if (event.key.code == Keyboard::A)
				{
					Dino.sprite.setTextureRect(IntRect(90, 6, 64, 96));
				}
			}
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
				Dino.sprite.setTextureRect(IntRect(74 * int(currentframe) + 74, 222, -74, 96));
			}
		}
		if (Dino.life == false)
		{
			currentframe += 0.002 * time;
			if (currentframe > 3) currentframe = 2;
			{
				Dino.sprite.setTextureRect(IntRect(77 * int(currentframe), 337, 64, 90));
			}
		}

		Dino.update(time);
		Rex.update(time);
		window.setView(camera);

		window.clear();
		lvl.Draw(window);

		if (!showLeveltext)
		{
			textmission.setPosition(Dino.getplayerx() + 70, Dino.getplayery() - 90);
			speechBubbleSprite.setPosition(Dino.getplayerx() + 60, Dino.getplayery() - 100);
			window.draw(speechBubbleSprite);
			window.draw(textmission);
		}
		ostringstream scorestring;
		scorestring << Dino.score;
		textscore.setString("Dinocoins collected:" + scorestring.str());
		textscore.setPosition(camera.getCenter().x + 100, camera.getCenter().y - 430);
		ostringstream Dinohealth;
		Dinohealth << Dino.health;
		texthealth.setString("Health:" + Dinohealth.str());
		texthealth.setPosition(camera.getCenter().x + 100, camera.getCenter().y - 400);
		window.draw(textscore);
		window.draw(texthealth);
		window.draw(Dino.sprite); //отрисовка персонажа
		window.draw(Rex.sprite);
		window.display();
	}

	return 0;
}