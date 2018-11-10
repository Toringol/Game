#include "menu.h"

int main(int argc, char** argv) {

	sf::RenderWindow mainWindow(sf::VideoMode(1280, 800, 32), "Simple game");

	menu(mainWindow);

	return 0;
}