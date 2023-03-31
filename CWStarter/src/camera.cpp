#include "camera.h"

/*!
\file camera.cpp
*/
Camera::Camera(sf::Vector2f worldSize)
{
	m_view.setCenter(0.f, 0.f);
	m_view.setSize(worldSize);
}

void Camera::setView(float X,float Y)
{
	m_view.setCenter(X,Y);
}

sf::View Camera::getView() const
{
	return m_view;
}


Camera::~Camera()
{
}