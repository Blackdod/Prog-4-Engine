#include "Subject.h"
#include "Observer.h"

namespace dae
{
	void Subject::AddObserver(Observer* observer)
	{
		m_Observers.push_back(observer);
	}

	void Subject::RemoveObserver(const Observer* observer)
	{
		for (auto it = m_Observers.begin(); it != m_Observers.end(); ++it) {
			if (*it == observer) {
				m_Observers.erase(it);
			}
		}
	}

	void Subject::Notify(const GameObject& other, Event event, const int optionalValue) const
	{
		for (auto const& it : m_Observers) {
			it->OnNotify(other, event, optionalValue);
		}
	}
}

