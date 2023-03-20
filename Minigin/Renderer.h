#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui_plot.h"

struct Transform
{
	float matrix[16] =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};

class GameObject3D
{
public:
	Transform transform;
	int ID;
};

class GameObject3DAlt
{
public:
	Transform* transform;
	int ID;
};

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
		ImGui::PlotConfig* m_pIntPlotConfig{new ImGui::PlotConfig()};
		void TrashTheCacheInts(ImGui::PlotConfig& plotConfig, int samples) const;

		ImGui::PlotConfig* m_pGameObjectPlotConfig{new ImGui::PlotConfig()};
		void TrashTheCacheGameObjects(ImGui::PlotConfig& plotConfig, int samples) const;

		ImGui::PlotConfig* m_pGameObjectAltPlotConfig{ new ImGui::PlotConfig() };
		void TrashTheCacheGameObjectAlts(ImGui::PlotConfig& plotConfig, int samples) const;
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

