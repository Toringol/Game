#ifndef GAME_RESOUCEIDENTIFIERS_H
#define GAME_RESOUCEIDENTIFIERS_H

namespace sf {
	class Texture;
	class Font;
}

namespace Textures {
	enum ID {
		Image_01 = 0,
		Image_02,
		Image_03,
		Image_04,
		Image_05
	};
}

namespace Fonts {
	enum ID {
		Main = 0
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>   TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>         FontHolder;

#endif //GAME_RESOUCEIDENTIFIERS_H
