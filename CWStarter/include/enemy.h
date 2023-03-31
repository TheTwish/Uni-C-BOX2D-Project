#pragma once

/*!
\file enemy.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "kinematicBlock.h"
#include <vector>

/*! \class enemy
\brief An enemy is a kinematic block that applies lerping to follow a set animation that destroys the player
*/
struct EnemyKeyframe
{
	float m_time;
	b2Vec2 m_linearOffset;
	float m_angularOffset;
};

class EnemyAnimation
{
private:
	std::vector<EnemyKeyframe> m_keyframes;
	float m_animationLength = 0;
	float m_currentTime = 0;
public:
	void addKeyframe(float timestep, b2Vec2 linearOffset, float angularOffset);
	void play(float timestep, b2Vec2& linearOffset, float& angularOffset);
};


class Enemy : public sf::Drawable
{
protected:

private:
	KinematicBlock m_platform;
	EnemyAnimation m_animation;
public:
	Enemy() {}; //!< Default Constructor
	Enemy(b2World* world, const sf::Vector2f& position, const sf::Vector2f& size, const float orientation, b2Body* anchor, EnemyAnimation myAnimation); //!< Complete contructor
	void update(float timestep); //!< Update rendering infomation 
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;//!<draw dynamic block to the world
};

