#include "stdafx.h"
#include "CHealth.hpp"

CEntity::CHealth::CHealth(PhysicsSystem* physicsIntance, int Health)
	:CEntity::ICEntity(physicsIntance), m_Health(Health),m_MaxHealth(Health)
{}

CEntity::CHealth::~CHealth()
{}

void CEntity::CHealth::increaseHealthPoints(int points)
{
	m_Health	+= points;
	if (m_Health > m_MaxHealth)
		m_Health	= m_MaxHealth;
}

void CEntity::CHealth::decreaseHealthPoints(int points)
{
	m_Health	-= points;
	if (m_Health < m_MaxHealth)
		m_Health	= 0;
}

int CEntity::CHealth::getHealthPoints()
{
	return m_Health;
}

void CEntity::CHealth::update(float deltaTime)
{
}

void CEntity::CHealth::onCollisionBegin(Entity::IEntity* other)
{
}

void CEntity::CHealth::onCollisionEnd(Entity::IEntity* other)
{
}