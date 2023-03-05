#include "GameObject.h"
#include "ResourceManager.h"

dae::GameObject::~GameObject()
{
	for(auto& child : m_pChildren)
	{
		delete child;
		child = nullptr;
	}
};

void dae::GameObject::Update(float dt)
{
	UpdateWorldPosition();
	for(auto& component :m_pComponents)
	{
		component->Update(dt);
		component->SetPosition(m_worldPos.x, m_worldPos.y, m_worldPos.z);
	}

	for(auto& child : m_pChildren)
	{
		if(m_positionIsDirty)
		{
			child->SetPositionDirty();
		}
		child->Update(dt);
	}
}

void dae::GameObject::FixedUpdate([[maybe_unused]] float dt) const
{
	
}

void dae::GameObject::Render() const
{
	for(const auto& component : m_pComponents)
	{
		component->Render();
	}

	for (auto& child : m_pChildren)
	{
		child->Render();
	}
}

void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	
	if (pParent == nullptr)
	{
		SetLocalPosition(GetWorldPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			SetLocalPosition(GetLocalPosition() - pParent->GetWorldPosition());
		}
		SetPositionDirty();
	}
	if (m_pParent)
	{
		m_pParent->RemoveChild(this);
	}
	m_pParent = pParent;
	if (m_pParent)
	{
		m_pParent->AddChild(this);
	}
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_localPos = pos;
	SetPositionDirty();
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_positionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_worldPos;
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_positionIsDirty)
	{
		if (m_pParent == nullptr)
			m_worldPos = m_localPos;
		else
			m_worldPos = m_pParent->GetWorldPosition() + m_localPos;
	}
	m_positionIsDirty = false;
}

void dae::GameObject::AddChild(GameObject* child)
{
	m_pChildren.push_back(child);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	const auto childToDelete = std::find(m_pChildren.begin(), m_pChildren.end(), child);
	if(childToDelete != m_pChildren.end())
	{
		m_pChildren.erase(childToDelete);	
	}
}