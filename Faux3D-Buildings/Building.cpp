#include "Building.h"
#include <iostream>
#include "Game.h"
#include "Utilities.h"

Building::Building(sf::Vector2f pos, int dimX, int dimY, float height)
: mPos(pos)
, dimentions(dimX, dimY)
, mHeight(height)
{
	roofTopLeft = pos;
	groundTopLeft = pos;
	groundTopRight = pos + sf::Vector2f(dimentions.x,0);
	groundBotLeft = pos + sf::Vector2f(0, dimentions.y);
	groundBotRight = pos + sf::Vector2f(dimentions.x, dimentions.y);
	groundCenter = groundTopLeft + sf::Vector2f(dimentions.x / 2.f, dimentions.y / 2.f);


	std::string fileToLoad = "BuildingTileSheet.png";
	//std::string fileToLoad = "bricktile.jpg";
	if (!texture.loadFromFile(fileToLoad))
	{
		std::cerr << "Can't load texture: " << fileToLoad << std::endl;
	}
	texture.setRepeated(true);

	mBuildingQuad = sf::VertexArray(sf::Quads, 12);

	//mBuildingQuad[8].color = sf::Color(121, 189, 154);
	//mBuildingQuad[9].color = sf::Color(121, 189, 154);
	//mBuildingQuad[10].color = sf::Color(121, 189, 154);
	//mBuildingQuad[11].color = sf::Color(121, 189, 154);
	
	mBuildingQuad[8].texCoords = sf::Vector2f(0, 0);
	mBuildingQuad[9].texCoords = sf::Vector2f(500, 0);
	mBuildingQuad[10].texCoords = sf::Vector2f(500, 500);
	mBuildingQuad[11].texCoords = sf::Vector2f(0, 500);
	

	//mBuildingQuad[0].color = sf::Color(168, 219, 168);
	//mBuildingQuad[1].color = sf::Color(168, 219, 168);
	//mBuildingQuad[2].color = sf::Color(168, 219, 168);
	//mBuildingQuad[3].color = sf::Color(168, 219, 168);

	
	mBuildingQuad[0].texCoords = sf::Vector2f(1000, 0);
	mBuildingQuad[1].texCoords = sf::Vector2f(1000, 500);
	mBuildingQuad[2].texCoords = sf::Vector2f(500, 500);
	mBuildingQuad[3].texCoords = sf::Vector2f(500, 0);

	/*mBuildingQuad[0].texCoords = sf::Vector2f(0, 425);
	mBuildingQuad[1].texCoords = sf::Vector2f(0, 0);
	mBuildingQuad[2].texCoords = sf::Vector2f(425, 0);
	mBuildingQuad[3].texCoords = sf::Vector2f(425, 425);*/


	//mBuildingQuad[4].color = sf::Color(168, 219, 168);
	//mBuildingQuad[5].color = sf::Color(168, 219, 168);
	//mBuildingQuad[6].color = sf::Color(168, 219, 168);
	//mBuildingQuad[7].color = sf::Color(168, 219, 168);
	
	
	mBuildingQuad[4].texCoords = sf::Vector2f(500, 0);
	mBuildingQuad[5].texCoords = sf::Vector2f(1000, 0);
	mBuildingQuad[6].texCoords = sf::Vector2f(1000, 500);
	mBuildingQuad[7].texCoords = sf::Vector2f(500, 500);

	//mBuildingQuad[4].texCoords = sf::Vector2f(0, 0);
	//mBuildingQuad[5].texCoords = sf::Vector2f(425, 0);
	//mBuildingQuad[6].texCoords = sf::Vector2f(425, 425);
	//mBuildingQuad[7].texCoords = sf::Vector2f(0, 425);
}

void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &texture;

	target.draw(mBuildingQuad, states);

	sf::Vector2f windowPos = TheGame::Instance()->getWorldView()->getCenter();
	sf::RenderWindow* pWin = TheGame::Instance()->getWindow();
	//drawDebugLine(windowPos, mBuildingQuad[0].position, *pWin);
	//drawDebugLine(windowPos, mBuildingQuad[1].position, *pWin);
	//drawDebugLine(windowPos, mBuildingQuad[2].position, *pWin);
	//drawDebugLine(windowPos, mBuildingQuad[3].position, *pWin);
	//drawDebugLine(windowPos, mBuildingQuad[4].position, *pWin);
	//drawDebugLine(windowPos, mBuildingQuad[5].position, *pWin);
	//drawDebugLine(windowPos, mBuildingQuad[6].position, *pWin);
	//drawDebugLine(windowPos, mBuildingQuad[7].position, *pWin);

	///< Roof
	//drawDebugLine(windowPos, mBuildingQuad[8].position, *pWin, sf::Color::Red);
	//drawDebugLine(windowPos, mBuildingQuad[9].position, *pWin, sf::Color::Red);
	//drawDebugLine(windowPos, mBuildingQuad[10].position, *pWin, sf::Color::Red);
	//drawDebugLine(windowPos, mBuildingQuad[11].position, *pWin, sf::Color::Red);
}

void Building::update(sf::Time dTime)
{
	sf::Vector2f windowPos = TheGame::Instance()->getWorldView()->getCenter();

	//Roof
	mBuildingQuad[8].position = groundTopLeft + (groundTopLeft - windowPos) * mHeight;

	mBuildingQuad[9].position = groundTopRight + (groundTopRight - windowPos) * mHeight;
	mBuildingQuad[10].position = groundBotRight + (groundBotRight - windowPos) * mHeight;
	mBuildingQuad[11].position = groundBotLeft + (groundBotLeft - windowPos) * mHeight;

	//East/West Wall
	if (windowPos.x > groundTopRight.x)
	{ //Display East Wall
		mBuildingQuad[0].position = mBuildingQuad[9].position;
		mBuildingQuad[1].position = groundTopRight;
		mBuildingQuad[2].position = groundBotRight;
		mBuildingQuad[3].position = mBuildingQuad[10].position;
	}
	else if (windowPos.x < groundTopLeft.x)
	{ //Display West Wall
		mBuildingQuad[0].position = groundTopLeft;
		mBuildingQuad[1].position = mBuildingQuad[8].position;
		mBuildingQuad[2].position = mBuildingQuad[11].position;
		mBuildingQuad[3].position = groundBotLeft;
	}
	else
	{ //No wall to display
		mBuildingQuad[0].position = sf::Vector2f(0,0);
		mBuildingQuad[1].position = sf::Vector2f(0, 0);
		mBuildingQuad[2].position = sf::Vector2f(0, 0);
		mBuildingQuad[3].position = sf::Vector2f(0, 0);
	}

	//North/South Wall
	if (windowPos.y > groundBotLeft.y)
	{ //Display South Wall
		mBuildingQuad[4].position = mBuildingQuad[11].position;
		mBuildingQuad[5].position = mBuildingQuad[10].position;
		mBuildingQuad[6].position = groundBotRight;
		mBuildingQuad[7].position = groundBotLeft;
	}
	else if (windowPos.y < groundTopLeft.y)
	{ //Display North Wall
		mBuildingQuad[4].position = groundTopLeft;
		mBuildingQuad[5].position = groundTopRight;
		mBuildingQuad[6].position = mBuildingQuad[9].position;
		mBuildingQuad[7].position = mBuildingQuad[8].position;
	}
	else
	{ //No wall to display
		mBuildingQuad[4].position = sf::Vector2f(0, 0);
		mBuildingQuad[5].position = sf::Vector2f(0, 0);
		mBuildingQuad[6].position = sf::Vector2f(0, 0);
		mBuildingQuad[7].position = sf::Vector2f(0, 0);
	}
}