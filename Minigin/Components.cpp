#include <stdexcept>
#include <SDL_ttf.h>
#include "Components.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "GameObject.h"

#pragma region Transform

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

#pragma endregion

#pragma region Render

void dae::RenderComponent::Update([[maybe_unused]] float deltaT)
{
	if (m_pOwnerTransform == nullptr) //Check if the transform pointer is set
	{
		m_pOwnerTransform = GetOwner()->GetComponent<Transform>(); //This will only happen the first time
	}
	//or every update if there's no transform, this need optimization but don't know how
}

void dae::RenderComponent::Render() const
{
	const float textureWidthOffset = m_texture->GetSize().x / 2.f;
	const float textureHeightOffset = m_texture->GetSize().y / 2.f;
	if (m_pOwnerTransform == nullptr) //Check if the transform pointer is set
	{
		Renderer::GetInstance().RenderTexture(*m_texture, -textureWidthOffset, -textureHeightOffset);
	}
	else
	{
		const auto& pos = m_pOwnerTransform->GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x - textureWidthOffset, pos.y - textureHeightOffset);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
	m_texture = texture;
}
#pragma endregion

#pragma region Text

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font)
	:Component(pOwner)
	,m_needsUpdate(true)
	,m_text(text)
	,m_font(std::move(font))
{
}

void dae::TextComponent::Update([[maybe_unused]] float deltaT)
{
	if (m_needsUpdate)
	{
		const SDL_Color color{ 255,255,255 };
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		GetOwner()->GetComponent<RenderComponent>()->SetTexture(std::make_shared<Texture2D>(texture));
		m_needsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

//void dae::TextComponent::SetColor(Uint8 r, Uint8 g, Uint8 b)
//{
//	m_color.r = r;
//	m_color.g = g;
//	m_color.b = b;
//}

#pragma endregion

#pragma region FPS

void dae::FPSComponent::Update(float deltaT)
{
	if(m_pOwnerText == nullptr) //Check if the text pointer is set
	{
		m_pOwnerText = GetOwner()->GetComponent<TextComponent>(); //This will only happen the first time
	}
	else
	{
		m_NrOfFrames = static_cast<int>(1 / deltaT);
		const std::string printString = std::to_string(m_NrOfFrames) + " FPS";
		m_pOwnerText->SetText(printString);
	}
}

#pragma endregion

#pragma region Encircle

void dae::Encircle::Update(float deltaT)
{
	float multiplier{ 1 };//+ for anti-clockwise rotation, + for clockwise, could be variable
	if(m_isRotatingClockwise)
	{
		multiplier = -1;
	}

	m_Time += deltaT; 
	m_Offset.x =  m_Distance * cos(multiplier * ( static_cast<float>(2 * M_PI) / m_SecPerRotation) * m_Time);
	m_Offset.y =  m_Distance * sin(multiplier * (static_cast<float>(2 * M_PI) / m_SecPerRotation) * m_Time);
	
	GetOwner()->SetLocalPosition(glm::vec3(m_Offset.x, m_Offset.y, 0.f));
	GetOwner()->SetPositionDirty();
}

#pragma endregion