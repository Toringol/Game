#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-system.lib")

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Object
{
	int GetPropertyInt(string name);
	float GetPropertyFloat(string name);
	string GetPropertyString(string name);
	string name;
	string type;
	Rect<float> rect;
	map<string, string> properties;
	Sprite sprite;
};

struct Layer
{
	int opacity;
	vector<Sprite> tiles;
};

class Level
{
public:
	bool LoadFromFile(string filename);
	Object GetObject(string name);
	vector<Object> GetObjects(string name);
	std::vector<Object> GetAllObjects();
	void Draw(RenderWindow &window);
	Vector2i GetTileSize();

private:
	int width, height, tileWidth, tileHeight;
	int firstTileID;
	Rect<float> drawingBounds;
	Texture tilesetImage;
	vector<Object> objects;
	vector<Layer> layers;
};

#endif //GAME_LEVEL_H
