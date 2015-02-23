#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Object3D.h"

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

class Game : private sf::NonCopyable
{
public:

	typedef std::unique_ptr<Object3D> Object3DPtr;

	static Game* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new Game();
		}
		return s_pInstance;
	}

	Game();
	void					run();

	sf::RenderWindow*		getWindow() { return &mWindow; }
	sf::View*				getWorldView() { return &mWorldView; }

private:
	void					processEvents();
	void					update(sf::Time dtTime);
	void					render();

	void					updateStatistics(sf::Time elapsedTime);
	void                    handlePlayerInput(sf::Keyboard::Key key, bool state);

private:
	static Game*			s_pInstance;
	//For FPS calculations and consistent movement
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;

	sf::Font				mFont;
	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;
	sf::RectangleShape		mTextBackground;

	sf::View				mUIView;
	sf::View				mWorldView;
	float                   worldZoom;

	std::vector<Object3DPtr>		Object3DsVector;
	std::vector<sf::RectangleShape*>groundTiles;

	sf::Vector2f			camPos;
	float					moveVel;
	float					friction;

	bool					upPressed, rightPressed, downPressed, leftPressed;

	sf::CircleShape			positionMarker;
	sf::RectangleShape		ground;

	TextureHolder			mTextures;
	FontHolder				mFonts;

};

typedef Game TheGame;
