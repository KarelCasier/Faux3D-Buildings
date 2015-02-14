#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Building : public sf::Drawable, public sf::Transformable
{
public:
	Building(sf::Vector2f pos, int dimX, int dimY, float height);

	void update(sf::Time dTime);

	const sf::Vector2f& getCenter() { return groundCenter; }

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::VertexArray mBuildingQuad;

	sf::Texture		texture;

	sf::Vector2f	roofTopLeft;

	sf::Vector2f	groundTopLeft;
	sf::Vector2f	groundTopRight;
	sf::Vector2f	groundBotLeft;
	sf::Vector2f	groundBotRight;

	sf::Vector2f	groundCenter;

	sf::Vector2i	dimentions;

	float			mHeight;

	sf::Vector2f	mPos;
};