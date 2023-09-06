#include "Pch.h"
#include "Asset.h"

namespace IS {
    void ISAsset::Initialize() {//call once
        //constuctor init alr

    }

    void ISAsset::Update([[maybe_unused]] float deltaTime) {//every frame

    }

    std::string ISAsset::getName() {
        return "ISAsset";
    }

    void ISAsset::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            std::cout << "Handling Debug" << std::endl;
        }
    }

    // Constructor
    ISAsset::ISAsset() {

    }

    // Destructor
    ISAsset::~ISAsset() {

    }
}