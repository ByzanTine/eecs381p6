#ifndef WARRIOR_H
#define WARRIOR_H
#include "Agent.h"
#include <memory>

class Warrior: public Agent
{
public:
	// empty update 
	void update() override{};
	// start_attacking is the same protocol
	void start_attacking(std::shared_ptr<Agent> target_ptr) override;
	// Override Stop for the warriors, since they all don't stop
	void stop() override;
	// output information about the current state, need access to all variables
	void describe() const override;

protected:
	// Derived class want to know the state that they can be in
	enum class Warrior_state {Attacking, Not_Attacking};

	// Make this an abstract class by making the constructor protected to prevent direct creation.
	Warrior(const std::string& name_, Point location_, int attack_strength_, int attack_range_);
	// a derived class specific interface to transfer to the not-attacking state and discard target_ptr
	void go_peace();
	// a derived class specific interface to transfer to the attacking state and assign target_ptr
	void go_war(std::shared_ptr<Agent> target_ptr);
	// * Accessors 
	bool is_attacking();
	// return a shared ptr by the weak ptr, may return nullptr.
	std::shared_ptr<Agent> get_target_ptr();
	double get_attack_range()
		{return attack_range;}
	int get_attack_strength()
		{return attack_strength;}


private:
	int attack_strength;
	double attack_range;

	std::weak_ptr<Agent> target;
	
	Warrior_state warrior_state;

};

#endif