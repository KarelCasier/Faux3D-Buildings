#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Building : public sf::Drawable, public sf::Transformable
{
public:
	Building(sf::Vector2f pos, int width, int height);

	void update(sf::Time dTime);

	const sf::Vector2f& getCenter() { return groundCenter; }

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::VertexArray mBuildingQuad;

	sf::Texture		texture;

	sf::Vector2f	roofTopLeft;

	sf::Vector2f	groundTopLeft;

	sf::Vector2f	groundCenter;

	int				mWidth, mHeight;

	sf::Vector2f	mPos;

	//sf::Vertex line[2];

};