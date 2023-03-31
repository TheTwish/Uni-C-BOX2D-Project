#pragma once

/*!
\file dynamicBlock.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "physicalThing.h"
#include <vector>


/*! \class DynamicBlock
\brief A simple block which can move, rotate and collide with stuff
*/

class KinematicBlock : public sf::Drawable, public PhysicalThing
{
protected:
	sf::RectangleShape m_renderShape;
	b2World* m_world;
	b2Fixture* m_fixture;
	sf::Color m_defaultColor;
private:


public:
	KinematicBlock() {}; //!< Default Constructor
	KinematicBlock(b2World * world, const sf::Vector2f& position, const sf::Vector2f &size, const float orientation, CollisionFilter type); //!< Complete contructor
	void update(); //!< Update rendering infomation 
	void setDensity(float Density);
	void setRestitution(float Restitution);
	void setFriction(float Friction);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateShapeColor(sf::Color color) { m_renderShape.setFillColor(color); };
};