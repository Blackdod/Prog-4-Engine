#pragma once
#include <string>
#include <functional>

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		float GetFixedTimeStep() const { return m_FixedTimeStep; };

	private:
		const float m_FixedTimeStep{ 0.02f };
		const int m_DesiredFPS{ 144 };
		const int m_FrameTimeMs{ 1000 / m_DesiredFPS };
	};
}