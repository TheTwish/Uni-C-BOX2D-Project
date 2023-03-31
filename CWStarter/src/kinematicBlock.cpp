/*!
\file dynamicBlock.cpp
*/

#include "kinematicBlock.h"

KinematicBlock::KinematicBlock(b2World * world, const sf::Vector2f& position, const sf::Vector2f &size, const float orientation, CollisionFilter type)
{
	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;
	l_bodyDef.type = b2_kinematicBody;


	l_shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;

	m_world = world;

	switch (type)
	{
	case TERRAIN:
		m_defaultColor = sf::Color::Green;
		m_renderShape.setFillColor(sf::Color::Green);
		l_fixtureDef.filter.categoryBits = TERRAIN;
		l_fixtureDef.filter.maskBits = CHARACTER | ARM | HAND | TERRAIN | OBJECT;
		break;
	case OBJECT:
		m_defaultColor = sf::Color::Green;
		m_renderShape.setFillColor(sf::Color::Green);
		l_fixtureDef.filter.categoryBits = OBJECT;
		l_fixtureDef.filter.maskBits = CHARACTER | ARM | HAND | TERRAIN | OBJECT;
	case ENEMY:
		m_defaultColor = sf::Color::Green;
		m_renderShape.setFillColor(sf::Color::Black);
		l_fixtureDef.filter.categoryBits = ENEMY;
		l_fixtureDef.filter.maskBits = CHARACTER;
		break;
	default:
		m_defaultColor = sf::Color::Black;
		m_renderShape.setFillColor(sf::Color::Black);
		break;
	}

	m_body = world->CreateBody(&l_bodyDef);

	m_fixture = m_body->CreateFixture(&l_fixtureDef);

	m_renderShape.setPosition(position);
	m_renderShape.setSize(size);
	m_renderShape.setOrigin(size * 0.5f);
	m_renderShape.setRotation(orientation);
	m_renderShape.setOutlineThickness(0.f);
}


void KinematicBlock::update()
{
	b2Vec2 pos = m_body->GetPosition();
	m_renderShape.setPosition(pos.x, pos.y);
	float angle = m_body->GetAngle()* RAD2DEG;
	m_renderShape.setRotation(angle);
}

void KinematicBlock::setDensity(float Density)
{
	m_fixture->SetDensity(Density);
	m_body->ResetMassData();
}


void KinematicBlock::setRestitution(float Restitution)
{
	m_fixture->SetRestitution(Restitution);
	m_body->ResetMassData();
}

void KinematicBlock::setFriction(float Friction)
{
	m_fixture->SetFriction(Friction);
	m_body->ResetMassData();
}

void KinematicBlock::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_renderShape);
}