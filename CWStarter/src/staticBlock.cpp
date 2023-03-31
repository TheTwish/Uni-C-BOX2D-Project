/*!
\file staticBlock.cpp
*/

#include "staticBlock.h"


StaticBlock::StaticBlock(b2World * world, const sf::Vector2f& position, const sf::Vector2f& size, const float& orientation, CollisionFilter type,sf::Texture* texture)
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

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;

	//sfml tiling/texures
	texture->setRepeated(true);

	float textureWidth = texture->getSize().x;
	float textureHeight = texture->getSize().y;

	m_renderShape.setPosition(sf::Vector2f(position.x - (size.x / 2 -0.25f), position.y));
	m_renderShape.setScale(size.x / textureWidth / (size.x * 4), size.y / textureHeight);
	m_renderShape.setOrigin(sf::Vector2f(textureWidth * 0.5, textureHeight * 0.5));
	m_renderShape.setRotation(orientation);

	m_renderShape.setTexture(*texture);
	m_renderShape.setTextureRect(sf::IntRect(0, 0, textureWidth * (size.x * 4), textureHeight));




	//!< There will never be a static block that isn't either Terrain, so no checks for arms, character or hand or Object
	switch (type)
	{
	case TERRAIN:
		l_fixtureDef.filter.categoryBits = TERRAIN;
		l_fixtureDef.filter.maskBits = CHARACTER | ARM | HAND | TERRAIN | OBJECT;
		break;
	default:
		//do nothing
		break;
	}

	m_body->CreateFixture(&l_fixtureDef);




}

void StaticBlock::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_renderShape);
}
