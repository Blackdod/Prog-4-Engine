#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject()
{
	for (auto component : m_pComponents)
	{
		delete component;
		component = nullptr;
	}
};

void dae::GameObject::Update([[maybe_unused]] float dt) {}

void dae::GameObject::Render() const
{
	if (IsComponentPresent(dae::Component::ComponentType::transform) == true)
	{
		Transform* pTransform = GetTransform();
		const auto& pos = pTransform->GetPosition();

		if (m_texture != nullptr)
		{
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		}
	}
	else
	{
		const glm::vec3& pos{ 0,0,0 };
		if (m_texture != nullptr)
		{
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		}
	}
	
}

void dae::GameObject::AddComponent(Component* component)
{
	m_pComponents.push_back(component);
}

dae::Component* dae::GameObject::GetComponent() const
{
		auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
			return (i->GetType() == Component::ComponentType::none);
			});

		if (result == m_pComponents.end())
		{
			return nullptr; //early out if no such component
		}

		return *result;
}

dae::Transform* dae::GameObject::GetTransform() const
{
	auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
		return (i->GetType() == Component::ComponentType::transform);
		});

	if (result == m_pComponents.end())
	{
		return nullptr; //early out if no such component
	}

	return static_cast<Transform*>(*result);
}

bool dae::GameObject::IsComponentPresent(dae::Component::ComponentType type) const
{
	bool isFound{ false };
	switch (type)
	{
	case dae::Component::ComponentType::transform:
		{
			auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
				return (i->GetType() == Component::ComponentType::transform);
				});

			if (result != m_pComponents.end())
			{
				isFound = true;
			}
		}
	}

	return isFound;
}

void dae::GameObject::SetPosition(float x, float y, float z)
{
	auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
		return (i->GetType() == Component::ComponentType::transform);
		});

	if (result != m_pComponents.end())
	{
		static_cast<Transform*>(*result)->SetPosition(x,y,z);
	}
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}