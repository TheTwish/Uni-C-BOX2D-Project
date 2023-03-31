#pragma once

/*!
\file movingPlatform.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "kinematicBlock.h"
#include <vector>

/*! \class movingPlatform
\brief A moving platform is a kinematic block that applies lerping to follow a set animation
*/
struct PlatformKeyframe
{
	float m_time;
	b2Vec2 m_linearOffset;
	float m_angularOffset;
};

class MovingPlatformAnimation
{
private:
	std::vector<PlatformKeyframe> m_keyframes;
	float m_animationLength = 0;
	float m_currentTime = 0;
public:
	void addKeyframe(float timestep, b2Vec2 linearOffset, float angularOffset);
	void play(float timestep, b2Vec2& linearOffset, float& angularOffset);
};


class MovingPlatform : public sf::Drawable
{
protected:

private:
	KinematicBlock m_platform;
	MovingPlatformAnimation m_animation;
public:
	MovingPlatform() {}; //!< Default Constructor
	MovingPlatform(b2World * world, const sf::Vector2f& position, const sf::Vector2f &size, const float orientation, PhysicalThing::CollisionFilter type,b2Body* anchor,MovingPlatformAnimation myAnimation); //!< Complete contructor
	void update(float timestep); //!< Update rendering infomation 
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;//!<draw dynamic block to the world
};