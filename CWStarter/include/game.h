#pragma once

/*!
\file game.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include <vector>

#include "SFMLDebugDraw.h"

#include "dynamicBlock.h"
#include "dynamicCircle.h"
#include "staticBlock.h"
#include "myContactListener.h"
#include "character.h"
#include "movingPlatform.h"
#include "collectable.h"
#include "camera.h"
#include "respawner.h"
#include "enemy.h"
#include "endgoal.h"
#include "textureManager.h"
/*! \class Game
\brief Holds all information about the game, blocks, balls etc and allows updating and rendering.
*/

class Game : public sf::Drawable {
private:
	sf::Vector2f m_worldSize = sf::Vector2f(16.f, 12.f); //!< Size of the physical worlds is 8 X 6 metres
	Camera m_Camera; //!< The view maps from physical co-ordinates to rendering co-ordinates
	Camera m_UICamera;

	MyContactListener myContactListenerInstance;//!< create custom contact listener

	b2World* m_pWorld = nullptr; //!< Pointer to the Box2D world.  Using pointers as BOX2D has it's own memory management
	const int mk_iVelIterations = 7; //!< On each update there will be 7 velocity iterations in the physics engine
	const int mk_iPosIterations = 5; //!< On each update there will be 5 position iterations on each update
	const b2Vec2 mk_gravity = b2Vec2(0.f, 9.81f); //!< Standard earth gravity will be used (negate the value ofcourse!)

	bool m_debug = false; //!< Toggle for debug drawing
	SFMLDebugDraw m_debugDraw; //!< Box2D debug drawing

	std::vector<DynamicCircle> m_dynamicCircles; //!< Moving circles
	std::vector<StaticBlock> m_staticBlocks; //!< Static blocks
	std::vector<DynamicBlock> m_dynamicBlocks; //!< Moving blocks
	std::vector<MovingPlatform> m_movingPlatforms; //!< Moving blocks
	std::vector<Collectable> m_collectables;//!< Collectables
	std::vector<Respawner> m_respawners;//!< Collectables
	std::vector<Enemy> m_enemies;//!< Collectables
	std::vector<Endgoal> m_endgoal;//!< Collectables

	TextureManager m_textureManager;

	Character* m_character;//!< The playable character

	int Score = 0;
	sf::Text m_text;
	sf::Font m_font;

public:
	
	Game(); //!< Constructor which sets up the game
	~Game(); //!< Destructor which cleans all the pointer memory up
	void LoadTextures();
	void createScene();
	void update(float timestep); //!< Update the game with give timestep
	void draw(sf::RenderTarget &target, sf::RenderStates states) const; //!< Draw the game to the render context
	void toggleDebug(); //!< Toggle for debug drawing
	Character getCharacter();

	void onMousePress(const sf::Event& event);
	void onMouseRelease(const sf::Event& event);
	void onKeyPress(const sf::Event& event);
	void onKeyRelease(const sf::Event& event);
};