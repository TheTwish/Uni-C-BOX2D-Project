/*!
\file collectable.cpp
*/

#include "collectable.h"

Collectable::Collectable(b2World* world, const sf::Vector2f& position, const float& radius, const float& orientation, CollisionFilter type)
{
	b2BodyDef l_bodyDef;
	b2CircleShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;

	m_body = world->CreateBody(&l_bodyDef);

	l_shape.m_radius = radius;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;


	switch (type)
	{
	case COLLECTABLE:
		m_renderShape.setFillColor(sf::Color::Yellow);
		l_fixtureDef.filter.categoryBits = COLLECTABLE;
		l_fixtureDef.filter.maskBits = HAND | TERRAIN | OBJECT;
		break;
	default:
		m_renderShape.setFillColor(sf::Color::Black);
		break;
	}

	m_body->CreateFixture(&l_fixtureDef);

	m_renderShape.setPosition(position);
	m_renderShape.setRadius(radius);
	m_renderShape.setOrigin(radius,radius);
	m_renderShape.setRotation(orientation);
	m_renderShape.setOutlineThickness(0.f);
}

void Collectable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_renderShape);
}

void Collectable::registerForDeletion()
{
	m_registeredForDeletion = true;
}

bool Collectable::registeredForDeletion()
{
	return m_registeredForDeletion;
}
