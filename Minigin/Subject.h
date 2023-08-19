#pragma once
#include <vector>
#include <memory>

#include "Observer.h"

namespace dae
{
	template<typename... Args>
	class Subject final {
		std::vector<Observer<Args...>*> m_observers;
	public:
		~Subject()
		{
			for (auto& observer : m_observers)
			{
				observer->OnSubjectDestroy();
			}
		}
		void AddObserver(Observer<Args...>* observer)
		{
			m_observers.push_back(observer);
		}
		void RemoveObserver(Observer<Args...>* observer)
		{
			m_observers.erase(std::remove(
				m_observers.begin(),
				m_observers.end(), observer),
				m_observers.end());
		}
		void Notify(Args... args)
		{
			for (auto& observer : m_observers)
			{
				observer->HandleEvent(args...);
			}
		}
	};
}