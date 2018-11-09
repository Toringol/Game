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

		if (sf::IntRect(30, 550, 160, 45).contains(sf::Mouse::getPosition(window))) {
			spriteNewGame.setColor(sf::Color(135, 34, 34));
			menuNum = 1;
		}

		if (sf::IntRect(30, 590, 125, 45).contains(sf::Mouse::getPosition(window))) {
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

				window.create(sf::VideoMode(1280, 800), "Level - 1");

				while (window.isOpen()) {
					sf::Event event;

					while (window.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
							window.close();
					}

					window.clear();
					level.Draw(window);
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

