#include "Pch.h"
#include "GameObject.h"

namespace IS {

    //constructor
    ISGameObject::ISGameObject() : position(0.f, 0.f), velocity(0.f, 0.f), state(true) {};
    //update the object
    void ISGameObject::Update(float deltaTime){}
    // Draw the game object. This could be abstract if all game objects have unique visual representations.
    void ISGameObject::Draw(){}
    // Clone this object. this returns a pointer and *this returns the object itself
    ISGameObject* ISGameObject::Clone() const {
        return new ISGameObject(*this);
    }

}