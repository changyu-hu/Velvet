#include "Component.hpp"
#include "Actor.hpp"


std::shared_ptr<Velvet::Transform> Velvet::Component::transform()
{
	if (actor)
	{
		return actor->transform;
	}
	else
	{
		return std::make_shared<Velvet::Transform>(Velvet::Transform(nullptr));
	}
}
