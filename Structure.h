/* A Structure is a Sim_object with a location and interface to derived types */

#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <string>

#include "Sim_object.h"
#include "Geometry.h" // Need to know the size of location

class Structure : public Sim_object{


public:
	// Constructor
	Structure(const std::string& name_, Point location_, double food_ = 0.0);
	
	// Destructor, pure virtual to avoid instance creation
	virtual ~Structure() = 0;
	
	Point get_location() const override
		{return location;}

	void update() override {} ;

	// output information about the current state
	void describe() const override;

	// ask model to notify views of current state
    void broadcast_current_state() override;
    
	// fat interface for derived types	
	virtual double withdraw(double amount_to_get);
	virtual void deposit(double amount_to_give);
protected:
	// food accessors
	double get_food() const
		{return food;}
	// give negative values if you want the food amount decrease
	// notify the changes as well.
	void add_food(double food_added);
	
private:
	Point location;
	double food; // needed for the broadcast
};

#endif