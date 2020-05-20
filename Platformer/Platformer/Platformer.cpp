#include <iostream>
#include <sstream> // для строк в потоке
#include "camera.h"
#include "speech.h"
#include "level.h"
#include <vector>
#include <list>
#include "tinyxml/tinyxml.h"
// осталось реализовать музыку,пропадание элементов.
using namespace sf;
using namespace std;

class Enemy { // класс врага
private:

public:
	vector<Object> obj;
	float x, y; // координаты игрока,используются как через класс,так и через сеттеры
	float dx, dy, w, h; // перемещение по осям , высота и ширина спрайта игрока
	float speed = 0; // изначальная скорость игрока
	int score; // счет
	bool life; // логическая переменная, жив ли враг
	bool playerOnGround; // на земле ли враг
	int health; // количесвто жизней
	enum  statePlayer
	{
		left, right, up, down, jump, stay
	}; // перечисляемая переменная,сделана для показа анимаций и передвижения по карте
	statePlayer state;
	String File; // здесь хранится имя файла
	Sprite sprite; // спрайт игрока
	Image image; // картинка для спрайта
	Texture texture; // текстура для спрайта
	Enemy(String F, Level& lev, float X, float Y, int W, int H) { //класс конкструктор
		obj = lev.GetAllObjects(); //из карты вытягиваем все доступные обьекты для нашего класса.
		File = F;
		w = W;
		h = H;
		score = 0; 
		health = 100;
		dx = 0.01;
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
	FloatRect getRect() { // функция,которая определяет прямоугольник нашего спрайта
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
					}
				}
				if (obj[i].name == "health")
				{
					health = health + 20;
				}
				if (obj[i].name == "potion")
				{
					health = health - 40;
				}
			}
	}
};


void startmenu(RenderWindow & window) {

	Texture menuTexture, menuBackground;
	menuTexture.loadFromFile("../../PNG/play.png");
	menuBackground.loadFromFile("../../PNG/menu.png");
	Sprite menu(menuBackground), play(menuTexture);
	bool menue = true;
	int menuNumber = 0;
	play.setPosition(540, 480);
	menu.setPosition(0, 0);
	while (menue)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		
		menuNumber = 0;
		window.clear(Color(0,0,0));

		if (IntRect(540, 480, 240, 120).contains(Mouse::getPosition(window))) { play.setColor(Color::Magenta); menuNumber = 1; }
	

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNumber == 1) menue = false;//если нажали первую кнопку, то выходим из меню 
			

		}

		window.draw(menu);
		window.draw(play);
		window.display();
	}
	
}


int main()
{
	RenderWindow window(VideoMode(1280, 960), "Dinotaur");

	startmenu(window);
	camera.reset(FloatRect(0, 0, 1280, 960));
	Level lvl;
	lvl.LoadFromFile("test.tmx");

	Font pixelfont;
	pixelfont.loadFromFile("slkscreb.ttf");
	Text textscore("", pixelfont, 25);
	Text texthealth("", pixelfont, 25);
	Text textmission("", pixelfont, 15);
	Text textwin("", pixelfont, 80);

	textscore.setFillColor(Color::Black);
	textmission.setFillColor(Color::Black);
	texthealth.setFillColor(Color::Black);
	textwin.setFillColor(Color::Black);

	list<Enemy*> enemies;
	list<Enemy*>::iterator it;
	vector<Object> e = lvl.GetObjects("enemy");
	for (int i = 0; i < e.size(); i++)
	{
		enemies.push_back(new Enemy("Dinoenemy.png", lvl, e[i].rect.left, e[i].rect.top, 64, 90));
	}
	

	Image speechBubble;
	speechBubble.loadFromFile("../../Sprites/PNG/speech.png");
	Texture speechBubbleTexture;
	speechBubbleTexture.loadFromImage(speechBubble);
	Sprite speechBubbleSprite;
	speechBubbleSprite.setTexture(speechBubbleTexture);
	speechBubbleSprite.setTextureRect(IntRect(139, 74, 128, 122));
	Object player = lvl.GetObject("player");
	Object coins = lvl.GetObject("coins");
	Player Dino("DinoSpriteDoux.png", lvl, player.rect.left, player.rect.top, 64.0, 90.0);

	bool win = false;
	float currentframe = 0;
	bool showLeveltext = true;
	Clock gametime; // привязка ко времени сфмл, а не к процессору

	while (window.isOpen()) // основной цикл программы
	{
		float time = gametime.getElapsedTime().asMicroseconds();
		gametime.restart();
		time = time / 550;
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
		for (it = enemies.begin(); it != enemies.end(); it++)
		{
			(*it)->update(time);
		}

		Dino.update(time);

		for (it = enemies.begin(); it != enemies.end();)//говорим что проходимся от начала до конца
		{
			Enemy* b = *it;//для удобства, чтобы не писать (*it)->
			b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false) { it = enemies.erase(it); delete b; }// если этот объект мертв, то удаляем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}
		for (it = enemies.begin(); it != enemies.end(); it++)//проходимся по эл-там списка
		{
			if ((*it)->getRect().intersects(Dino.getRect()))//если прямоугольник спрайта объекта пересекается с игроком
			{
				if ((Dino.dy > 0) && (Dino.playerOnGround == false)) { (*it)->dx = 0; Dino.dy = -0.2; (*it)->health = 0; }//если прыгнули на врага,то даем врагу скорость 0,отпрыгиваем от него чуть вверх,даем ему здоровье 0
				else {
					Dino.health -= 1;	//иначе враг подошел к нам сбоку и нанес урон
				}
			}
		}
		//cout << Dino.x <<" "<< Dino.y << endl;
		if (Dino.x > 6600)
		{
			win = true;
		}
		
		window.setView(camera);

		window.clear();
		lvl.Draw(window);

		if (win)
		{
			textwin.setPosition(camera.getCenter().x-600, camera.getCenter().y);
			window.draw(textwin);
		}
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
		textwin.setString("CONGRATULATIONS!\nYOU WON!");
		
		ostringstream Dinohealth;
		Dinohealth << Dino.health;
		texthealth.setString("Health:" + Dinohealth.str());
		texthealth.setPosition(camera.getCenter().x + 100, camera.getCenter().y - 400);
		window.draw(textscore);
		window.draw(texthealth);
		window.draw(Dino.sprite); //отрисовка персонажа
		for (it = enemies.begin(); it != enemies.end(); it++) {
			window.draw((*it)->sprite);
		}

		window.display();
	}

	return 0;
}