#include "stdafx.h"
#include "Game.hpp"
#include <iostream>

namespace GSTATE
{
	enum
	{
		Uninitialized, Splash, Menu, Play, Pause, Resume, Exit
	};
}

namespace
{
	class DamageListenerComponent: public IComponent
	{
	public:
		std::string getTypeName() const
		{
			return "Damage listener";
		}

		std::list<Property> listProperties() const
		{
			return std::list<Property>();
		}

		void initialise(Entity* owner)
		{
		}

		void update(float dt)
		{
		}

		void onDamage(Entity* other, int damage)
		{
			std::cout << "Hit by " << other << ", lost " << damage
					<< " health points!\n";
		}
	};
}

void Game::initialize()
{
	Mad::Utility::ConfigParser cfg("setting.cfg");
	std::string width, height, depth, title;

	cfg.getValue("width", width);
	cfg.getValue("height", height);
	cfg.getValue("depth", depth);
	cfg.getValue("title", title);
	Graphics->createWindow(title, std::stoi(width), std::stoi(height));

	setDisplayStatistics(true);
	time = 0;

	GameState->changeState(GSTATE::Splash);
	Controller = Mad::Manager::Controller::getSingleton();

	m_PhysicsManager = nullptr;
}

void Game::update()
{
	switch (GameState->getCurrentState())
	{
	case GSTATE::Splash:
	{
		splash();
	}
		break;
	case GSTATE::Menu:
	{
		menu();
	}
		break;
	case GSTATE::Play:
	{
		play();
	}
		break;
	}
}

void Game::splash()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		GameState->changeState(GSTATE::Menu);

	Graphics->beginRendering();
	m_Splash.draw(0);
	Graphics->endRendering();
}

void Game::menu()
{
	GameState->changeState(GSTATE::Play);
}

void Game::load()
{
	switch (GameState->getCurrentState())
	{
	case GSTATE::Splash:
	{
		ResourceProvider->load<Mad::Graphics::Texture>("volge.png");
		ResourceProvider->load<Mad::Graphics::Texture>("soldier.png");
		ResourceProvider->load<Mad::Graphics::SpriteData>("volge.json");
		ResourceProvider->load<Mad::Graphics::SpriteData>("soldier.json");

		sf::Vector2u ws = Graphics->getRenderWindow().getSize();
		m_Splash.setSource("volge.json");
		m_Splash.setPosition(b2Vec2(ws.x / 2.f, ws.y / 2.f));
		time += clock.getElapsedTime().asSeconds();
	}
		break;
	case GSTATE::Menu:
	{
	}
		break;
	case GSTATE::Play:
	{
		m_PhysicsManager = PhysicsSystem::getSingleton();
		m_PhysicsManager->enableDebugDraw(Graphics->getRenderWindow());
		m_EntityManager = new EntityManager(m_PhysicsManager);

		ResourceProvider->load<Mad::Graphics::Texture>("soldier.png");
		ResourceProvider->load<Mad::Graphics::Texture>("zombie.png");
		ResourceProvider->load<Mad::Sound::SoundBuffer>("gun_m16_reload.ogg");
		ResourceProvider->load<Mad::Sound::SoundBuffer>("gun_cock.ogg");
		ResourceProvider->load<Mad::Sound::SoundBuffer>("gun_shoot.ogg");
		ResourceProvider->load<Mad::Sound::SoundBuffer>("gun_noammo.ogg");
		ResourceProvider->load<Mad::Sound::SoundBuffer>("footsteps.ogg");

		Controller->setKeyboardControl("forward", sf::Keyboard::Up);
		Controller->setKeyboardControl("backward", sf::Keyboard::Down);
		Controller->setKeyboardControl("exit", sf::Keyboard::Escape);
		Controller->setKeyboardControl("left", sf::Keyboard::Left);
		Controller->setKeyboardControl("right", sf::Keyboard::Right);
		Controller->setKeyboardControl("attack", sf::Keyboard::LControl);
		Controller->setKeyboardControl("reload", sf::Keyboard::LShift);
		Controller->setKeyboardControl("throw", sf::Keyboard::G);

		m_Player = m_EntityManager->createEntity("Player");
		m_Player->loadComponent("Assets/Object/Soldier.xml");

		/*
		m_Player->addComponent(new PlayerCtrlComponent());

		TransformableComponent* tr = new TransformableComponent();
		tr->setPosition(b2Vec2(250, 250));
		m_Player->addComponent(tr);

		SpriteComponent* sp = new SpriteComponent();
		sp->setSprite("soldier.json");
		sp->setSize(b2Vec2(2, 2));
		m_Player->addComponent(sp);

		CameraComponent* cam = new CameraComponent();
		cam->setVirtualSize(b2Vec2(10, 10));
		cam->makeActive();
		m_Player->addComponent(cam);

		WeaponComponent* wp = new WeaponComponent(m_PhysicsManager);
		Item::Gun* gun = new Item::Gun("M16", 20, 30, 30, 0.5f, 0.1f);
		wp->setGun(gun);
		m_Player->addComponent(wp);

		PhysicsDef phDef;
		phDef.shape = PhysicsShape::Circle;
		phDef.friction = 0.5f;
		phDef.mass = 80.0f;
		phDef.circle.radius = 0.5f;

		PhysicsComponent* ph = new PhysicsComponent(m_PhysicsManager, phDef,
				0xffffffff);
		m_Player->addComponent(ph);

		SoundComponent* sc = new SoundComponent();
		sc->addSound("Idle", "gun_noammo.ogg", true);
		m_Player->addComponent(sc);

		//m_Player->initialise();
		m_EntityManager->createEntity(m_Player);
		m_Player->onGenericEvent("Idle");
		*/
		//m_EntityManager->loadEntity("Assets/Object/Soldier.xml");

		m_Obstacle = new Entity();

		DamageListenerComponent* dmgList = new DamageListenerComponent();
		m_Obstacle->addComponent(dmgList);

		TransformableComponent* tr = new TransformableComponent();
		tr->setPosition(b2Vec2(250, 270));
		m_Obstacle->addComponent(tr);

		PhysicsDef phDef;
		phDef.shape = PhysicsShape::Box;
		phDef.friction = 0.5f;
		phDef.mass = 0;
		phDef.box.size.Set(2, 2);

		PhysicsComponent* ph = new PhysicsComponent(m_PhysicsManager, phDef, ~0);
		m_Obstacle->addComponent(ph);

		m_Obstacle->initialise();
	}
		break;
	}
}

void Game::unLoad()
{
	switch (GameState->getCurrentState() - 1)
	//Unload object from previous state
	{
	case GSTATE::Splash:
	{
	}
		break;

	case GSTATE::Menu:
	{
	}
		break;

	case GSTATE::Play:
	{
		delete m_Player;
		m_PhysicsManager = nullptr;
	}
		break;
	}
}

void Game::play()
{
	float deltaTime = clock.getElapsedTime().asSeconds();

	m_PhysicsManager->update(deltaTime);

	Graphics->beginGameRendering();
	m_PhysicsManager->drawDebugData();
//	m_Player->update(deltaTime);
//	m_Obstacle->update(deltaTime);
	m_EntityManager->update(deltaTime);
	Graphics->endGameRendering();

	Graphics->beginRendering();
	Graphics->getDrawBatch().drawText("Test Text");
	Graphics->getDrawBatch().drawText("another test text",
			sf::Vector2f(100, 100), 32, 15, sf::Color::Red,
			sf::Text::Underlined);
	Graphics->endRendering();

	if (Controller->getControl("exit"))
		quit();

	clock.restart();
}
