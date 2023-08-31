#include "Pch.h"
#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "System.h"
#include <iostream>

namespace IS {

    class PhysicsSystem : public ParentSystem {
    public:
        void Update(float time) override {
            // Your physics update logic here
            std::cout << "Physics update with delta time: " << time << std::endl;
        }

        std::string getName() override {
            return "PhysicsSystem";
        }

        void Initialize() override {
            // Initialization logic here
            std::cout << "PhysicsSystem initialized." << std::endl;
        }

        void HandleMessage(const Message& message) override {
            if (message.GetType() == MessageType::Collide) {
                // Handle collision logic here
                std::cout << "Handling collision in PhysicsSystem." << std::endl;
            }
        }
    };

}

#endif // PHYSICS_SYSTEM_H
