#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

#include <chrono>
#include <numeric>


int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	//GUI here
	//Static because stuff didn't work as member variable
	static int nrOfSamples{ 10 };
	static bool showIntGraph{ false };
	ImGui::PlotConfig intPlotConfig{};

	ImGui::Begin("Exercise 2"); //Create a window called "Exercise 2"
	ImGui::InputInt("samples", &nrOfSamples);
	if(ImGui::Button("Trash the Cache") && !showIntGraph)
	{
		showIntGraph = true;
		TrashTheCacheInts(intPlotConfig, nrOfSamples);
		*m_pIntPlotConfig = ImGui::PlotConfig{ intPlotConfig };
	}

	if (showIntGraph)
	{
		ImGui::Plot("Trash the Cache Ints", *m_pIntPlotConfig);
	}
	ImGui::End();

	static bool showGameObjectGraph{ false };
	ImGui::PlotConfig gameObjectPlotConfig{};
	ImGui::PlotConfig gameObjectAltPlotConfig{};

	ImGui::Begin("Exercise 3"); //Create a window called "Exercise 3"
	ImGui::InputInt("samples", &nrOfSamples);
	if (ImGui::Button("Trash the Cache") && !showGameObjectGraph)
	{
		showGameObjectGraph = true;
		TrashTheCacheGameObjects(gameObjectPlotConfig, nrOfSamples);
		*m_pGameObjectPlotConfig = ImGui::PlotConfig{ gameObjectPlotConfig };

		TrashTheCacheGameObjectAlts(gameObjectAltPlotConfig, nrOfSamples);
		*m_pGameObjectAltPlotConfig = ImGui::PlotConfig{ gameObjectAltPlotConfig };
	}

	if (showGameObjectGraph)
	{
		ImGui::Plot("Trash the Cache GameObjects", *m_pGameObjectPlotConfig);
		ImGui::Plot("Trash the Cache GameObjectAlts", *m_pGameObjectAltPlotConfig);
	}

	ImGui::End();

	//ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	delete m_pIntPlotConfig;
	delete m_pGameObjectPlotConfig;
	delete m_pGameObjectAltPlotConfig;
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }


void dae::Renderer::TrashTheCacheInts(ImGui::PlotConfig& plotConfig, int samples) const
{
	const size_t size = 1'000'000;
	int* chungus = new int[size] {};

	delete plotConfig.values.ys;

	std::vector<float> averages{};
	std::vector<float> xData{};

	for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
	{
		std::vector<long long> durations{};
		for (int j{}; j < samples; ++j)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (int i{}; i < size; i += stepsize)
			{
				chungus[i] *= 2;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto deltaT = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			durations.push_back(deltaT);
		}
		durations.erase(std::max_element(durations.begin(), durations.end()));
		durations.erase(std::min_element(durations.begin(), durations.end()));

		long long longSize{ long long(durations.size()) };

		float average{ (float)std::accumulate(durations.begin(), durations.end(), 0LL) / longSize };
		averages.push_back(average);

		xData.push_back((float)stepsize);
	}
	const int dataBufferSize{ (int)averages.size() };
	static float* pDataX = new float[dataBufferSize] {}; //These cause small memory leaks
	static float* pDataY = new float[dataBufferSize] {};

	for (size_t i{}; i < dataBufferSize; ++i)
	{
		pDataX[i] = xData[i];
	}

	for(size_t i{}; i < dataBufferSize; ++i)
	{
		pDataY[i] = averages[i];
	}

	plotConfig.values.xs = pDataX; // this line is optional
	plotConfig.values.ys = pDataY;
	plotConfig.values.count = dataBufferSize;
	plotConfig.scale.min = 0;
	plotConfig.scale.max = pDataY[0];
	plotConfig.tooltip.show = true;
	plotConfig.tooltip.format = "x=%.2f, y=%.2f";
	plotConfig.grid_x.show = true;
	plotConfig.grid_y.show = true;
	plotConfig.frame_size = ImVec2(200, 200);
	plotConfig.line_thickness = 2.f;
	plotConfig.values.color = ImColor{ 200, 150, 0 };

	delete[] chungus;
}

void dae::Renderer::TrashTheCacheGameObjects(ImGui::PlotConfig& plotConfig, int samples) const
{
	const size_t size = 1'000'000;
	GameObject3D* chungus = new GameObject3D[size] {};

	delete plotConfig.values.ys;

	std::vector<float> averages{};
	std::vector<float> xData{};

	for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
	{
		std::vector<long long> durations{};
		for (int j{}; j < samples; ++j)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (int i{}; i < size; i += stepsize)
			{
				chungus[i].ID *= 2;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto deltaT = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			durations.push_back(deltaT);
		}
		durations.erase(std::max_element(durations.begin(), durations.end()));
		durations.erase(std::min_element(durations.begin(), durations.end()));

		long long longSize{ long long(durations.size()) };

		float average{ (float)std::accumulate(durations.begin(), durations.end(), 0LL) / longSize };
		averages.push_back(average);

		xData.push_back((float)stepsize);
	}
	const int dataBufferSize{ (int)averages.size() };
	static float* pDataX = new float[dataBufferSize] {};
	static float* pDataY = new float[dataBufferSize] {};

	for (size_t i{}; i < dataBufferSize; ++i)
	{
		pDataX[i] = xData[i];
	}

	for (size_t i{}; i < dataBufferSize; ++i)
	{
		pDataY[i] = averages[i];
	}

	plotConfig.values.xs = pDataX; // this line is optional
	plotConfig.values.ys = pDataY;
	plotConfig.values.count = dataBufferSize;
	plotConfig.scale.min = 0;
	plotConfig.scale.max = pDataY[0];
	plotConfig.tooltip.show = true;
	plotConfig.tooltip.format = "x=%.2f, y=%.2f";
	plotConfig.grid_x.show = true;
	plotConfig.grid_y.show = true;
	plotConfig.frame_size = ImVec2(200, 200);
	plotConfig.line_thickness = 2.f;
	plotConfig.values.color = ImColor{ 200, 150, 0 };

	delete[] chungus;
}

void dae::Renderer::TrashTheCacheGameObjectAlts(ImGui::PlotConfig& plotConfig, int samples) const
{
	const size_t size = 1'000'000;
	GameObject3DAlt* chungus = new GameObject3DAlt[size]{};

	delete plotConfig.values.ys;

	std::vector<float> averages{};
	std::vector<float> xData{};

	for (int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
	{
		std::vector<long long> durations{};
		for (int j{}; j < samples; ++j)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (int i{}; i < size; i += stepsize)
			{
				chungus[i].ID *= 2;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto deltaT = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			durations.push_back(deltaT);
		}
		durations.erase(std::max_element(durations.begin(), durations.end()));
		durations.erase(std::min_element(durations.begin(), durations.end()));

		long long longSize{ long long(durations.size()) };

		float average{ (float)std::accumulate(durations.begin(), durations.end(), 0LL) / longSize };
		averages.push_back(average);

		xData.push_back((float)stepsize);
	}
	const int dataBufferSize{ (int)averages.size() };
	static float* pDataX = new float[dataBufferSize] {};
	static float* pDataY = new float[dataBufferSize] {};

	for (size_t i{}; i < dataBufferSize; ++i)
	{
		pDataX[i] = xData[i];
	}

	for (size_t i{}; i < dataBufferSize; ++i)
	{
		pDataY[i] = averages[i];
	}

	plotConfig.values.xs = pDataX; // this line is optional
	plotConfig.values.ys = pDataY;
	plotConfig.values.count = dataBufferSize;
	plotConfig.scale.min = 0;
	plotConfig.scale.max = pDataY[0];
	plotConfig.tooltip.show = true;
	plotConfig.tooltip.format = "x=%.2f, y=%.2f";
	plotConfig.grid_x.show = true;
	plotConfig.grid_y.show = true;
	plotConfig.frame_size = ImVec2(200, 200);
	plotConfig.line_thickness = 2.f;
	plotConfig.values.color = ImColor{ 200, 150, 0 };

	delete[] chungus;
}