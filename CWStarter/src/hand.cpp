/*!
\file dynamicBlock.cpp
*/

#include "hand.h"
#include "character.h"


Hand::~Hand()
{
	m_touchingBodies.clear();
	std::vector <b2Body*> m_touchingBodies;
	//delete(m_parent);
	m_parent = nullptr;
}

Hand::Hand(b2World * world, const sf::Vector2f& position, const sf::Vector2f &size, const float orientation, CollisionFilter type,bool whatHand, sf::Texture* texture)
{
	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;
	l_bodyDef.type = b2_dynamicBody;

	m_touchingBodies.reserve(10);

	l_shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;

	m_world = world;

	l_bodyDef.fixedRotation = true;
	
	l_fixtureDef.filter.categoryBits = HAND;
	l_fixtureDef.filter.maskBits = CHARACTER | TERRAIN | OBJECT | COLLECTABLE;

	m_body = world->CreateBody(&l_bodyDef);

	m_fixture = m_body->CreateFixture(&l_fixtureDef);


	//sfml tiling/texures

	float textureWidth = texture->getSize().x / 2;
	float textureHeight = texture->getSize().y;

	m_renderShape.setPosition(position);
	m_renderShape.setScale(size.x / textureWidth+0.002f, size.y / textureHeight + 0.002f);
	m_renderShape.setOrigin(sf::Vector2f(textureWidth * 0.5, textureHeight * 0.5));
	m_renderShape.setRotation(orientation);

	m_renderShape.setTexture(*texture);
	m_renderShape.setTextureRect(sf::IntRect(0, 0, textureWidth, textureHeight));

}


void Hand::update()
{
	b2Vec2 pos = m_body->GetPosition();
	m_renderShape.setPosition(pos.x, pos.y);
	float angle = m_body->GetAngle()* RAD2DEG;
	m_renderShape.setRotation(angle);

	if (m_fixture->GetFilterData().categoryBits == PhysicalThing::HAND)//ensure I am hand (just for saftey)
	{
		if (m_touchingBodies.size() > 0 && m_grabbing && !m_weld)
		{
			//!< If contacting with grabbable object and player is grabbing with this hand, create weld joint between this dynamic object and the last thing I started colliding with that hasn't ended its collision
			b2WeldJointDef l_def;
			l_def.bodyA = m_body;//!< define body A
			l_def.bodyB = m_touchingBodies.back();//!< define body B

			l_def.referenceAngle = m_touchingBodies.back()->GetAngle();


			l_def.localAnchorB = m_contactPoints.back() - m_touchingBodies.back()->GetPosition() ;//!< define local anchor A, which is body B's position
			float x = l_def.localAnchorB.x * cos(-l_def.referenceAngle) - l_def.localAnchorB.y * sin(-l_def.referenceAngle);
			float y = l_def.localAnchorB.y * cos(-l_def.referenceAngle) + l_def.localAnchorB.x * sin(-l_def.referenceAngle);

			l_def.localAnchorB = b2Vec2(x, y);

			l_def.localAnchorA =  m_contactPoints.back() - m_body->GetPosition(); //!< define local anchor B, which is body A's position
			m_weld = static_cast<b2WeldJoint*>(m_world->CreateJoint(static_cast<b2JointDef*>(&l_def)));

			m_parent->onWeldCreate();
			
			m_holding = true;
		}
		else if (!m_grabbing)
		{
			if (m_weld)
			{
				m_world->DestroyJoint(m_weld);

				//delete m_weld;

				m_weld = nullptr;
				m_parent->onWeldDestroy();
				m_holding = false;
			}
			//m_renderShape.setFillColor(m_defaultColor);
		}
	}


}

void Hand::updateGrabbing(bool grabbing)
{
	m_grabbing = grabbing;
}

bool Hand::getHolding()
{
	return m_holding;
}

void Hand::setDensity(float Density)
{
	m_fixture->SetDensity(Density);
	m_body->ResetMassData();
}


void Hand::setRestitution(float Restitution)
{
	m_fixture->SetRestitution(Restitution);
	m_body->ResetMassData();
}

void Hand::setFriction(float Friction)
{
	m_fixture->SetFriction(Friction);
	m_body->ResetMassData();
}

sf::Color Hand::getColor()
{
	return m_defaultColor;
}

void Hand::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_renderShape);
}

void Hand::updateShapeTexture()
{
	if (m_grabbing)
	{
		m_renderShape.setTextureRect(sf::IntRect(0, 0, 100, 100));
	}
	else
	{
		m_renderShape.setTextureRect(sf::IntRect(100, 0, 100, 100));
	}
}

void Hand::startContact(b2Body* otherBody,b2Vec2 contactPoint)
{
	if (!m_parent->getRespawn())
	{
		m_contactPoints.push_back(contactPoint);
		m_touchingBodies.push_back(otherBody);
	}
}

void Hand::endContact(b2Body * otherBody)
{
	if (!m_parent->getRespawn())
	{
		int i = 0;
		for (auto it = m_touchingBodies.begin(); it != m_touchingBodies.end(); ++it)
		{
			if (otherBody == *it)
			{
				m_touchingBodies.erase(it);
				m_contactPoints.erase(m_contactPoints.begin() + i);
				break;
			}

			i++;
		}
	}
	else
	{
		m_touchingBodies.clear();
	}
}