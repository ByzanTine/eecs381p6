/*
A Soldier is an Agent that has attack and defense behaviors. It can be commanded
to start attacking another Agent and will continue the attack as long as 
it is alive and the target is alive and in range. If attacked, the Soldier will
start attacking its attacker.
*/
	

#ifndef SOLDIER_H
#define SOLDIER_H
#include <memory>
#include "Agent.h"

// // base class, instance creation is not allowed
class Warrior: public Agent
{
public:
	// virtual destructor since it's base class
	virtual ~Warrior();
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

// A soldier is a special warrior that do counter-attack
class Soldier: public Warrior
{
public:
	
	Soldier(const std::string& name_, Point location_);
	~Soldier();
	
	// update implements Soldier behavior
	void update() override;
	
	
	// Overrides Agent's take_hit to counterattack when attacked.
	void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
	
	// output information about the current state
	void describe() const override;

};


// An archer is a special warrior that run-away when attacked 
// Also it won't stop shooting people
class Archer: public Warrior
{
public:
	
	Archer(const std::string& name_, Point location_);
	~Archer();
	
	// update implements Soldier behavior
	void update() override;
	
	
	// Overrides Agent's take_hit to counterattack when attacked.
	void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
	
	// output information about the current state
	void describe() const override;
	

};

#endif