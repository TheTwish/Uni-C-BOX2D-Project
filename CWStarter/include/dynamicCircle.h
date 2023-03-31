#pragma once

/*!
\file dynamicCircle.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "physicalThing.h"

/*! \class DynamicCircle
\brief A simple circle which can move, rotate and collide with stuff
*/

class DynamicCircle : public sf::CircleShape, public PhysicalThing
{
private:
public:
	DynamicCircle() {}; //!< Default Constructor
	DynamicCircle(b2World* world, const sf::Vector2f& position, float radius, float orientation, CollisionFilter type); //!< Complete contructor
	void update(); //!< Update rendering infomation 
	void draw(sf::RenderTarget& target, sf::RenderStates states) const; //!< Overridden drawing methos to include line to allow users to see oreintation of the ball
};