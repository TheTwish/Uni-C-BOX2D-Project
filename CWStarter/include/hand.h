#pragma once

/*!
\file dynamicBlock.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "physicalThing.h"
#include <vector>
#include "dynamicBlock.h"
class Character;//!< Forward decleration of the class


/*! \class DynamicBlock
\brief A simple block which can move, rotate and collide with stuff
*/

class Hand : public DynamicBlock
{
protected:
	
private:
	sf::Sprite m_renderShape;
	Character* m_parent;
	b2WeldJoint* m_weld = nullptr;
	std::vector <b2Body*> m_touchingBodies;//!< current number of bodies I am colliding with
	std::vector <b2Vec2> m_contactPoints;
	bool m_grabbing = false;//!< boolean to keep track of if hand is closed
	bool m_holding = false;//!< boolean to keep track of if hand is successfully holding onto something

public:
	Hand() {}; //!< Default Constructor
	~Hand();
	Hand(b2World * world, const sf::Vector2f& position, const sf::Vector2f &size, const float orientation, CollisionFilter type, bool whatHand, sf::Texture* texture); //!< Complete contructor
	void update(); //!< Update rendering infomation 
	void setDensity(float Density);
	void setRestitution(float Restitution);
	void setFriction(float Friction);
	sf::Color getColor();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void updateGrabbing(bool grabbing);
	bool getHolding();
	void setCharacterData(Character* character) { m_parent = character; }; //!< Set the userdata of the body

	void updateShapeTexture();
	//!< collision handling, used by the custom contact listener
	void startContact(b2Body* otherBody,b2Vec2 contactPoint);
	void endContact(b2Body* otherBody);

};