#include "Peasant.h"

#include <iostream>
#include <cassert>

#include "Model.h"
#include "Utility.h"
#include "Structure.h"
using std::cout;
using std::cerr;
using std::endl;
using std::shared_ptr;

const double max_carry = 35.0;
Peasant::Peasant(const std::string& name_, Point location_):Agent(name_, location_)
{
	food_carrying = 0.;
	peasant_state = Peasant_state::Not_working;

}

// implement Peasant behavior
void Peasant::update()
{
	Agent::update();
	if (!is_alive() || 
		peasant_state == Peasant_state::Not_working)
		return;
	if (peasant_state == Peasant_state::Inbound &&
		!is_moving())
	{
		peasant_state = Peasant_state::Collecting;
		return;
	}
	if (peasant_state == Peasant_state::Collecting)
	{
		double request = max_carry - food_carrying;

		double recieved = source->withdraw(request);
		food_carrying += recieved;
		if (recieved > 0)
		{
			cout << get_name() << ": Collected " << recieved << endl;
			peasant_state = Peasant_state::Outbound;
			Agent::move_to(destination->get_location());
		}
		else
		{
			cout << get_name() << ": Waiting " << endl;
		}
		Model::get_instance().notify_food_carry(get_name(), food_carrying);
		return;
	}
	if (peasant_state == Peasant_state::Outbound &&
		!is_moving())
	{
		peasant_state = Peasant_state::Depositing;
		return;
	}
	

	if (peasant_state == Peasant_state::Depositing)
	{
		destination->deposit(food_carrying);
		cout << get_name() << ": Deposited " << food_carrying << endl;
		food_carrying = 0.;
		Agent::move_to(source->get_location());
		Model::get_instance().notify_food_carry(get_name(), food_carrying);
		peasant_state = Peasant_state::Inbound;
	}
}

// overridden to suspend working behavior
void Peasant::move_to(Point dest)
{

	if (peasant_state != Peasant_state::Not_working)
	{
		cout << get_name() << ": I'm stopping work" << endl;
		peasant_state = Peasant_state::Not_working;
		
	}
	Agent::move_to(dest);
}
// stop moving and working
void Peasant::stop()
{
	Agent::stop();
	if (peasant_state != Peasant_state::Not_working)
	{
		cout << get_name()  << ": I'm stopping work" << endl;
		peasant_state = Peasant_state::Not_working;
		source = nullptr;
		destination = nullptr;
	}

}

// starts the working process
// Throws an exception if the source is the same as the destination.
void Peasant::start_working(shared_ptr<Structure> source_, shared_ptr<Structure> destination_)
{
	Agent::stop();
	peasant_state = Peasant_state::Not_working;
	source = nullptr;
	destination = nullptr;
	if (source_ == destination_)
		throw Error(get_name() + ": I can't move food to and from the same place!");
	source = source_;
	destination = destination_;
	if (food_carrying == 0.)
	{
		Point source_location = source->get_location();
		if (get_location() == source_location)
		{
			peasant_state = Peasant_state::Collecting;
		}
		else
		{
			Agent::move_to(source_location);
			peasant_state = Peasant_state::Inbound;
		}
		

	}
	else
	{
		Point dest_location = destination->get_location();
		if (get_location() == dest_location)
		{
			peasant_state = Peasant_state::Depositing;
		}
		else
		{
			Agent::move_to(dest_location);
			peasant_state = Peasant_state::Outbound;
		}
	}
}

// output information about the current state
void Peasant::describe() const
{
	cout << "Peasant ";
	Agent::describe();
	cout << "   Carrying " << food_carrying << endl;
	switch(peasant_state)
	{
		case Peasant_state::Not_working:
			break;
		case Peasant_state::Outbound:
			cout << "   Outbound to destination " << destination->get_name() << endl;
			break;
		case Peasant_state::Inbound:
			cout << "   Inbound to source " << source->get_name() << endl;
			break;
		case Peasant_state::Collecting:
			cout << "   Collecting at source " << source->get_name() << endl;
			break;
		case Peasant_state::Depositing:
			cout << "   Depositing at destination " << destination->get_name() << endl;
			break;	
		default:
			cerr << "Peasant in a inaccessible state" << endl;
			assert(0);
	}
}
// update the current state
void Peasant::broadcast_current_state()
{
	Agent::broadcast_current_state();
	Model::get_instance().notify_food_carry(get_name(), food_carrying);
}