#pragma once

/*!
\file physicalThing.h
*/

#include <Box2D/Box2D.h>
#include <iostream>

#define DEG2RAD 0.017453f
#define RAD2DEG 57.29577f

/*!
\class PhysicalThing
\brief A class that is the basis for every "thing" that will appear in the game, includes an enumerator that handles collision filtering
*/
class PhysicalThing {
protected:
	b2Body * m_body = nullptr; //!< Box2D body, no need to delete this as BOX2D will look after it
	float mk_fDensity = 1.0f; //!< Density of the material - used to give mass
	float mk_fFriction = 1.f; //!< Friction - I guess this is only friction from movement, not when static
	float mk_fRestitution = 0.f; //!< Restitution - bouncyness of stuff
public:
	enum CollisionFilter {TERRAIN = 1,CHARACTER = 2,ARM = 4,HAND = 8,OBJECT = 16, COLLECTABLE = 32,RESPAWNER = 64, ENEMY = 128,GOAL = 256}; //!< Enums for collision filtering - hex values converted to decimal for the enum
	void setUserData(void* data) { m_body->SetUserData(data); }; //!< Set the userdata of the body
	b2Body* GetBody() { return m_body; };
	
};
