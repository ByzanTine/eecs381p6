#include "Warrior.h"

#include <iostream>

#include "Utility.h"
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;

void Warrior::start_attacking(shared_ptr<Agent> target_ptr)
{
	if (target_ptr == shared_from_this()) //maybe use get()==this?
		throw Error(get_name() + ": I cannot attack myself!");

	if (!target_ptr->is_alive())
		throw Error(get_name() + ": Target is not alive!");

	if (cartesian_distance(get_location(), target_ptr->get_location()) > attack_range)
		throw Error(get_name() + ": Target is out of range!");
	go_war(target_ptr);

	cout << get_name() << ": I'm attacking!" << endl;
}

void Warrior::stop()
{
	cout << get_name() << ": Don't bother me" << endl;
}


// output information about the current state, need access to all variables
void Warrior::describe() const
{
	Agent::describe();
	// Get the thing from shared_ptr
	if (warrior_state == Warrior_state::Attacking)
	{
		shared_ptr<Agent> shared_target = target.lock();
		if (!shared_target || !shared_target->is_alive())
			cout << "   Attacking dead target" << endl;
		else
			cout << "   Attacking " << shared_target->get_name() << endl;
	}
	else
	{
		cout << "   Not attacking" << endl;	
	}
}

// Make this an abstract class by making the constructor protected to prevent direct creation.
Warrior::Warrior(const string& name_, Point location_, int attack_strength_, int attack_range_):Agent(name_, location_)
{
	// cout << "Warrior " << get_name() << " constructed" << endl;
	warrior_state = Warrior_state::Not_Attacking;
	attack_strength = attack_strength_;
	attack_range = attack_range_;
}
// a derived class specific interface to transfer to the not-attacking state and discard target_ptr
void Warrior::go_peace()
{
	warrior_state = Warrior_state::Not_Attacking;
	target.reset();
}
// a derived class specific interface to transfer to the attacking state and assign target_ptr
void Warrior::go_war(shared_ptr<Agent> target_ptr)
{	
	warrior_state = Warrior_state::Attacking;
	target = target_ptr;
}
// * getters 
bool Warrior::is_attacking()
{
	return warrior_state == Warrior_state::Attacking;
}
// return a shared ptr by the weak ptr, may return nullptr.
shared_ptr<Agent> Warrior::get_target_ptr()
{
	return target.lock();
}