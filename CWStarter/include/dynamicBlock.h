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
class Character;//!< Forward decleration of the class

class DynamicBlock : public sf::Drawable, public PhysicalThing
{
protected:
	sf::Sprite m_renderShape;
	b2World* m_world;
	b2Fixture* m_fixture;
	sf::Color m_defaultColor;
private:
	
	Character* m_parent;
public:
	DynamicBlock() {}; //!< Default Constructor
	DynamicBlock(b2World * world,const sf::Vector2f& position, const sf::Vector2f &size, const float orientation, CollisionFilter type, sf::Texture* texture); //!< Complete contructor
	void update(); //!< Update rendering infomation 
	void setDensity(float Density);
	void setRestitution(float Restitution);
	void setFriction(float Friction);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setCharacterData(Character* character) 
	{ 
		m_parent = character;
	}; //!< Set the userdata of the body
	void registerForRespawn();
	void registerForEnd();
};