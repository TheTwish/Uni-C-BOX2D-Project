#pragma once

/*!
\file respawner.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "physicalThing.h"

/*! \class respawner
\brief 
*/

class Respawner : public sf::Drawable, public PhysicalThing
{
protected:
	sf::RectangleShape m_renderShape;
public:
	Respawner() {}; //!< Default Contructor;
	Respawner(b2World* world, const sf::Vector2f& position, const sf::Vector2f& size, const float& orientation, CollisionFilter type); //!< Complete contructor
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};