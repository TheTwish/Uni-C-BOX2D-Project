#include "movingPlatform.h"

void MovingPlatformAnimation::addKeyframe(float timestep, b2Vec2 linearOffset, float angularOffset)
{
	PlatformKeyframe kf;
	if (m_keyframes.empty())
	{
		kf.m_time = timestep;
	}
	else
	{
		kf.m_time = timestep + m_keyframes.back().m_time;
	}
	
	kf.m_linearOffset = linearOffset;
	kf.m_angularOffset = angularOffset;
	m_keyframes.push_back(kf);
	m_animationLength += timestep;
}

void MovingPlatformAnimation::play(float timestep, b2Vec2 & linearOffset, float & angularOffset)
{
	m_currentTime += timestep;
	if (m_currentTime > m_animationLength)
	{
		m_currentTime -= m_animationLength;
	}
	for (size_t i = 0; i < m_keyframes.size() - 1; i++)
	{
		if (m_keyframes.at(i).m_time < m_currentTime && m_currentTime < m_keyframes.at(i+1).m_time)
		{

			//v(tn) = m_keyframes.at(i+1)
			//
			//lerp linearoffset x y and angular offset
			// t =  (time - startTime) / (endTime - startTime)
			//lerping = start + ((end - start) * t);
			//lerp(m_keyframes.at(i).m_linearOffset, m_keyframes(i+1).m_linearOffset, t);
			float t = (m_currentTime - m_keyframes.at(i).m_time) / (m_keyframes.at(i + 1).m_time - m_keyframes.at(i).m_time);

			linearOffset.x = m_keyframes.at(i).m_linearOffset.x + ((m_keyframes.at(i + 1).m_linearOffset.x - m_keyframes.at(i).m_linearOffset.x) * t);
			linearOffset.y = m_keyframes.at(i).m_linearOffset.y + ((m_keyframes.at(i + 1).m_linearOffset.y - m_keyframes.at(i).m_linearOffset.y) * t);
			angularOffset = m_keyframes.at(i).m_angularOffset + ((m_keyframes.at(i + 1).m_angularOffset - m_keyframes.at(i).m_angularOffset) * t);	
		}
	}
}

MovingPlatform::MovingPlatform(b2World* world, const sf::Vector2f& position, const sf::Vector2f& size, const float orientation, PhysicalThing::CollisionFilter type, b2Body* anchor, MovingPlatformAnimation myAnimation)
{
	m_platform = KinematicBlock(world,position,size,orientation,type);
	//ensure platform does not bounce
	m_platform.setRestitution(0.f);

	b2WeldJoint* l_weld = nullptr;
	//Create weld joint between platform and anchor(the floor)
	b2WeldJointDef l_def;
	l_def.bodyA = anchor;//!< define body A
	l_def.bodyB = m_platform.GetBody();//!< define body B

	l_def.localAnchorA = m_platform.GetBody()->GetPosition();//!< define local anchor A, which is body B's position
	l_def.localAnchorB = anchor->GetPosition();//!< define local anchor B, which is body A's position
	l_weld = static_cast<b2WeldJoint*>(world->CreateJoint(static_cast<b2JointDef*>(&l_def)));
	world->CreateJoint(&l_def);
	/* How to destroy if needed
			m_world->DestroyJoint(m_weld);

			//delete m_weld;

			m_weld = nullptr;
			m_parent->onWeldDestroy();
			m_holding = false;
	*/
	m_animation = myAnimation;
}

void MovingPlatform::update(float timestep)
{
	b2Vec2 temppos = m_platform.GetBody()->GetPosition();
	float tempang = m_platform.GetBody()->GetAngle();

	m_animation.play(timestep, temppos, tempang);
	//m_animation.play(timestep, m_platform.GetBody(), m_platform.GetBody()->GetAngle());
	b2Vec2 velocity = temppos - m_platform.GetBody()->GetPosition();
	//std::cout << velocity.x << std::endl;
	m_platform.GetBody()->SetLinearVelocity(velocity);

	float angularvelocity = tempang - m_platform.GetBody()->GetAngle();
	m_platform.GetBody()->SetAngularVelocity(angularvelocity);
	
	//std::cout << angularvelocity << std::endl;
	//m_platform.GetBody()->SetTransform(temppos, tempang);

	m_platform.update();
}

void MovingPlatform::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
	target.draw(m_platform);
}

