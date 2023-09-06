#include "Pch.h"

namespace IS
{
	Physics* PHYSICS = NULL;

	Physics::Physics()
	{
		//ErrorIf(PHYSICS != NULL, "Physics already initialized");
		PHYSICS = this;
		Gravity = Vector2D(0, -400);
		MaxVelocity = 1000;

	}

	void Physics::Initialize()
	{
		//RegisterComponent(Body);
	}

	void Physics::Update(float dt)
	{
		const float TimeStep = 1.0f / 60.0f;

	}
}