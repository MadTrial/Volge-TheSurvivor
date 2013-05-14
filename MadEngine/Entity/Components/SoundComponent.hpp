#pragma once

#include "../../stdafx.h"
#include "../Entity.hpp"

class SoundComponent : public IComponent
{
public:
	SoundComponent();
	~SoundComponent();

	std::string getTypeName() const;
	std::list<Property> listProperties() const;

	void addSound(const std::string& eventName, const std::string& soundId,bool repeatPlay = false);
	void initialise(Entity* owner);
	void update(float deltaTime);
	void onGenericEvent(const std::string& name);

private:
	std::map<std::string,sf::Sound*> m_SFXList;
	std::string currentEvent;
};