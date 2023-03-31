/*!
\file dynamicBlock.cpp
*/

#include "dynamicBlock.h"
#include "character.h"

DynamicBlock::DynamicBlock(b2World * world, const sf::Vector2f& position, const sf::Vector2f &size, const float orientation, CollisionFilter type,sf::Texture* texture)
{
	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;
	l_bodyDef.type = b2_dynamicBody;


	l_shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;

	m_world = world;

	switch (type)
	{
	case CHARACTER:
		l_fixtureDef.filter.categoryBits = CHARACTER;
		l_fixtureDef.filter.maskBits = TERRAIN | CHARACTER | OBJECT | RESPAWNER | ENEMY | GOAL;
		l_fixtureDef.friction = 1.0;
		l_fixtureDef.density = 100;
		break;
	case ARM:
		l_fixtureDef.filter.categoryBits = ARM;
		l_fixtureDef.density = 15;
		l_fixtureDef.filter.maskBits = CHARACTER | TERRAIN | OBJECT;
		break;
	case TERRAIN:
		l_fixtureDef.filter.categoryBits = TERRAIN;
		l_fixtureDef.filter.maskBits = CHARACTER | ARM | HAND | TERRAIN  | OBJECT | COLLECTABLE;
		break;
	case OBJECT:
		l_fixtureDef.filter.categoryBits = OBJECT;
		l_fixtureDef.filter.maskBits = CHARACTER | ARM | HAND | TERRAIN | OBJECT | COLLECTABLE;
		break;
	default:
		break;
	}

	m_body = world->CreateBody(&l_bodyDef);

	m_fixture = m_body->CreateFixture(&l_fixtureDef);


	float textureWidth = texture->getSize().x;
	float textureHeight = texture->getSize().y;

	m_renderShape.setPosition(position);
	m_renderShape.setScale(size.x / textureWidth, size.y / textureHeight);
	m_renderShape.setOrigin(sf::Vector2f(textureWidth * 0.5, textureHeight * 0.5));
	m_renderShape.setRotation(orientation);

	m_renderShape.setTexture(*texture);
	m_renderShape.setTextureRect(sf::IntRect(0, 0, textureWidth, textureHeight));
}


void DynamicBlock::update()
{
	b2Vec2 pos = m_body->GetPosition();
	m_renderShape.setPosition(pos.x, pos.y);
	float angle = m_body->GetAngle()* RAD2DEG;
	m_renderShape.setRotation(angle);
}

void DynamicBlock::setDensity(float Density)
{
	m_fixture->SetDensity(Density);
	m_body->ResetMassData();
}


void DynamicBlock::setRestitution(float Restitution)
{
	m_fixture->SetRestitution(Restitution);
	m_body->ResetMassData();
}

void DynamicBlock::setFriction(float Friction)
{
	m_fixture->SetFriction(Friction);
	m_body->ResetMassData();
}

void DynamicBlock::draw(sf::RenderTarget & target, sf::RenderStates states) const
{

	target.draw(m_renderShape);
}

void DynamicBlock::registerForRespawn()
{
	m_parent->registerForRespawn();
}

void DynamicBlock::registerForEnd()
{
	m_parent->registerForEnd();
}

