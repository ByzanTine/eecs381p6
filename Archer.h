#ifndef ARCHER_H
#define ARCHER_H

#include "Warrior.h"
#include <memory>
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