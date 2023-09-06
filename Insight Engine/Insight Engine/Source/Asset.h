#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H

#include "System.h"

namespace IS {
    class ISAsset :public ParentSystem {
    public:
        //override parent
        void Initialize() override;
        void Update(float deltaTime) override;
        std::string getName() override;
        void HandleMessage(const Message& message) override;

        ISAsset();
        ~ISAsset();


    private:

    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H  