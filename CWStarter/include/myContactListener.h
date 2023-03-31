#pragma once

/*!
\file MycontactListener.h
*/
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "hand.h"
#include "staticBlock.h"
#include "physicalThing.h"
#include "collectable.h"
#include "character.h"

class MyContactListener : public b2ContactListener
{

	void BeginContact(b2Contact* contact) {
		
		b2Vec2 contactPoint;
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);

		if (contact->GetManifold()->pointCount == 2)
		{
			contactPoint = (worldManifold.points[0] + worldManifold.points[1]);
			contactPoint.x /= 2.0f;
			contactPoint.y /= 2.0f;
		}
		else
		{
			contactPoint = worldManifold.points[0];
		}
		
		
			int l_filterData = contact->GetFixtureA()->GetFilterData().categoryBits;
			//!<Check if contact fixtures filter data is a HAND for fixture A
			if (l_filterData == PhysicalThing::HAND)
			{
				if (contact->GetFixtureB()->GetFilterData().categoryBits == PhysicalThing::COLLECTABLE)
				{
					static_cast<Collectable*>(contact->GetFixtureB()->GetBody()->GetUserData())->registerForDeletion();
				}
				static_cast<Hand*>(contact->GetFixtureA()->GetBody()->GetUserData())->startContact(
					contact->GetFixtureB()->GetBody(), contactPoint);
			}
			l_filterData = contact->GetFixtureB()->GetFilterData().categoryBits;
			//!<Check if contact fixtures filter data is a HAND for fixture B
			if (l_filterData == PhysicalThing::HAND)
			{
				if (contact->GetFixtureA()->GetFilterData().categoryBits == PhysicalThing::COLLECTABLE)
				{
					static_cast<Collectable*>(contact->GetFixtureA()->GetBody()->GetUserData())->registerForDeletion();
				}

				static_cast<Hand*>(contact->GetFixtureB()->GetBody()->GetUserData())->startContact(
					contact->GetFixtureA()->GetBody(), contactPoint);
			}

			//Respawning character
			l_filterData = contact->GetFixtureA()->GetFilterData().categoryBits;
			if (l_filterData == PhysicalThing::RESPAWNER)
			{
				static_cast<DynamicBlock*>(contact->GetFixtureB()->GetBody()->GetUserData())->registerForRespawn();
			}
			l_filterData = contact->GetFixtureB()->GetFilterData().categoryBits;
			if (l_filterData == PhysicalThing::RESPAWNER)
			{
				static_cast<DynamicBlock*>(contact->GetFixtureA()->GetBody()->GetUserData())->registerForRespawn();
			}

			//Endgoal
			l_filterData = contact->GetFixtureA()->GetFilterData().categoryBits;
			if (l_filterData == PhysicalThing::GOAL)
			{
				static_cast<DynamicBlock*>(contact->GetFixtureB()->GetBody()->GetUserData())->registerForEnd();
			}
			l_filterData = contact->GetFixtureB()->GetFilterData().categoryBits;
			if (l_filterData == PhysicalThing::GOAL)
			{
				static_cast<DynamicBlock*>(contact->GetFixtureA()->GetBody()->GetUserData())->registerForEnd();
			}

			//collide with enemy, respawn
			l_filterData = contact->GetFixtureA()->GetFilterData().categoryBits;
			if (l_filterData == PhysicalThing::ENEMY)
			{
				static_cast<DynamicBlock*>(contact->GetFixtureB()->GetBody()->GetUserData())->registerForRespawn();
			}
			l_filterData = contact->GetFixtureB()->GetFilterData().categoryBits;
			if (l_filterData == PhysicalThing::ENEMY)
			{
				static_cast<DynamicBlock*>(contact->GetFixtureA()->GetBody()->GetUserData())->registerForRespawn();
			}
	}

	void EndContact(b2Contact* contact) {

		//!< does the same as begin contact, simply calls endContact instead
		int l_filterData = contact->GetFixtureA()->GetFilterData().categoryBits;
		if (l_filterData == PhysicalThing::HAND)
			static_cast<Hand*>(contact->GetFixtureA()->GetBody()->GetUserData())->endContact(contact->GetFixtureB()->GetBody());

		
		l_filterData = contact->GetFixtureB()->GetFilterData().categoryBits;
		if (l_filterData == PhysicalThing::HAND)
			static_cast<Hand*>(contact->GetFixtureB()->GetBody()->GetUserData())->endContact(contact->GetFixtureA()->GetBody());

	}
};