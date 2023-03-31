/*!
\file dynamicBlock.cpp
*/

#include "character.h"


Character::~Character()
{
	m_characterComponents.clear();
	m_characterHands.clear();


	//clear any pointers
	//clear touching bodies
}

Character::Character(b2World * world,float x,float y,TextureManager& myTextureManager)
{
	m_world = world;
	m_characterComponents.reserve(7);

	m_characterComponents.resize(7);
	//!< Create the character at the chosen location
	m_characterComponents[0] = DynamicBlock(m_world, sf::Vector2f(x, y), sf::Vector2f(0.5f, 0.5f), 0.f, PhysicalThing::CollisionFilter::CHARACTER, myTextureManager.getTexture("playerface"));//!< main body of the character

	m_characterComponents[1] = DynamicBlock(m_world, sf::Vector2f(x, y), sf::Vector2f(m_armSegmentWidth, 0.1f), 0.f, PhysicalThing::CollisionFilter::ARM, myTextureManager.getTexture("arm"));//!< shoulder joint of the right arm, position does not matter as it is joined to the main body
	m_characterComponents[2] = DynamicBlock(m_world, sf::Vector2f(x, y), sf::Vector2f(m_armSegmentWidth, 0.1f), 0.f, PhysicalThing::CollisionFilter::ARM, myTextureManager.getTexture("arm"));//!< elbow joint of the right arm, position does not matter as it is joined to the left shoulder joint
	m_characterComponents[3] = DynamicBlock(m_world, sf::Vector2f(x, y), sf::Vector2f(m_armSegmentWidth, 0.1f), 0.f, PhysicalThing::CollisionFilter::ARM, myTextureManager.getTexture("arm"));//!< wrist joint of the right arm, position does not matter as it is joined to the left elbow joint

	m_characterComponents[4] = DynamicBlock(m_world, sf::Vector2f(x, y), sf::Vector2f(m_armSegmentWidth, 0.1f), 0.f, PhysicalThing::CollisionFilter::ARM, myTextureManager.getTexture("arm"));//!< shoulder joint of the left arm, position does not matter as it is joined to the main body
	m_characterComponents[5] = DynamicBlock(m_world, sf::Vector2f(x, y), sf::Vector2f(m_armSegmentWidth, 0.1f), 0.f, PhysicalThing::CollisionFilter::ARM, myTextureManager.getTexture("arm"));//!< elbow joint of the left arm, position does not matter as it is joined to the left shoulder joint
	m_characterComponents[6] = DynamicBlock(m_world, sf::Vector2f(x, y), sf::Vector2f(m_armSegmentWidth, 0.1f), 0.f, PhysicalThing::CollisionFilter::ARM, myTextureManager.getTexture("arm"));//!< wrist joint of the left arm, position does not matter as it is joined to the left elbow joint

	m_characterHands.resize(2);
	m_characterHands[0] = Hand(m_world, sf::Vector2f(x, y), sf::Vector2f(0.3f, 0.3f), 0.f, PhysicalThing::CollisionFilter::HAND,1, myTextureManager.getTexture("right_hand"));//!< hand of the right arm, position does not matter as it is joined to the right wrist joint
	m_characterHands[1] = Hand(m_world, sf::Vector2f(x, y), sf::Vector2f(0.3f, 0.3f), 0.f, PhysicalThing::CollisionFilter::HAND,0,myTextureManager.getTexture("left_hand"));//!< hand of the left arm, position does not matter as it is joined to the left wrist joint
	
	
	m_characterComponents[0].setCharacterData(this);//!< let the main body know who their parent is
	for (int i = 0; i < 7; i++)
	{
		m_characterComponents[i].setUserData(&m_characterComponents[i]);
		
	}
	for (int i = 0; i < 2; i++)
	{
		m_characterHands[i].setCharacterData(this);//!< let the hands know who their parent is
		m_characterHands[i].setUserData(&m_characterHands[i]);
	}


#pragma region Joints
	float l_localAnchorLocation = m_armSegmentWidth / 2.f - 0.01f;//!< -0.01 used to create an overlap between the joins to give the effect it is all one object

	//set up the definition for a revolute joint
	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.collideConnected = false;

#pragma region right arm + hand
	//!< Create definitions for joint between body and right shoulder
	revoluteJointDef.bodyA = m_characterComponents[0].GetBody();
	revoluteJointDef.bodyB = m_characterComponents[1].GetBody();
	revoluteJointDef.localAnchorA.Set(0.25f, 0.f);
	revoluteJointDef.localAnchorB.Set(-l_localAnchorLocation, 0.f);
	//create the joint
	m_world->CreateJoint(&revoluteJointDef);//!< No need to save pointer of joint using "b2RevoluteJoint* joint = (b2RevoluteJoint*)"m_pWorld->CreateJoint(&revoluteJointDef) as these joints shall not be interacted with at any other point

	//!< Create definitions for joint between right shoulder and right elbow
	revoluteJointDef.bodyA = m_characterComponents[1].GetBody();
	revoluteJointDef.bodyB = m_characterComponents[2].GetBody();
	revoluteJointDef.localAnchorA.Set(l_localAnchorLocation, 0.f);
	revoluteJointDef.localAnchorB.Set(-l_localAnchorLocation, 0.f);
	//create the joint
	m_world->CreateJoint(&revoluteJointDef);

	//!< Create definitions for joint between right elbow and right wrist
	revoluteJointDef.bodyA = m_characterComponents[2].GetBody();
	revoluteJointDef.bodyB = m_characterComponents[3].GetBody();
	revoluteJointDef.localAnchorA.Set(l_localAnchorLocation, 0.f);
	revoluteJointDef.localAnchorB.Set(-l_localAnchorLocation, 0.f);
	//create the joint
	m_world->CreateJoint(&revoluteJointDef);

	//!< Create definitions for joint between right wrist and the right hand
	revoluteJointDef.bodyA = m_characterComponents[3].GetBody();
	revoluteJointDef.bodyB = m_characterHands[0].GetBody();
	revoluteJointDef.localAnchorA.Set(l_localAnchorLocation, 0.f);
	revoluteJointDef.localAnchorB.Set(0.f, 0.f);
	//create the joint
	m_world->CreateJoint(&revoluteJointDef);
#pragma endregion

#pragma region left arm + hand
	//!< Create definitions for joint between body and left shoulder
	revoluteJointDef.bodyA = m_characterComponents[0].GetBody();
	revoluteJointDef.bodyB = m_characterComponents[4].GetBody();
	revoluteJointDef.localAnchorA.Set(-0.25, 0);
	revoluteJointDef.localAnchorB.Set(l_localAnchorLocation, 0);
	//create the joint
	m_world->CreateJoint(&revoluteJointDef);

	//!< Create definitions for joint between left shoulder and left elbow
	revoluteJointDef.bodyA = m_characterComponents[4].GetBody();
	revoluteJointDef.bodyB = m_characterComponents[5].GetBody();
	revoluteJointDef.localAnchorA.Set(-l_localAnchorLocation, 0);
	revoluteJointDef.localAnchorB.Set(l_localAnchorLocation, 0);
	//create the joint
	m_world->CreateJoint(&revoluteJointDef);

	//!< Create definitions for joint between left elbow and left wrist
	revoluteJointDef.bodyA = m_characterComponents[5].GetBody();
	revoluteJointDef.bodyB = m_characterComponents[6].GetBody();
	revoluteJointDef.localAnchorA.Set(-l_localAnchorLocation, 0);
	revoluteJointDef.localAnchorB.Set(l_localAnchorLocation, 0);
	//create the joint
	m_world->CreateJoint(&revoluteJointDef);

	//!< Create definitions for joint between left wrist and the left hand
	revoluteJointDef.bodyA = m_characterComponents[6].GetBody();
	revoluteJointDef.bodyB = m_characterHands[1].GetBody();
	revoluteJointDef.localAnchorA.Set(-l_localAnchorLocation, 0);
	revoluteJointDef.localAnchorB.Set(0, 0);
	//create the joint
	m_world->CreateJoint(&revoluteJointDef);
#pragma endregion

#pragma endregion
	//b2RevoluteJoint* myJoint = (b2RevoluteJoint*)"m_pWorld->CreateJoint(&revoluteJointDef)
	//How to destroy a point: world->DestroyJoint( myJoint );
}

void Character::update()
{

	if (m_characterHands[0].getHolding() || m_characterHands[1].getHolding())
	{
		//std::cout << "Made Light" << endl;
		m_characterComponents[0].setDensity(0.1f);//!< change density of character when hand is holding onto object to allow movement
	}
	else
	{
		//std::cout << "Made Heavy" << endl;
		m_characterComponents[0].setDensity(100.f);//!< change density of character when hand is not holding onto object to disallow movement
	}
	for (auto& dblock : m_characterHands) dblock.update();
	for (auto& dblock : m_characterComponents) dblock.update();
	updateCharacterHands();
}

void Character::updateCharacterHands()
{
	if (m_respawn)
	{
		m_characterHands[0].updateShapeTexture();
		m_characterHands[0].updateGrabbing(false);

		m_characterHands[1].updateShapeTexture();
		m_characterHands[1].updateGrabbing(false);
	}
	else
	{
		float power = 30.f;
		//!< move hands up

		if (m_handStates[0])
		{
			float power = 40.f;
			m_characterHands[0].GetBody()->ApplyForceToCenter(b2Vec2(0.f, -power), true);
			m_characterHands[1].GetBody()->ApplyForceToCenter(b2Vec2(0.f, -power), true);
		}
		//!< move hands left
		if (m_handStates[1])
		{
			m_characterHands[0].GetBody()->ApplyForceToCenter(b2Vec2(-power, 0.f), true);
			m_characterHands[1].GetBody()->ApplyForceToCenter(b2Vec2(-power, 0.f), true);
		}
		//!< move hands down
		if (m_handStates[2])
		{
			m_characterHands[0].GetBody()->ApplyForceToCenter(b2Vec2(0.f, power), true);
			m_characterHands[1].GetBody()->ApplyForceToCenter(b2Vec2(0.f, power), true);
		}
		//!< move hands right
		if (m_handStates[3])
		{
			m_characterHands[0].GetBody()->ApplyForceToCenter(b2Vec2(power, 0.f), true);
			m_characterHands[1].GetBody()->ApplyForceToCenter(b2Vec2(power, 0.f), true);
		}
		//!< close/open right hand
		if (m_handStates[4])
		{
			m_characterHands[0].updateShapeTexture();
			m_characterHands[0].updateGrabbing(true);
		}
		else
		{
			m_characterHands[0].updateShapeTexture();
			m_characterHands[0].updateGrabbing(false);

		}
		//!< close/open left hand
		if (m_handStates[5])
		{
			m_characterHands[1].updateShapeTexture();
			m_characterHands[1].updateGrabbing(true);
		}
		else
		{
			m_characterHands[1].updateShapeTexture();
			m_characterHands[1].updateGrabbing(false);
		}
	}

}

void Character::onWeldCreate()
{
	
}

void Character::onWeldDestroy()
{
}

void Character::draw(sf::RenderTarget & target, sf::RenderStates state) const
{
	for (auto dblock : m_characterComponents) target.draw(dblock);
	for (auto& dblock : m_characterHands) target.draw(dblock);
}

void Character::onMousePress(const sf::Event& event)
{
	if (event.mouseButton.button == sf::Mouse::Right)
	{
		m_handStates[RCLOSED] = true;
	}
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		m_handStates[LCLOSED] = true;
	}
}

void Character::onMouseRelease(const sf::Event& event)
{
	if (event.mouseButton.button == sf::Mouse::Right)
	{
		m_handStates[RCLOSED] = false;
	}
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		m_handStates[LCLOSED] = false;
	}
}

void Character::onKeyPress(const sf::Event & event)
{
	if (event.key.code == sf::Keyboard::W)
	{
		m_handStates[UP] = true;
	}
	if (event.key.code == sf::Keyboard::A)
	{
		m_handStates[LEFT] = true;
	}
	if (event.key.code == sf::Keyboard::S)
	{
		m_handStates[DOWN] = true;
	}
	if (event.key.code == sf::Keyboard::D)
	{
		m_handStates[RIGHT] = true;
	}
}

void Character::onKeyRelease(const sf::Event & event)
{
	if (event.key.code == sf::Keyboard::W)
	{
		m_handStates[UP] = false;
	}
	if (event.key.code == sf::Keyboard::A)
	{
		m_handStates[LEFT] = false;
	}
	if (event.key.code == sf::Keyboard::S)
	{
		m_handStates[DOWN] = false;
	}
	if (event.key.code == sf::Keyboard::D)
	{
		m_handStates[RIGHT] = false;
	}
}

b2Vec2 Character::getPosition()
{
	return m_characterComponents[0].GetBody()->GetPosition();
}
