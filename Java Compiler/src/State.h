#include <unordered_map>
#include <vector>
#include <memory>

class State
{
public:
	void addTransition(std::shared_ptr<State> state, uint8_t transition);
	std::unordered_map<char, std::vector<std::shared_ptr<State>>>& getTransitions()
	{
		return m_Transitions;
	}
private:
	std::unordered_map<char, std::vector<std::shared_ptr<State>>> m_Transitions;
};
