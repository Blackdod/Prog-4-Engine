#pragma once
#include <list>
#include <memory>

#include "Event.h"

namespace dae
{
	class GameObject;
	class Observer;
	class Subject final
	{
	public:
		void AddObserver(Observer* observer);
		void RemoveObserver(const Observer* observer);
		void Notify(const GameObject& other, Event event, int optionalValue = 0) const;
	private:
		std::list<Observer*> m_Observers;

	};
}