#ifndef VIEW_FACTORY_H
#define VIEW_FACTORY_H

#include <string>
#include <memory>

struct Point;
class View;

// Create and return the pointer to the specified View type. If the type
// is unrecognized, throws Error("Trying to create View of unknown type!")
// The View is allocated with new, so some other component is resposible for deleting it.
std::shared_ptr<View> create_view(const std::string& type);

#endif
