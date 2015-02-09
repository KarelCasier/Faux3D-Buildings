#include "Game.h"
#include <algorithm>
#include <functional>
#include "HelperFunctions.hpp"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f); // = 0.6 seconds per frame, therefore 60fps

Game* Game::s_pInstance = nullptr;

Game::Game()
: mWindow(sf::VideoMode(1280, 720), "Faux3D-Buildings")
, mFont()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mWorldView(sf::Vector2f(0.f,0.f), sf::Vector2f(1280.0, 720.f))
, mUIView(sf::Vector2f(640.f, 360.f), sf::Vector2f(1280.f, 720.f))
, friction(0.9)
, moveVel(200)
, camPos(600, 600)
{
		//mWindow.setKeyRepeatEnabled(false);

	if (!mFont.loadFromFile("MYRIADPRO-BOLD.OTF"))
	{
		std::cerr << "Can't load font" << std::endl;
	}
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	mTextBackground.setSize(sf::Vector2f(125.f, 35.f));
	mTextBackground.setFillColor(sf::Color(110, 110, 110, 80));
	
	BuildingPtr b1(new Building(sf::Vector2f(-300, 100), 300, 300));
	BuildingPtr b2(new Building(sf::Vector2f(-300, 600), 300, 300));
	BuildingPtr b3(new Building(sf::Vector2f(-300, 1100), 300, 300));

	BuildingPtr b4(new Building(sf::Vector2f(200, 100), 300, 300));
	BuildingPtr b5(new Building(sf::Vector2f(200, 600), 300, 300));
	BuildingPtr b6(new Building(sf::Vector2f(200, 1100), 300, 300));

	BuildingPtr b7(new Building(sf::Vector2f(700, 100), 300, 300));
	BuildingPtr b8(new Building(sf::Vector2f(700, 600), 300, 300));
	BuildingPtr b9(new Building(sf::Vector2f(700, 1100), 300, 300));

	//BuildingPtr b10(new Building(sf::Vector2f(-300, -600), 1100, 600));

	buildingsVector.push_back(std::move(b1));
	buildingsVector.push_back(std::move(b2));
	buildingsVector.push_back(std::move(b3));
	buildingsVector.push_back(std::move(b4));
	buildingsVector.push_back(std::move(b5));
	buildingsVector.push_back(std::move(b6));
	buildingsVector.push_back(std::move(b7));
	buildingsVector.push_back(std::move(b8));
	buildingsVector.push_back(std::move(b9));
	//buildingsVector.push_back(std::move(b10));

	positionMarker = sf::CircleShape(5);
	ground = sf::RectangleShape(sf::Vector2f(3000, 3000));
	ground.setPosition(sf::Vector2f(-1500, -1500));
	ground.setFillColor(sf::Color(85,150,50));
	
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart(); //Note restart returns the time on the clock then restarts the clock
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) //Ensures that the logic is caught up before rendering again
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
	mWindow.close();
}

void Game::processEvents()
{

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}

	// realtime events
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousePos(sf::Mouse::getPosition(mWindow));
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		upPressed = true;
	}
	else
	{
		upPressed = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		rightPressed = true;
	}
	else
	{
		rightPressed = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		downPressed = true;
	}
	else
	{
		downPressed = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		leftPressed = true;
	}
	else
	{
		leftPressed = false;
	}


}

void Game::update(sf::Time dTime)
{
	if (upPressed)
	{
		camPos.y -= (moveVel* dTime.asSeconds());
	}
	if (rightPressed)
	{
		camPos.x += (moveVel* dTime.asSeconds());
	}
	if (downPressed)
	{
		camPos.y += (moveVel* dTime.asSeconds());
	}
	if (leftPressed)
	{
		camPos.x -= (moveVel* dTime.asSeconds());
	}
	mWorldView.setCenter(camPos);
	
	for (BuildingPtr& building : buildingsVector)
	{
		building->update(dTime);
	}
	positionMarker.setPosition(camPos);
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	std::stringstream strStream;

	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		strStream << "Frames / Second = " << mStatisticsNumFrames << "\n" <<
			"Time / Update = " << mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames << "us" << std::endl;

		mStatisticsText.setString(strStream.str());

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::render()
{
	mWindow.clear();

	mWindow.draw(ground);

	mWindow.setView(mWorldView);

	for (BuildingPtr& building : buildingsVector)
	{
		mWindow.draw(*building);
	}
	mWindow.draw(positionMarker);
	mWindow.setView(mUIView);
	mWindow.draw(mTextBackground);
	mWindow.draw(mStatisticsText);

	mWindow.display();
}