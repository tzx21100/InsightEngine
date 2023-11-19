#include "Pch.h"
#include "Particle.h"
#include "ParticleEmitter.h"

namespace IS {
    std::string ParticleSystem::GetName() {
        return "Particle";
    }

    void ParticleSystem::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);
    }

    void ParticleSystem::Update(float deltaTime) {
        auto& engine = InsightEngine::Instance();
        auto asset = engine.GetSystem<AssetManager>("Asset");

        for (auto it = mParticleList.begin(); it != mParticleList.end(); it++) {

            //particles death

            if (it->mLifespan <= 0) {
                it = mParticleList.erase(it);
            }

            if (it->mAlpha <= 0) {
                it = mParticleList.erase(it);
            }
                
                
            // particle types

            switch (it->mParticleType) 
            {
            case pt_square:

                Sprite::draw_colored_quad(it->mParticlePos, it->mRotation, it->mScale, { it->mColor.R, it->mColor.G ,it->mColor.B ,it->mColor.A });
                break;

            case pt_circle:

                for (int i = 0; i < 8; i++) {
                    Sprite::draw_colored_quad(it->mParticlePos, it->mRotation +(i*45), it->mScale, {it->mColor.R, it->mColor.G ,it->mColor.B ,it->mColor.A});
                }

                break;

            case pt_texture:

                Sprite::draw_textured_quad(it->mParticlePos, it->mRotation, it->mScale,*asset->GetImage(it->mImageName));

                break;
                
            }

            //affectors
            it->mLifespan -= deltaTime;
            it->mParticlePos += it->mVelocity * deltaTime * DirectionToVector2D(it->mDirection);
            it->mScale.x += it->mSizeGrowth * deltaTime;
            it->mScale.y += it->mSizeGrowth * deltaTime;
            it->mAlpha += it->mAlphaGrowth * deltaTime;


        }

           

    }



    void ParticleSystem::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
        }
    }



    void ParticleSystem::ClearParticles(){
        //mParticles.clear();
    }


}