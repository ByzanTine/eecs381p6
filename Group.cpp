#include "Group.h"
#include "Utility.h"
#include <algorithm>
#include <functional>
using std::shared_ptr;
using std::for_each;
using std::bind;
using namespace std::placeholders;
using std::string;

void Group::move_to(Point destination_)
{
	for_each(members.begin(), members.end(), 
		bind(&Unit::move_to, _1, destination_));
}

void Group::stop() 
{
	for_each(members.begin(), members.end(),
		bind(&Unit::stop, _1));
}

void Group::start_working(shared_ptr<Structure> source_, shared_ptr<Structure> destination_) 
{
	for_each(members.begin(), members.end(), [&](const shared_ptr<Unit> member_ptr) 
	{
		if (member_ptr->can_work())
			member_ptr->start_working(source_, destination_);
	});
}

void Group::start_attacking(shared_ptr<Agent> target) 
{
	for_each(members.begin(), members.end(), [&](const shared_ptr<Unit> member_ptr) 
	{
		if (member_ptr->can_attack())
			member_ptr->start_attacking(target);
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