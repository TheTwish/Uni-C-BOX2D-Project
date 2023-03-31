#pragma once

/*!
\file collectable.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "physicalThing.h"

/*! \class Collectable
\brief A collectable item with sensor
*/

class Collectable : public sf::Drawable, public PhysicalThing
{
protected:
	sf::CircleShape m_renderShape;
	bool m_registeredForDeletion = false;
public:
	Collectable() {}; //!< Default Contructor;
	Collectable(b2World* world, const sf::Vector2f& position, const float& radius, const float& orientation, CollisionFilter type); //!< Complete contructor
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void registerForDeletion();
	bool registeredForDeletion();
};