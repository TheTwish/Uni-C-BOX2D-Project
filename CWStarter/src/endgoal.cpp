#include "endgoal.h"

Endgoal::Endgoal(b2World* world, const sf::Vector2f& position, const sf::Vector2f& size, const float& orientation, CollisionFilter type)
{
	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;

	m_body = world->CreateBody(&l_bodyDef);
	m_body->SetUserData(this); // used by our contact listener

	l_shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.isSensor = true;//make sensor
	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;


	//!< There will never be a static block that isn't either Terrain, so no checks for arms, character or hand or Object
	switch (type)
	{
	case GOAL:
		m_renderShape.setFillColor(sf::Color::White);
		l_fixtureDef.filter.categoryBits = GOAL;
		l_fixtureDef.filter.maskBits = CHARACTER;
		break;
	default:
		m_renderShape.setFillColor(sf::Color::Black);
		break;
	}

	m_body->CreateFixture(&l_fixtureDef);

	m_renderShape.setPosition(position);
	m_renderShape.setSize(size);
	m_renderShape.setOrigin(size * 0.5f);
	m_renderShape.setRotation(orientation);
	m_renderShape.setOutlineThickness(0.f);
}

void Endgoal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_renderShape);
}
