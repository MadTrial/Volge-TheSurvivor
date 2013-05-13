#include "stdafx.h"
#include "PlayerCtrlComponent.hpp"
#include "../../MadEngine/Manager/Controller.hpp"

PlayerCtrlComponent::PlayerCtrlComponent()
	:m_Physics(nullptr), m_Transform(nullptr)
{
}

void PlayerCtrlComponent::initialise(Entity* owner)
{
	m_Physics=owner->component<PhysicsComponent>();
	m_Transform=owner->component<TransformableComponent>();
}

void PlayerCtrlComponent::update(float dt)
{
	if(!m_Physics || !m_Transform)
		return;

	auto ctrl=Mad::Manager::Controller::getSingleton();

	b2Vec2 fwd;
	fwd.x=std::cos(m_Transform->rotation());
	fwd.y=std::sin(m_Transform->rotation());

	b2Vec2 dir(0, 0);
	float angle=m_Transform->rotation();

	if(ctrl->getControl("forward"))
		dir+=b2Vec2(0, -1);
	if(ctrl->getControl("backward"))
		dir+=b2Vec2(0, 1);

	//SFML have inverted y axis, the same applies with angle
	if(ctrl->getControl("left"))
		angle-=10.0f*DEGTORAD;
	if(ctrl->getControl("right"))
		angle+=10.0f*DEGTORAD;

	m_Physics->setSpeed(1.5f);
	m_Physics->setMovementDirection(dir);
	m_Physics->setAngle(angle);
}