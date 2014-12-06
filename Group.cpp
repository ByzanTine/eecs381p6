#include "Group.h"
#include "Agent.h"
#include "Utility.h"
#include <iostream>
#include <algorithm>
#include <functional>
using std::cout;
using std::endl;
using std::shared_ptr;
using std::for_each;
using std::bind;
using namespace std::placeholders;
using std::string;

Group::Group(string name_) : name(name_) {}

void Group::move_to(Point destination_)
{
	cout << "Group: " << name << " move to " << destination_ << endl;
	for_each(members.begin(), members.end(), 
		bind(&Unit::move_to, _1, destination_));
}

void Group::stop() 
{
	cout << "Group: " << name << " stop" << endl;
	for_each(members.begin(), members.end(),
		bind(&Unit::stop, _1));
}

void Group::start_working(shared_ptr<Structure> source_, shared_ptr<Structure> destination_) 
{
	cout << "Group: " << name << " starg working"<<endl;
	for_each(members.begin(), members.end(), [&](const shared_ptr<Unit> member_ptr) 
	{
		try
		{
			member_ptr->start_working(source_, destination_);
		}
		catch (Error& e) 
		{
			cout << e.what() << endl;
		}
	});
}

void Group::start_attacking(shared_ptr<Agent> target) 
{
	cout << "Group: " << name << " starting attacking " << target->get_name() << endl;
	for_each(members.begin(), members.end(), [&](const shared_ptr<Unit> member_ptr) 
	{
		try
		{
			member_ptr->start_attacking(target);
		}
		catch (Error& e) {}
	});
}

void Group::add_component(shared_ptr<Unit> unit_ptr) 
{
	if (shared_from_this() == unit_ptr)
		throw (Error("Cannot add itself!"));
	if (is_offspring_of(unit_ptr))
		throw (Error("No cycle is allowed!"));
	members.insert(unit_ptr);
	unit_ptr->set_parent(shared_from_this());
}

void Group::remove_component(shared_ptr<Unit> unit_ptr) 
{
	auto member_it = members.find(unit_ptr);
	if (member_it == members.end())
		return;
	members.erase(member_it);
	unit_ptr->set_parent(shared_ptr<Unit>());
}
