#ifndef GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H
#define GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H

#include "System.h"
#include "AssetComponents.h"

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

        void ISImageLoad(Image* img, const char* file_name);
        void ISImageCreate(Image * img, int width, int height, int channels, bool zeroed);
        void ISImageSave(const Image * img, const char* file_name);
        void ISImageFree(Image * img);

        void ISImageToGray(const Image * original, Image * gray);
        void ISImageToSepia(const Image * original, Image * sepia);

    private:

    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_ASSET_H  