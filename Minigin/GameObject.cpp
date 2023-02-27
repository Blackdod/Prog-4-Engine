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

void dae::GameObject::Update([[maybe_unused]] float dt) const
{
	FPSComponent* pFPS = GetFPSComponent();
	TextComponent* pText = GetText();

	if (pFPS != nullptr)
	{
		pFPS->Update(dt);

		if (pText != nullptr)
		{
			const std::string printString = std::to_string(pFPS->GetNrOfFrames()) + " FPS";
			pText->SetText(printString);
		}
	}

	if (pText != nullptr)
	{
		pText->Update();
	}
}

void dae::GameObject::FixedUpdate([[maybe_unused]] float dt) const
{
	
}

void dae::GameObject::Render() const
{
	const TextureComponent* pTexture = GetTexture();
	if (IsComponentPresent(dae::Component::ComponentType::transform) == true)
	{
		const Transform* pTransform = GetTransform();
		const auto& pos = pTransform->GetPosition();


		if (pTexture != nullptr)
		{
			Renderer::GetInstance().RenderTexture(*pTexture->GetTexturePtr(), pos.x, pos.y);
		}
	}
	else
	{
		const glm::vec3& pos{ 0,0,0 };
		if (pTexture != nullptr)
		{
			Renderer::GetInstance().RenderTexture(*pTexture->GetTexturePtr(), pos.x, pos.y);
		}
	}
	
	const TextComponent* pText = GetText();
	if (pText != nullptr)
	{
		pText->Render();
	}
}

void dae::GameObject::AddComponent(Component* component)
{
	m_pComponents.push_back(component);
}

void dae::GameObject::RemoveComponent(Component::ComponentType type)
{
	switch (type)
	{
	case dae::Component::ComponentType::none:
	{
		//Couldn't use get since i wanted the iterator
		auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
			return (i->GetType() == Component::ComponentType::none);
			});
		
		if (result != m_pComponents.end())
		{
			m_pComponents.erase(result);
			delete *result;
			*result = nullptr;
		}
		break;
	}
	case dae::Component::ComponentType::transform:
	{
		//Couldn't use get since i wanted the iterator
		auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
			return (i->GetType() == Component::ComponentType::transform);
			});

		if (result != m_pComponents.end())
		{
			m_pComponents.erase(result);
			delete* result;
			*result = nullptr;
		}
		break;
	}
	case dae::Component::ComponentType::texture:
	{
		//Couldn't use get since i wanted the iterator
		auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
			return (i->GetType() == Component::ComponentType::texture);
			});

		if (result != m_pComponents.end())
		{
			m_pComponents.erase(result);
			delete* result;
			*result = nullptr;
		}
		break;
	}
	case dae::Component::ComponentType::text:
	{
		//Couldn't use get since i wanted the iterator
		auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
			return (i->GetType() == Component::ComponentType::text);
			});

		if (result != m_pComponents.end())
		{
			m_pComponents.erase(result);
			delete* result;
			*result = nullptr;
		}
		break;
	}
	case dae::Component::ComponentType::FPS:
	{
		//Couldn't use get since i wanted the iterator
		auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
			return (i->GetType() == Component::ComponentType::FPS);
			});

		if (result != m_pComponents.end())
		{
			m_pComponents.erase(result);
			delete* result;
			*result = nullptr;
		}
		break;
	}
	}
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

dae::TextureComponent* dae::GameObject::GetTexture() const
{
	auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
		return (i->GetType() == Component::ComponentType::texture);
		});

	if (result == m_pComponents.end())
	{
		return nullptr; //early out if no such component
	}

	return static_cast<TextureComponent*>(*result);
}

dae::TextComponent* dae::GameObject::GetText() const
{
	auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
		return (i->GetType() == Component::ComponentType::text);
		});

	if (result == m_pComponents.end())
	{
		return nullptr; //early out if no such component
	}

	return static_cast<TextComponent*>(*result);
}

dae::FPSComponent* dae::GameObject::GetFPSComponent() const
{
	auto result = std::find_if(m_pComponents.begin(), m_pComponents.end(), [](const Component* i) {
		return (i->GetType() == Component::ComponentType::FPS);
		});

	if (result == m_pComponents.end())
	{
		return nullptr; //early out if no such component
	}

	return static_cast<FPSComponent*>(*result);
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
	Transform* pTransform = GetTransform();

	if (pTransform != nullptr)
	{
		pTransform->SetPosition(x,y,z);
	}

	TextComponent* pText = GetText();

	if (pText != nullptr)
	{
		pText->SetPosition(x, y);
	}
}