#include "IGameObject.h"

IGameObject::IGameObject(const EObjectType objectType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: m_pOwner(nullptr)
	, m_objectType(objectType)
	, m_position(position)
	, m_targetPosition(position)
	, m_size(size)
	, m_rotation(rotation)
	, m_layer(layer)
	, m_direction(glm::vec2(0, 1.f))
	, m_velocity(0)
{
}

IGameObject::~IGameObject()
{
}

void IGameObject::destroy()
{
	m_objectType = EObjectType::Unknown;
}

void IGameObject::explode()
{
}

void IGameObject::setOwner(IGameObject* pOwner)
{
	m_pOwner = pOwner;
}

void IGameObject::setVelocity(const double velocity)
{
	m_velocity = velocity;
}