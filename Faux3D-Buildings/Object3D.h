#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

class Object3D : public sf::Drawable, public sf::Transformable
{
public:

	enum Object3DType
	{
		Building,
		LampPost
	};

	Object3D(const TextureHolder& textures, sf::Vector2f pos, sf::Vector2i dim, Object3DType type, float height);

	void update(sf::Time dTime);

	const sf::Vector2f& getCenter() { return groundCenter; }

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::VertexArray mObject3DQuad;

	Object3DType	mType;

	sf::Texture		objectTexture;

	sf::Vector2f	roofTopLeft;

	sf::Vector2f	groundTopLeft;
	sf::Vector2f	groundTopRight;
	sf::Vector2f	groundBotLeft;
	sf::Vector2f	groundBotRight;

	sf::Vector2f	groundCenter;

	sf::Vector2i	dimentions;

	float			mHeight;

	sf::Vector2f	mPos;

	const TextureHolder&	texturesConstRef;
};