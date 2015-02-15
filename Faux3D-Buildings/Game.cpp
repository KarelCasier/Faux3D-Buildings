#include "Game.h"
#include <algorithm>
#include <functional>
#include "Utilities.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f); // = 0.6 seconds per frame, therefore 60fps

Game* Game::s_pInstance = nullptr;														///< Initiate singleton pointer to nullptr

bool sortByDistance(Game::Object3DPtr& o1, Game::Object3DPtr& o2)						///< Custom comparison function for sorting the Object3Ds before rendering
{
	const sf::Vector2f& viewPos = TheGame::Instance()->getWorldView()->getCenter();
	return (Distance(viewPos, o1->getCenter()) > Distance(viewPos, o2->getCenter()));
}

Game::Game()
: mWindow(sf::VideoMode(2560, 1440), "Faux3DObjects")
, mFont()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mWorldView(sf::Vector2f(0.f, 0.f), (sf::Vector2f)mWindow.getSize())
, mUIView((sf::Vector2f)(mWindow.getSize()) / 2.f, (sf::Vector2f)mWindow.getSize())
, friction(0.9)
, moveVel(200)
, camPos(600, 600)
{
		//mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	mFonts.load(FontID::Default, "MYRIADPRO-BOLD.OTF");

	mStatisticsText.setFont(mFonts.get(FontID::Default));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	mTextBackground.setSize(sf::Vector2f(125.f, 35.f));
	mTextBackground.setFillColor(sf::Color(110, 110, 110, 80));

	mTextures.load(TextureID::Road2Way, "RoadTile.png");
	mTextures.load(TextureID::Road3Way, "3WayRoadIntersection.png");
	mTextures.load(TextureID::Road4Way, "RoadIntersection.png");
	mTextures.load(TextureID::BuildingTileSet, "BuildingTileSheet.png");
	mTextures.load(TextureID::LampPostTileSet, "LampPostTileSheet.png");

	std::array<std::array<int, 11>, 11> layout = 
	{ {
		{ 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4 },
		{ 1, 7, 1, 7, 1, 7, 1, 6, 1, 7, 1 },
		{ 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4 },
		{ 1, 7, 1, 7, 1, 6, 1, 8, 1, 7, 1 },
		{ 4, 2, 4, 2, 4, 7, 4, 2, 4, 2, 4 },
		{ 1, 7, 1, 7, 1, 5, 1, 6, 1, 7, 1 },
		{ 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4 },
		{ 1, 6, 1, 7, 1, 7, 1, 7, 1, 8, 1 },
		{ 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4 },
		{ 1, 7, 1, 7, 1, 6, 1, 7, 1, 7, 1 },
		{ 4, 2, 4, 2, 4, 2, 4, 2, 4, 2, 4 }
	} };

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if (layout[i][j] == 1)
			{
				sf::RectangleShape* tile(new sf::RectangleShape(sf::Vector2f(300, 300)));
				tile->setPosition(sf::Vector2f(i * 300 + 150, j * 300 + 150));
				tile->setTexture(&mTextures.get(TextureID::Road2Way));
				tile->setOrigin(150, 150);
				tile->rotate(90.f);
				groundTiles.push_back(tile);
			}
			else if (layout[i][j] == 2)
			{
				sf::RectangleShape* tile(new sf::RectangleShape(sf::Vector2f(300, 300)));
				tile->setPosition(sf::Vector2f(i * 300 + 150, j * 300 +150));
				tile->setTexture(&mTextures.get(TextureID::Road2Way));
				tile->setOrigin(150, 150);
				///atile->rotate(90.f);
				groundTiles.push_back(tile);
			}
			else if (layout[i][j] == 4)
			{
				sf::RectangleShape* tile(new sf::RectangleShape(sf::Vector2f(300, 300)));
				tile->setPosition(sf::Vector2f(i * 300, j * 300));
				tile->setTexture(&mTextures.get(TextureID::Road4Way));
				groundTiles.push_back(tile);

								///< Place Lamp Posts at the corner of each intersection block
				Object3DPtr lamp1(new Object3D(mTextures, sf::Vector2f(i * 300 + 25, j * 300 + 25), sf::Vector2i(10, 10), Object3D::Object3DType::LampPost, 0.1f));
				Object3DPtr lamp2(new Object3D(mTextures, sf::Vector2f(i * 300 + 270, j * 300 + 25), sf::Vector2i(10, 10), Object3D::Object3DType::LampPost, 0.1f));
				Object3DPtr lamp3(new Object3D(mTextures, sf::Vector2f(i * 300 + 25, j * 300 + 270), sf::Vector2i(10, 10), Object3D::Object3DType::LampPost, 0.1f));
				Object3DPtr lamp4(new Object3D(mTextures, sf::Vector2f(i * 300 + 270, j * 300 + 270), sf::Vector2i(10, 10), Object3D::Object3DType::LampPost, 0.1f));

				Object3DsVector.push_back(std::move(lamp1));
				Object3DsVector.push_back(std::move(lamp2));
				Object3DsVector.push_back(std::move(lamp3));
				Object3DsVector.push_back(std::move(lamp4));
			}
			else if (layout[i][j] == 5)
			{
				Object3DPtr building(new Object3D(mTextures, sf::Vector2f(i * 300, j * 300), sf::Vector2i(300, 300), Object3D::Object3DType::Building, 0.25f));
				Object3DsVector.push_back(std::move(building));
			}
			else if (layout[i][j] == 6)
			{
				Object3DPtr building(new Object3D(mTextures, sf::Vector2f(i * 300, j * 300), sf::Vector2i(300, 300), Object3D::Object3DType::Building, 0.5f));
				Object3DsVector.push_back(std::move(building));
			}
			else if (layout[i][j] == 7)
			{
				Object3DPtr building(new Object3D(mTextures, sf::Vector2f(i * 300, j * 300), sf::Vector2i(300, 300), Object3D::Object3DType::Building, 1.f));
				Object3DsVector.push_back(std::move(building));
			}
			else if (layout[i][j] == 8)
			{
				Object3DPtr building(new Object3D(mTextures, sf::Vector2f(i * 300, j * 300), sf::Vector2i(300, 300), Object3D::Object3DType::Building, 1.5f));
				Object3DsVector.push_back(std::move(building));
			}
			
		}
	}

	//Object3DPtr lamp1(new Object3D(mTextures, sf::Vector2f(0 * 300 + 25, 0 * 300 + 25), sf::Vector2i(10, 10), Object3D::Object3DType::LampPost, 0.075f));
	//Object3DPtr lamp2(new Object3D(mTextures, sf::Vector2f(625, 625), sf::Vector2i(10, 10), Object3D::Object3DType::LampPost, 0.075f));
	//Object3DsVector.push_back(std::move(lamp1));
	//Object3DsVector.push_back(std::move(lamp2));

	positionMarker = sf::CircleShape(5);
	positionMarker.setFillColor(sf::Color(255, 255, 255));
	positionMarker.setOrigin(5, 5);
	ground = sf::RectangleShape(sf::Vector2f(3000, 3000));
	ground.setPosition(sf::Vector2f(-0, -0));
	ground.setFillColor(sf::Color(50,50,50));
	
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
		std::cout << vectorToStr(camPos) << std::endl;
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
	
	for (Object3DPtr& Object3D : Object3DsVector)
	{
		Object3D->update(dTime);
	}
	positionMarker.setPosition(camPos);

	std::sort(Object3DsVector.begin(), Object3DsVector.end(), sortByDistance);
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

	for (const sf::RectangleShape* tile : groundTiles)
	{
		mWindow.draw(*tile);
	}

	for (Object3DPtr& Object3D : Object3DsVector)
	{
		mWindow.draw(*Object3D);
	}
	mWindow.draw(positionMarker);
	mWindow.setView(mUIView);
	mWindow.draw(mTextBackground);
	mWindow.draw(mStatisticsText);

	mWindow.display();
}