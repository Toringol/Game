#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>

#include "ResourceHolder.h"
#include "Resourceidentifiers.h"
#include "Level.h"
#include "view.h"

class Entity {
public:
	std::vector<Object> obj;
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}
};

class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;

	Player(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		playerScore = 0; state = stay; obj = lvl.GetAllObjects();
		if (name == "player") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
	}

	void control() {
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				state = left; speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; speed = 0.1;
			}

			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				state = jump; dy = -0.1;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = jump; dy = 0.1;
			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time)
	{
		control();
		switch (state)
		{
		case right: dx = speed; break;
		case left: dx = -speed; break;
		case up: dy = speed; break;
		case down: dy = speed; break;
		case stay: break;
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy);
		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		setPlayerCoordinateForView(x, y);
		if (life) { setPlayerCoordinateForView(x, y); }
		//dy = dy + 0.0015 * time;
	}
};



class Enemy :public Entity {
public:
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid") {
				if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; }
				if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
				if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {
			checkCollisionWithMap(dx, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};


void menu(sf::RenderWindow & window) {

	TextureHolder textures;
	textures.load(Textures::Image_01, "BackGround.jpg");
	textures.load(Textures::Image_02, "NewGame.png");
	textures.load(Textures::Image_03, "Options.png");
	textures.load(Textures::Image_04, "Exit.png");
	textures.load(Textures::Image_05, "CheckPicture.jpg");

	sf::Sprite spriteBackGround, spriteNewGame, spriteOptions, spriteExit, spriteCheck;
	spriteBackGround.setTexture(textures.get(Textures::Image_01));
	spriteNewGame.setTexture(textures.get(Textures::Image_02));
	spriteOptions.setTexture(textures.get(Textures::Image_03));
	spriteExit.setTexture(textures.get(Textures::Image_04));
	spriteCheck.setTexture(textures.get(Textures::Image_05));

	bool isMenu = 1;
	int menuNum = 0;

	spriteNewGame.setPosition(10, 530);
	spriteOptions.setPosition(10, 570);
	spriteExit.setPosition(10, 610);

	while (isMenu)
	{
		spriteNewGame.setColor(sf::Color(135, 135, 135));
		spriteOptions.setColor(sf::Color(135, 135, 135));
		spriteExit.setColor(sf::Color(135, 135, 135));

		menuNum = 0;
		window.clear(sf::Color::White);

		if (sf::IntRect(30, 550, 160, 40).contains(sf::Mouse::getPosition(window))) {
			spriteNewGame.setColor(sf::Color(135, 34, 34));
			menuNum = 1;
		}

		if (sf::IntRect(30, 590, 115, 40).contains(sf::Mouse::getPosition(window))) {
			spriteOptions.setColor(sf::Color(135, 34, 34));
			menuNum = 2;
		}

		if (sf::IntRect(30, 630, 65, 40).contains(sf::Mouse::getPosition(window))) {
			spriteExit.setColor(sf::Color(135, 34, 34));
			menuNum = 3;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (menuNum == 1) {
				Level level;
				level.LoadFromFile("map.tmx");
				
				Image heroImage;
				heroImage.loadFromFile("player.png");

				Object player = level.GetObject("player");

				Player p(heroImage, "player", level, player.rect.left, player.rect.top, 32, 32);

				Clock clock;

				window.create(sf::VideoMode(1280, 800), "Level - 1");

				while (window.isOpen()) {

					float time = clock.getElapsedTime().asMicroseconds();

					clock.restart();
					time = time / 800;

					sf::Event event;

					while (window.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
							window.close();
					}
					p.update(time);
					window.clear();
					level.Draw(window);
					window.draw(p.sprite);
					window.display();
				}

				isMenu = false;

			}
			if (menuNum == 2) { window.draw(spriteCheck); window.display(); while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }

		}

		window.draw(spriteBackGround);
		window.draw(spriteNewGame);
		window.draw(spriteOptions);
		window.draw(spriteExit);

		window.display();
	}
}

#endif //GAME_MENU_H

