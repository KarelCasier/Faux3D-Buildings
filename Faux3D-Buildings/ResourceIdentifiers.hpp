#pragma once

// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
}

enum class TextureID
{
	BuildingTileSet,
	LampPostTileSet,
	Road2Way,
	Road3Way,
	Road4Way
};

enum class FontID
{
	Default,
};

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, TextureID>	TextureHolder;
typedef ResourceHolder<sf::Font, FontID>		FontHolder;