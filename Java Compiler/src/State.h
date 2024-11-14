#include <unordered_map>
#include <vector>
#include <memory>

class State
{
public:
	State();
	void addTransition(std::shared_ptr<State> state, uint8_t transition);
	inline std::unordered_map<char, std::vector<std::shared_ptr<State>>>& getTransitions() { return m_Transitions; }
	inline uint32_t getId() { return m_Id; }
private:
	uint32_t m_Id;
	std::unordered_map<char, std::vector<std::shared_ptr<State>>> m_Transitions;
};
