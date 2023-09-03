#ifndef GAM200_INSIGHT_ENGINE_SOURCE_GAMEOBJECT_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_GAMEOBJECT_H_

#include "Vector2D.h"
#include <string>
#include <windows.data.json.h>

namespace IS {
    class ISGameObject {
    public:

        //3 basic ideas that all game objects need
        Vector2D position, velocity;
        bool state;
        int id;
        std::string type;

        ISGameObject();
        // Update the game object's state. This could be abstract if all game objects need unique updates.
        virtual void Update(float deltaTime);
        // Draw the game object. This could be abstract if all game objects have unique visual representations.
        virtual void Draw();
        // Clone this object.
        virtual ISGameObject* Clone() const;
    };
}

#endif