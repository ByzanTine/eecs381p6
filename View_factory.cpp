#include "View_factory.h"
#include "Map_view.h"
#include "Local_view.h"
#include "Agriculture_view.h"
#include "Amounts_view.h"
#include "Health_view.h"
#include "Utility.h"
#include "Model.h"

using std::shared_ptr;
using std::make_shared;
using std::string; 

shared_ptr<View> create_view(const string& type)
{

	if (type == "map")
		return make_shared<Map_view>();
	else if (type == "health")
		return make_shared<Health_view>();
	else if (type == "amounts")
		return make_shared<Amounts_view>();
	else if (type == "agriculture")
		return make_shared<Agriculture_view>();
	else if (Model::get_instance().is_name_in_use(type)) // find if the object exist 
		return make_shared<Local_view>(type); // here the type is agent name
	else
			throw Error("No object of that name!");
}