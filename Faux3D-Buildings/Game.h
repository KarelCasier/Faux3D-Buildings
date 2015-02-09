#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <iostream>
#include <sstream>
#include <array>
#include <vector>
#include "Building.h"

class Game : private sf::NonCopyable
{
public:

	typedef std::unique_ptr<Building> BuildingPtr;

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

	std::vector<BuildingPtr>buildingsVector;

	sf::Vector2f			camPos;
	float					moveVel;
	float					friction;

	bool					upPressed, rightPressed, downPressed, leftPressed;

	sf::CircleShape			positionMarker;
	sf::RectangleShape		ground;

};

typedef Game TheGame;
