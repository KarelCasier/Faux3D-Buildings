#include "Building.h"
#include <iostream>
#include "Game.h"
#include "HelperFunctions.hpp"

Building::Building(sf::Vector2f pos, int width, int height)
: mPos(pos)
, mWidth(width)
, mHeight(height)
{
	roofTopLeft = pos;
	groundTopLeft = pos;

	groundCenter = groundTopLeft + sf::Vector2f(mWidth / 2.f, mHeight / 2.f);


	std::string fileToLoad = "BuildingTileSheetFlat.png";
	if (!texture.loadFromFile(fileToLoad))
	{
		std::cerr << "Can't load texture: " << fileToLoad << std::endl;
	}

	mBuildingQuad = sf::VertexArray(sf::Quads, 12);

	//Top
	mBuildingQuad[8].position = sf::Vector2f(roofTopLeft.x,				roofTopLeft.y);
	mBuildingQuad[9].position = sf::Vector2f(roofTopLeft.x + mWidth,	roofTopLeft.y);
	mBuildingQuad[10].position = sf::Vector2f(roofTopLeft.x + mWidth,	roofTopLeft.y + mHeight);
	mBuildingQuad[11].position = sf::Vector2f(roofTopLeft.x,				roofTopLeft.y + mHeight);
	mBuildingQuad[8].texCoords = sf::Vector2f(0, 0);
	mBuildingQuad[9].texCoords = sf::Vector2f(500, 0);
	mBuildingQuad[10].texCoords = sf::Vector2f(500, 500);
	mBuildingQuad[11].texCoords = sf::Vector2f(0, 500);

	//Right
	mBuildingQuad[0].position = sf::Vector2f(roofTopLeft.x + mWidth,	roofTopLeft.y);
	mBuildingQuad[1].position = sf::Vector2f(groundTopLeft.x + mWidth,	groundTopLeft.y);
	mBuildingQuad[2].position = sf::Vector2f(groundTopLeft.x + mWidth,	groundTopLeft.y + mHeight);
	mBuildingQuad[3].position = sf::Vector2f(roofTopLeft.x + mWidth,	roofTopLeft.y + mHeight);
	mBuildingQuad[0].texCoords = sf::Vector2f(1000, 0);
	mBuildingQuad[1].texCoords = sf::Vector2f(1000, 500);
	mBuildingQuad[2].texCoords = sf::Vector2f(500, 500);
	mBuildingQuad[3].texCoords = sf::Vector2f(500, 0);

	//Bot
	mBuildingQuad[4].position = sf::Vector2f(roofTopLeft.x,				roofTopLeft.y + mHeight);
	mBuildingQuad[5].position = sf::Vector2f(roofTopLeft.x + mWidth,	roofTopLeft.y + mHeight);
	mBuildingQuad[6].position = sf::Vector2f(groundTopLeft.x + mWidth, groundTopLeft.y + mHeight);
	mBuildingQuad[7].position = sf::Vector2f(groundTopLeft.x,			groundTopLeft.y + mHeight);
	mBuildingQuad[4].texCoords = sf::Vector2f(500, 0);
	mBuildingQuad[5].texCoords = sf::Vector2f(1000, 0);
	mBuildingQuad[6].texCoords = sf::Vector2f(1000, 500);
	mBuildingQuad[7].texCoords = sf::Vector2f(500, 500);
}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &texture;

	target.draw(mBuildingQuad, states);
	//target.draw(line, 2, sf::Lines);
}

void Building::update(sf::Time dTime)
{
	sf::Vector2f windowPos = TheGame::Instance()->getWorldView()->getCenter();

	//line[0] = sf::Vertex(windowPos);
	//line[1] = sf::Vertex(groundCenter);

	float rotation = (float)(std::atan2f(groundCenter.y - windowPos.y, groundCenter.x - windowPos.x));

	float height = getLengthOfVector2f(sf::Vector2f(groundCenter.x - windowPos.x, groundCenter.y - windowPos.y)) / 2.5;



	roofTopLeft.x = (groundCenter.x + height * std::cosf(rotation)) - mWidth / 2.f;
	roofTopLeft.y = (groundCenter.y + height * std::sinf(rotation)) - mHeight / 2.f;

	float roofPerspectiveZoom = ((mWidth + mHeight) / 2) / 8; //Roof is bigger then the base

	//Roof
	mBuildingQuad[8].position = sf::Vector2f(roofTopLeft.x - roofPerspectiveZoom,				roofTopLeft.y - roofPerspectiveZoom);
	mBuildingQuad[9].position = sf::Vector2f(roofTopLeft.x + mWidth + roofPerspectiveZoom,		roofTopLeft.y - roofPerspectiveZoom);
	mBuildingQuad[10].position = sf::Vector2f(roofTopLeft.x + mWidth + roofPerspectiveZoom,		roofTopLeft.y + mHeight + roofPerspectiveZoom);
	mBuildingQuad[11].position = sf::Vector2f(roofTopLeft.x - roofPerspectiveZoom,				roofTopLeft.y + mHeight + roofPerspectiveZoom);

	//East/West Wall
	if (roofTopLeft.x - roofPerspectiveZoom < groundTopLeft.x)
	{ //Display East Wall
		mBuildingQuad[0].position = sf::Vector2f(roofTopLeft.x + mWidth + roofPerspectiveZoom,	roofTopLeft.y - roofPerspectiveZoom);
		mBuildingQuad[1].position = sf::Vector2f(groundTopLeft.x + mWidth,						groundTopLeft.y);
		mBuildingQuad[2].position = sf::Vector2f(groundTopLeft.x + mWidth,						groundTopLeft.y + mHeight);
		mBuildingQuad[3].position = sf::Vector2f(roofTopLeft.x + mWidth + roofPerspectiveZoom,	roofTopLeft.y + mHeight + roofPerspectiveZoom);
	}
	else
	{ //Display West Wall
		mBuildingQuad[0].position = sf::Vector2f(groundTopLeft.x,								groundTopLeft.y);
		mBuildingQuad[1].position = sf::Vector2f(roofTopLeft.x - roofPerspectiveZoom,			roofTopLeft.y - roofPerspectiveZoom);
		mBuildingQuad[2].position = sf::Vector2f(roofTopLeft.x - roofPerspectiveZoom,			roofTopLeft.y + mHeight + roofPerspectiveZoom);
		mBuildingQuad[3].position = sf::Vector2f(groundTopLeft.x,								groundTopLeft.y + mHeight);
	}

	//North/South Wall
	if (roofTopLeft.y - roofPerspectiveZoom < groundTopLeft.y)
	{ //Display South Wall
		mBuildingQuad[4].position = sf::Vector2f(roofTopLeft.x - roofPerspectiveZoom,			roofTopLeft.y + mHeight + roofPerspectiveZoom);
		mBuildingQuad[5].position = sf::Vector2f(roofTopLeft.x + mWidth + roofPerspectiveZoom,	roofTopLeft.y + mHeight + roofPerspectiveZoom);
		mBuildingQuad[6].position = sf::Vector2f(groundTopLeft.x + mWidth,						groundTopLeft.y + mHeight);
		mBuildingQuad[7].position = sf::Vector2f(groundTopLeft.x,								groundTopLeft.y + mHeight);
	}
	else
	{ //Display North Wall
		mBuildingQuad[4].position = sf::Vector2f(groundTopLeft.x,								groundTopLeft.y);
		mBuildingQuad[5].position = sf::Vector2f(groundTopLeft.x + mWidth,						groundTopLeft.y);
		mBuildingQuad[6].position = sf::Vector2f(roofTopLeft.x + mWidth + roofPerspectiveZoom, roofTopLeft.y - roofPerspectiveZoom);
		mBuildingQuad[7].position = sf::Vector2f(roofTopLeft.x - roofPerspectiveZoom,			roofTopLeft.y - roofPerspectiveZoom);
	}
}