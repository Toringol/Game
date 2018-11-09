#include <iostream>

#include "tinyxml/tinyxml.h"
#include "Level.h"

using namespace std;
using namespace sf;

int Object::GetPropertyInt(string name)
{
	return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(string name)
{
	return (float)strtod(properties[name].c_str(), NULL);
}

string Object::GetPropertyString(string name)
{
	return properties[name];
}

bool Level::LoadFromFile(string filename)
{
	TiXmlDocument levelFile(filename.c_str());

	if (!levelFile.LoadFile())
	{
		cout << "Loading level \"" << filename << "\" failed." << endl;
		return false;
	}

	TiXmlElement *map;
	map = levelFile.FirstChildElement("map");

	width = atoi(map->Attribute("width"));
	height = atoi(map->Attribute("height"));
	tileWidth = atoi(map->Attribute("tilewidth"));
	tileHeight = atoi(map->Attribute("tileheight"));

	TiXmlElement *tilesetElement;
	tilesetElement = map->FirstChildElement("tileset");
	firstTileID = atoi(tilesetElement->Attribute("firstgid"));

	TiXmlElement *image;
	image = tilesetElement->FirstChildElement("image");
	string imagepath = "tileset.png";

	Image img;

	if (!img.loadFromFile(imagepath))
	{
		cout << "Failed to load tile sheet." << endl;
		return false;
	}

	img.createMaskFromColor(Color(109, 159, 185));
	tilesetImage.loadFromImage(img);
	tilesetImage.setSmooth(false);

	int columns = tilesetImage.getSize().x / tileWidth;
	int rows = tilesetImage.getSize().y / tileHeight;

	vector<Rect<int>> subRects;

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
		{
			Rect<int> rect;

			rect.top = y * tileHeight;
			rect.height = tileHeight;
			rect.left = x * tileWidth;
			rect.width = tileWidth;

			subRects.push_back(rect);
		}

	TiXmlElement *layerElement;
	layerElement = map->FirstChildElement("layer");
	while (layerElement)
	{
		Layer layer;

		if (layerElement->Attribute("opacity") != NULL)
		{
			float opacity = (float)strtod(layerElement->Attribute("opacity"), NULL);
			layer.opacity = 255 * (int)opacity;
		}
		else
		{
			layer.opacity = 255;
		}

		TiXmlElement *layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == NULL)
		{
			cout << "Bad map. No layer information found." << endl;
		}

		TiXmlElement *tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == NULL)
		{
			cout << "Bad map. No tile information found." << endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - firstTileID;

			if (subRectToUse >= 0)
			{
				Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
				sprite.setPosition(x * (float)tileWidth, y * (float)tileHeight);
				sprite.setColor(Color(255, 255, 255, layer.opacity));

				layer.tiles.push_back(sprite);
			}

			tileElement = tileElement->NextSiblingElement("tile");

			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}
		}

		layers.push_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}

	TiXmlElement *objectGroupElement;

	if (map->FirstChildElement("objectgroup") != NULL)
	{
		objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			TiXmlElement *objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				string objectType;
				if (objectElement->Attribute("type") != NULL)
				{
					objectType = objectElement->Attribute("type");
				}
				string objectName;
				if (objectElement->Attribute("name") != NULL)
				{
					objectName = objectElement->Attribute("name");
				}
				int x = atoi(objectElement->Attribute("x"));
				int y = atoi(objectElement->Attribute("y"));

				int width, height;

				Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(Rect<int>(0, 0, 0, 0));
				sprite.setPosition((float)x, (float)y);

				if (objectElement->Attribute("width") != NULL)
				{
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else
				{
					width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
				}

				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = sprite;

				Rect <int> objectRect;
				objectRect.top = y;
				objectRect.left = x;
				objectRect.height = height;
				objectRect.width = width;
				object.rect = objectRect;

				TiXmlElement *properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != NULL)
				{
					TiXmlElement *prop;
					prop = properties->FirstChildElement("property");
					if (prop != NULL)
					{
						while (prop)
						{
							string propertyName = prop->Attribute("name");
							string propertyValue = prop->Attribute("value");

							object.properties[propertyName] = propertyValue;

							prop = prop->NextSiblingElement("property");
						}
					}
				}

				objects.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		cout << "No object layers found..." << endl;
	}

	return true;
}

Object Level::GetObject(string name)
{
	for (unsigned int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			return objects[i];
}

vector<Object> Level::GetObjects(string name)
{
	vector<Object> vec;
	for (unsigned int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			vec.push_back(objects[i]);

	return vec;
}

Vector2i Level::GetTileSize()
{
	return Vector2i(tileWidth, tileHeight);
}

void Level::Draw(RenderWindow &window)
{
	for (unsigned int layer = 0; layer < layers.size(); layer++)
		for (unsigned int tile = 0; tile < layers[layer].tiles.size(); tile++)
			window.draw(layers[layer].tiles[tile]);
}