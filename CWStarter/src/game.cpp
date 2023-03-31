/*!
\file game.cpp
*/
#include "game.h"

Game::Game()
{
	m_pWorld = new b2World(mk_gravity);
	m_Camera = Camera(sf::Vector2f(m_worldSize.x-4,m_worldSize.y-3));
	m_UICamera = Camera(sf::Vector2f(1024, 800));
	m_UICamera.setView(512,400);

	m_debugDraw.setWorld(m_pWorld);

	m_pWorld->SetContactListener(&myContactListenerInstance);
	LoadTextures();
	createScene();

	m_character = new Character(m_pWorld,0,-2,m_textureManager);
	
	if (!m_font.loadFromFile("./assets/fonts/arial.ttf"))
	{
		// Error...
	}
	m_text.setFont(m_font);
	m_text.setString(std::to_string(Score));
	m_text.setPosition(100, 100);
	m_text.setCharacterSize(24);
}

Game::~Game()
{
	delete m_pWorld;
	m_pWorld = nullptr;
}

void Game::LoadTextures()
{

	m_textureManager.addTexture("grass", "assets/textures/grass.png");
	m_textureManager.addTexture("left_hand", "assets/textures/left_hand.png");
	m_textureManager.addTexture("right_hand", "assets/textures/right_hand.png");
	m_textureManager.addTexture("playerface", "assets/textures/playerface.png");
	m_textureManager.addTexture("arm", "assets/textures/arm.png");
}

void Game::createScene()
{
		//!< Circles
	//m_dynamicCircles.resize(2);
		//!< World, Position, Radius, Orientaion, Type
	//m_dynamicCircles[0] = DynamicCircle(m_pWorld, sf::Vector2f(0.f, -3.5f), 0.1f, 0.0f, PhysicalThing::CollisionFilter::OBJECT);
	//m_dynamicCircles[1] = DynamicCircle(m_pWorld, sf::Vector2f(0.5f, -2.0f), 0.1f, 0.0f, PhysicalThing::CollisionFilter::OBJECT);

		//!<Static blocks (pretty much always terrain)
	m_staticBlocks.resize(5);
		//!< World, Position, Size, Orientaion, Type
	m_staticBlocks[0] = StaticBlock(m_pWorld, sf::Vector2f(0.f, 0.f), sf::Vector2f(6.0f, 0.25f), 0.f, PhysicalThing::CollisionFilter::TERRAIN,m_textureManager.getTexture("grass"));//floor, staticblocks[0] always has to be floor as it is anchor for moving platforms
	m_staticBlocks[1] = StaticBlock(m_pWorld, sf::Vector2f(6.f, 0.f), sf::Vector2f(2.0f, 0.25f), 0.f, PhysicalThing::CollisionFilter::TERRAIN, m_textureManager.getTexture("grass"));
	m_staticBlocks[2] = StaticBlock(m_pWorld, sf::Vector2f(14.f, -14.f), sf::Vector2f(2.0f, 2.f), 0.f, PhysicalThing::CollisionFilter::TERRAIN, m_textureManager.getTexture("grass"));
	m_staticBlocks[3] = StaticBlock(m_pWorld, sf::Vector2f(18.f, -13.f), sf::Vector2f(2.0f, 2.f), 0.f, PhysicalThing::CollisionFilter::TERRAIN, m_textureManager.getTexture("grass"));
	m_staticBlocks[4] = StaticBlock(m_pWorld, sf::Vector2f(22.f, -15.f), sf::Vector2f(2.0f, 2.f), 0.f, PhysicalThing::CollisionFilter::TERRAIN, m_textureManager.getTexture("grass"));

	//m_dynamicBlocks.resize(1);
		//!< World, Position, Size, Orientaion, Type
	//m_dynamicBlocks[0] = DynamicBlock(m_pWorld, sf::Vector2f(3.f, 1.f), sf::Vector2f(0.25f, 0.25f), 0.f, PhysicalThing::CollisionFilter::OBJECT);

	m_collectables.resize(3);
	m_collectables[0] = Collectable(m_pWorld, sf::Vector2f(16.f, -5.0f), 0.25f, 0.f, PhysicalThing::CollisionFilter::COLLECTABLE);
	m_collectables[1] = Collectable(m_pWorld, sf::Vector2f(7.f, -0.5f), 0.25f, 0.f, PhysicalThing::CollisionFilter::COLLECTABLE);
	m_collectables[2] = Collectable(m_pWorld, sf::Vector2f(0.f, -5.0f), 0.25f, 0.f, PhysicalThing::CollisionFilter::COLLECTABLE);
	//the further away the collectable, the earlier it needs to be in the vector (reason unknown)
	


	for (int i = 0; i < m_collectables.size(); i++)
	{
		m_collectables[i].setUserData(&m_collectables[i]);
	}

	m_respawners.resize(1);
	m_respawners[0] = Respawner(m_pWorld, sf::Vector2f(0.f, 8.f), sf::Vector2f(70.0f, 0.25f), 0.f, PhysicalThing::CollisionFilter::RESPAWNER);

	for (int i = 0; i < m_respawners.size(); i++)
	{
		m_respawners[i].setUserData(&m_respawners[i]);
	}

	m_endgoal.resize(1);
	m_endgoal[0] = Endgoal(m_pWorld, sf::Vector2f(22.1f, -17.f), sf::Vector2f(2.0f, 2.f), 0.f, PhysicalThing::CollisionFilter::GOAL);

	for (int i = 0; i < m_endgoal.size(); i++)
	{
		m_endgoal[i].setUserData(&m_endgoal[i]);
	}
#pragma region Enemies

	EnemyAnimation l_enemyAnimation_1;
	b2Vec2 temp = b2Vec2(4.25f, -0.5f);
	l_enemyAnimation_1.addKeyframe(0.0f, temp, 0.0f);
	temp = b2Vec2(8.f, -0.5f);
	l_enemyAnimation_1.addKeyframe(3.0f, temp, 0.0f);
	temp = b2Vec2(8.f, 0.5f);
	l_enemyAnimation_1.addKeyframe(2.0f, temp, 0.0f);
	temp = b2Vec2(4.25f, 0.5f);
	l_enemyAnimation_1.addKeyframe(3.0f, temp, 0.0f);
	temp = b2Vec2(4.25f, -0.5f);
	l_enemyAnimation_1.addKeyframe(2.0f, temp, 0.0f);


	m_enemies.resize(1);
	m_enemies[0] = Enemy(m_pWorld, sf::Vector2f(4.25f, -0.5f), sf::Vector2f(0.5f, 0.5f), 0.f, m_staticBlocks[0].GetBody(), l_enemyAnimation_1);

#pragma endregion

#pragma region Moving Platforms

	m_movingPlatforms.resize(3);

	MovingPlatformAnimation l_platformAnimation_1;
	//!< position in the world
	temp = b2Vec2(6.f, 4.f);
	l_platformAnimation_1.addKeyframe(0.0f, temp, 0.0f);
	temp = b2Vec2(14.f, 4.f);
	l_platformAnimation_1.addKeyframe(4.0f, temp, 0.0f);
	temp = b2Vec2(6.f, 4.f);
	l_platformAnimation_1.addKeyframe(4.0f, temp, 0.0f);

	
	//!< World, Position, Size, Orientaion, Type, Anchor, Animation
	m_movingPlatforms[0] = MovingPlatform(m_pWorld, sf::Vector2f(6.f, 4.0f),sf::Vector2f(4.0f,0.25f),0.f, PhysicalThing::CollisionFilter::TERRAIN, m_staticBlocks[0].GetBody(), l_platformAnimation_1);
	
	
	MovingPlatformAnimation l_platformAnimation_2;
	//!< position in the world
	temp = b2Vec2(14.f, -12.f);
	l_platformAnimation_2.addKeyframe(0.0f, temp, 0.f);
	temp = b2Vec2(14.f, 4.f);
	l_platformAnimation_2.addKeyframe(4.0f, temp, 0.f);
	temp = b2Vec2(14.f, -12.f);
	l_platformAnimation_2.addKeyframe(4.0f, temp, 0.f);

	//!< World, Position, Size, Orientaion, Type, Anchor, Animation
	m_movingPlatforms[1] = MovingPlatform(m_pWorld, sf::Vector2f(14.f, -12.0f), sf::Vector2f(.25f, 8.f), 0.f, PhysicalThing::CollisionFilter::TERRAIN, m_staticBlocks[0].GetBody(), l_platformAnimation_2);



	MovingPlatformAnimation l_platformAnimation_3;
	//!< position in the world
	temp = b2Vec2(0.f, -4.f);
	l_platformAnimation_3.addKeyframe(0.0f, temp, 0.0f);

	//1 timestep angle of 6 for 360 every second
	//l_platformAnimation_2.addKeyframe(1.0f, temp, 6.0f);
	l_platformAnimation_3.addKeyframe(4.0f, temp, 9.f);

	l_platformAnimation_3.addKeyframe(4.0f, temp, 0.f);

	//!< World, Position, Size, Orientaion, Type, Anchor, Animation
	m_movingPlatforms[2] = MovingPlatform(m_pWorld, sf::Vector2f(0.f, -4.0f), sf::Vector2f(4.0f, 0.25f), 0.f, PhysicalThing::CollisionFilter::TERRAIN, m_staticBlocks[0].GetBody(), l_platformAnimation_3);


#pragma endregion
}

void Game::update(float timestep)
{
	m_Camera.setView(m_character->getPosition().x , m_character->getPosition().y);
	//!< Update the world
	m_pWorld->Step(timestep, mk_iVelIterations, mk_iVelIterations);

	//!< Update all game objects - updates rendering positions
	for (auto& circle : m_dynamicCircles) circle.update();

	//!< Update all dynamic blocks
	for (auto& dblock : m_dynamicBlocks) dblock.update();
	
	for (auto& mplat : m_movingPlatforms) mplat.update(timestep);

	for (auto& enemy : m_enemies) enemy.update(timestep);

	for (auto it = m_collectables.begin(); it != m_collectables.end(); ++it)
	{
		if (it->registeredForDeletion())
		{
			m_pWorld->DestroyBody(it->GetBody());	
			m_collectables.erase(it);
			
			Score += 100;

			m_text.setString(std::to_string(Score));
			break;
		}
	}
	m_character->update();

	// Delete debug shapes
	if (m_debug) m_debugDraw.clear();

	//!<handles respawning the character
	if (m_character->getRespawn())
	{
		m_character->setRespawn(false);
		m_character = new Character(m_pWorld, 0, -2, m_textureManager);//!<when character is replaced it handles clearing pointers in the destructor
	}

	if (m_character->getEnd())
	{
		std::string temp = "You Win!\n" + std::to_string(Score);
		m_text.setString(temp);
		m_text.setScale(3, 3);
	}
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//!< Set the view
	target.setView(m_Camera.getView());

	//!< Draw all game objects
	for (auto circle : m_dynamicCircles) target.draw(circle);

	for (auto block : m_staticBlocks) target.draw(block);

	for (auto dblock : m_dynamicBlocks) target.draw(dblock);

	for (auto mplat : m_movingPlatforms) target.draw(mplat);

	for (auto enemy : m_enemies) target.draw(enemy);

	for (auto goal : m_endgoal) target.draw(goal);

	for (auto collectable : m_collectables) target.draw(collectable);

	for (auto respawner : m_respawners) target.draw(respawner);
	
	target.draw(*m_character);


	//!< Debug Draw
	if (m_debug) target.draw(m_debugDraw);

	//!<UI
	target.setView(m_UICamera.getView());
	target.draw(m_text);


	//!< Set the view back to the game view
	target.setView(m_Camera.getView());
}

void Game::toggleDebug()
{
	m_debug = !m_debug;
}

Character Game::getCharacter()
{
	return *m_character;
}

void Game::onMousePress(const sf::Event& event)
{
	m_character->onMousePress(event);

}

void Game::onMouseRelease(const sf::Event& event)
{
	m_character->onMouseRelease(event);
}

void Game::onKeyPress(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Tab)
	{
		toggleDebug();
	}
	m_character->onKeyPress(event);
}

void Game::onKeyRelease(const sf::Event& event)
{
	m_character->onKeyRelease(event);
}
