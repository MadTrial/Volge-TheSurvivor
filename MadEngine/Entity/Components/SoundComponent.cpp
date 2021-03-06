#include "SoundComponent.hpp"
#include "../../Sound/SoundBuffer.hpp"
#include "../../Manager/Resource.hpp"

SoundComponent::SoundComponent()
{
}

SoundComponent::~SoundComponent()
{
	for (auto it = m_SFXList.begin(); it != m_SFXList.end();)
	{
		delete it->second;
		it = m_SFXList.erase(it);
	}
}

std::string SoundComponent::getTypeName() const
{
	return "Sound";
}

std::list<Property> SoundComponent::listProperties() const
{
	// TODO
	return std::list<Property>();
}

void SoundComponent::addSound(const std::string& eventName, const std::string& soundId, bool repeatPlay)
{
	Mad::Sound::SoundBuffer* sb	= Mad::Manager::Resource::get<Mad::Sound::SoundBuffer>(soundId);
	sf::Sound* sound			= new sf::Sound();
	sound->setBuffer(sb->getBuffer());
	sound->setLoop(repeatPlay);
	m_SFXList[eventName]	= sound;
}

void SoundComponent::initialise(Entity* owner)
{
}

void SoundComponent::update(float deltaTime)
{
}

void SoundComponent::onGenericEvent(const std::string& name)
{
	if (m_SFXList.count(name) > 0)
		m_SFXList[name]->play();
}


IComponent* SoundComponent::factoryFunction(rapidxml::xml_node<>* comp_data)
{
	SoundComponent* sc = new SoundComponent();
	for (comp_data; comp_data; comp_data = comp_data->next_sibling())
	{
		sc->addSound(comp_data->first_attribute("name")->value()
					, comp_data->first_attribute("value")->value()
					, (comp_data->first_attribute("repeat")->value() == "true"));
	}
	return sc;
}