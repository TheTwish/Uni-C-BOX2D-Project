/*!
\file main.cpp
\brief Contains the entry point for the application
*/

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>

#include "game.h"

void main() /** Entry point for the application */
{
	sf::RenderWindow window(sf::VideoMode(1024, 800), "CW Starter"); //!< Open main window
	Game game;
	float fFrameTime = 1.f / 60.f;

	//!< Start a clock to provide time for the physics
	sf::Clock clock;

	//!< Run a game loop
	while (window.isOpen())
	{
		window.clear(sf::Color(120, 120, 120));
		sf::Event event;
		while (window.pollEvent(event))
		{
			//!< window closed button pressed
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed) game.onMousePress(event);
			if (event.type == sf::Event::MouseButtonReleased) game.onMouseRelease(event);
			if (event.type == sf::Event::KeyPressed) game.onKeyPress(event);
			if (event.type == sf::Event::KeyReleased) game.onKeyRelease(event);
			
			if (sf::Joystick::isConnected(0))
			{
				//!< joystick number 0 is connected
				float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
				float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
				//do something
			}

		}

		//!< Find out how much time has elapsed
		float m_fElapsedTime = clock.getElapsedTime().asSeconds();
		//!< If a frame has past the update the physics
		if (m_fElapsedTime > fFrameTime)
		{
			game.update(m_fElapsedTime);
			clock.restart();
		}

		//window.clear(sf::Color(120,120,120));

		window.draw(game);
		window.display();
	}



}