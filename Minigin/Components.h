#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>


namespace dae
{
	class Texture2D;
	class Font;
	class GameObject;

	class Component
	{
	public:
		//Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update([[maybe_unused]] float deltaT) {};
		virtual void Render() const {};
	protected:
		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {};
		GameObject* GetOwner() const { return m_pOwner; };
	private:
		GameObject* m_pOwner{nullptr};
	};

	class Transform final : public Component
	{
	public:
		Transform(GameObject* pOwner, float x = 0.f, float y = 0.f, float z = 0.f)
		:Component(pOwner)
		,m_position{ x, y, z }
		{};

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_position;
	};

	class RenderComponent final : public Component
	{
	public:
		RenderComponent(GameObject* pOwner)
		:Component(pOwner)
		{};

		void Update([[maybe_unused]] float deltaT) override;
		void Render() const override;
		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D>& texture);
		std::shared_ptr<Texture2D> GetTexturePtr() const { return m_texture; };
	private:
		std::shared_ptr<Texture2D> m_texture{ nullptr };
		Transform* m_pOwnerTransform{ nullptr };
	};

	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font);

		void Update([[maybe_unused]] float deltaT) override;

		void SetText(const std::string& text);
		//void SetColor(Uint8 r, Uint8 g, Uint8 b);
	private:
		bool m_needsUpdate; //Only update at initialize or if text changes
		std::string m_text;
		std::shared_ptr<Font> m_font{ nullptr };
		//SDL_Color m_color;
	};

	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* pOwner)
		:Component(pOwner)
		{};

		void Update(float deltaT) override;
		int GetNrOfFrames() const { return m_NrOfFrames; };

	private:
		int m_NrOfFrames{0};
		TextComponent* m_pOwnerText{ nullptr };
	};
}