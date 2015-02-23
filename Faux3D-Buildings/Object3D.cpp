#include "Object3D.h"
#include <iostream>
#include "Game.h"
#include "Utilities.h"

Object3D::Object3D(const TextureHolder& textures, sf::Vector2f pos, sf::Vector2i dim, Object3DType type, float height)
: mPos(pos)
, dimentions(dim)
, mHeight(height)
, texturesConstRef(textures)
, mType(type)
{
	roofTopLeft = pos;
	groundTopLeft = pos;
	groundTopRight = pos + sf::Vector2f(dimentions.x, 0);
	groundBotLeft = pos + sf::Vector2f(0, dimentions.y);
	groundBotRight = pos + sf::Vector2f(dimentions.x, dimentions.y);
	groundCenter = groundTopLeft + sf::Vector2f(dimentions.x / 2.f, dimentions.y / 2.f);

	mObject3DQuad = sf::VertexArray(sf::Quads, 12);

	switch (mType)
	{
	case Object3D::Building:
		objectTexture = texturesConstRef.get(TextureID::BuildingTileSet);

		mObject3DQuad[8].texCoords = sf::Vector2f(0, 0);
		mObject3DQuad[9].texCoords = sf::Vector2f(500, 0);
		mObject3DQuad[10].texCoords = sf::Vector2f(500, 500);
		mObject3DQuad[11].texCoords = sf::Vector2f(0, 500);

		mObject3DQuad[0].texCoords = sf::Vector2f(1000, 0);
		mObject3DQuad[1].texCoords = sf::Vector2f(1000, 500);
		mObject3DQuad[2].texCoords = sf::Vector2f(500, 500);
		mObject3DQuad[3].texCoords = sf::Vector2f(500, 0);

		mObject3DQuad[4].texCoords = sf::Vector2f(500, 0);
		mObject3DQuad[5].texCoords = sf::Vector2f(1000, 0);
		mObject3DQuad[6].texCoords = sf::Vector2f(1000, 500);
		mObject3DQuad[7].texCoords = sf::Vector2f(500, 500);

		zPos = 0;

		break;
	case Object3D::LampPost:
		objectTexture = texturesConstRef.get(TextureID::LampPostTileSet);

		mObject3DQuad[8].texCoords = sf::Vector2f(24, 0);
		mObject3DQuad[9].texCoords = sf::Vector2f(50, 0);
		mObject3DQuad[10].texCoords = sf::Vector2f(50, 24);
		mObject3DQuad[11].texCoords = sf::Vector2f(24, 24);

		mObject3DQuad[0].texCoords = sf::Vector2f(25, 25);
		mObject3DQuad[1].texCoords = sf::Vector2f(25, 100);
		mObject3DQuad[2].texCoords = sf::Vector2f(0, 100);
		mObject3DQuad[3].texCoords = sf::Vector2f(0, 25);

		mObject3DQuad[4].texCoords = sf::Vector2f(0, 25);
		mObject3DQuad[5].texCoords = sf::Vector2f(25, 25);
		mObject3DQuad[6].texCoords = sf::Vector2f(25, 100);
		mObject3DQuad[7].texCoords = sf::Vector2f(0, 100);

		zPos = 0;
		break;
	case Object3D::LampTop:

		objectTexture = texturesConstRef.get(TextureID::LampPostTileSet);

		mObject3DQuad[8].texCoords = sf::Vector2f(25, 25);
		mObject3DQuad[9].texCoords = sf::Vector2f(50, 25);
		mObject3DQuad[10].texCoords = sf::Vector2f(50, 50);
		mObject3DQuad[11].texCoords = sf::Vector2f(24, 50);

		mObject3DQuad[0].texCoords = sf::Vector2f(25, 25);
		mObject3DQuad[1].texCoords = sf::Vector2f(50, 25);
		mObject3DQuad[2].texCoords = sf::Vector2f(50, 50);
		mObject3DQuad[3].texCoords = sf::Vector2f(24, 50);

		mObject3DQuad[4].texCoords = sf::Vector2f(25, 25);
		mObject3DQuad[5].texCoords = sf::Vector2f(50, 25);
		mObject3DQuad[6].texCoords = sf::Vector2f(50, 50);
		mObject3DQuad[7].texCoords = sf::Vector2f(24, 50);

		zPos = 0.1;
		break;
	default:
		break;
	}

}

void Object3D::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &objectTexture;

	target.draw(mObject3DQuad, states);

	sf::Vector2f windowPos = TheGame::Instance()->getWorldView()->getCenter();
	sf::RenderWindow* pWin = TheGame::Instance()->getWindow();
}

void Object3D::update(sf::Time dTime)
{
	sf::Vector2f windowPos = TheGame::Instance()->getWorldView()->getCenter();

	//groundTopLeft = (groundCenter + sf::Vector2f(-dimentions.x/2, -dimentions.y/2) - windowPos + 1.5f*(sf::Vector2f)(dimentions));// * zPos;
	//groundTopRight = (groundCenter + sf::Vector2f(dimentions.x/2, -dimentions.y/2) - windowPos + 1.5f*(sf::Vector2f)(dimentions));// * zPos;
	//groundBotRight = (groundCenter + sf::Vector2f(dimentions.x/2, dimentions.y/2) - windowPos + 1.5f*(sf::Vector2f)(dimentions));// * zPos;
	//groundBotLeft = (groundCenter + sf::Vector2f(-dimentions.x/2, dimentions.y/2) - windowPos + 1.5f*(sf::Vector2f)(dimentions));// * zPos;

	//Roof
	mObject3DQuad[8].position = groundTopLeft + (groundTopLeft - windowPos) * mHeight + (groundTopLeft - windowPos) * zPos;
	mObject3DQuad[9].position = groundTopRight + (groundTopRight - windowPos) * mHeight + (groundTopRight - windowPos) * zPos;
	mObject3DQuad[10].position = groundBotRight + (groundBotRight - windowPos) * mHeight + (groundBotRight - windowPos) * zPos;
	mObject3DQuad[11].position = groundBotLeft + (groundBotLeft - windowPos) * mHeight + (groundBotLeft - windowPos) * zPos;

	//East/West Wall
	if (windowPos.x > groundTopRight.x)
	{ //Display East Wall
		mObject3DQuad[0].position = mObject3DQuad[9].position;
		mObject3DQuad[1].position = groundTopRight + (groundTopRight - windowPos) * zPos;
		mObject3DQuad[2].position = groundBotRight + (groundBotRight - windowPos) * zPos;
		mObject3DQuad[3].position = mObject3DQuad[10].position;
	}
	else if (windowPos.x < groundTopLeft.x)
	{ //Display West Wall
		mObject3DQuad[0].position = mObject3DQuad[8].position;
		mObject3DQuad[1].position = groundTopLeft + (groundTopLeft - windowPos) * zPos;
		mObject3DQuad[2].position = groundBotLeft + (groundBotLeft - windowPos) * zPos;
		mObject3DQuad[3].position = mObject3DQuad[11].position;
	}
	else
	{ //No wall to display
		mObject3DQuad[0].position = sf::Vector2f(0, 0);
		mObject3DQuad[1].position = sf::Vector2f(0, 0);
		mObject3DQuad[2].position = sf::Vector2f(0, 0);
		mObject3DQuad[3].position = sf::Vector2f(0, 0);
	}

	//North/South Wall
	if (windowPos.y > groundBotLeft.y)
	{ //Display South Wall
		mObject3DQuad[4].position = mObject3DQuad[11].position;
		mObject3DQuad[5].position = mObject3DQuad[10].position;
		mObject3DQuad[6].position = groundBotRight + (groundBotRight - windowPos) * zPos;
		mObject3DQuad[7].position = groundBotLeft + (groundBotLeft - windowPos) * zPos;
	}
	else if (windowPos.y < groundTopLeft.y)
	{ //Display North Wall
		mObject3DQuad[4].position = mObject3DQuad[9].position;
		mObject3DQuad[5].position = mObject3DQuad[8].position;
		mObject3DQuad[6].position = groundTopLeft + (groundTopLeft - windowPos) * zPos;
		mObject3DQuad[7].position = groundTopRight + (groundTopRight - windowPos) * zPos;
	}
	else
	{ //No wall to display
		mObject3DQuad[4].position = sf::Vector2f(0, 0);
		mObject3DQuad[5].position = sf::Vector2f(0, 0);
		mObject3DQuad[6].position = sf::Vector2f(0, 0);
		mObject3DQuad[7].position = sf::Vector2f(0, 0);
	}
}