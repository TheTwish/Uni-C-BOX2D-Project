#pragma once

/*!
\file dynamicBlock.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "DynamicBlock.h"
#include <vector>
#include "hand.h"
#include "textureManager.h"

/*! \class DynamicBlock
\brief A simple block which can move, rotate and collide with stuff
*/

class Character : public sf::Drawable
{
protected:
private:
	//!< current number of bodies I am colliding with, will only update in hand object as that is all we care about when it collides
	b2World* m_world;//add to character class
	bool m_grabbing = false;
	bool m_respawn = false;
	bool m_end = false;
	std::vector<DynamicBlock> m_characterComponents; //!< the playable character
	std::vector<Hand> m_characterHands;//!< the playable characters hands
	float m_armSegmentWidth = 0.45f;//!< the length of the player arms, could be fun to change

	bool m_handStates[6]{ 0,0,0,0,0,0 };//!< keeps tracks of the current hand states, in order: moving up, moving left, moving down, moving right, right hand closed, left hand closed
	void updateCharacterHands();
	enum handStates {UP, LEFT,DOWN,RIGHT,RCLOSED,LCLOSED};

public:
	Character() {}; //!< Default Constructor
	~Character();
	Character(b2World * world,float x,float y,TextureManager& myTextureManager); //!< Complete contructor
	void update(); //!< Update rendering infomation 

	void onWeldCreate();
	void onWeldDestroy();

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

	void onMousePress(const sf::Event& event);
	void onMouseRelease(const sf::Event& event);
	void onKeyPress(const sf::Event& event);
	void onKeyRelease(const sf::Event& event);

	void registerForRespawn() { m_respawn = true; };
	void registerForEnd() { m_end = true; };

	b2Vec2 getPosition();
	bool getRespawn() { return m_respawn; };
	void setRespawn(bool respawn) { m_respawn = respawn; };

	bool getEnd() { return m_end; };
	void setEnd(bool end) { m_end = end; };


};