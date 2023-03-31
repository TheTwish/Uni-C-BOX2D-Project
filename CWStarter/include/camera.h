#pragma once
/*!
\file camera.h
*/
#include <SFML/Graphics.hpp>

/*! \class camera
\brief The view maps from physical co-ordinates to rendering co-ordinate. The Camera class controls the view
*/

class Camera
{
private:
	sf::View m_view;

public:
	Camera() {};
	Camera(sf::Vector2f worldSize);
	void setView(float X,float Y);
	sf::View getView()const;
	~Camera();
};