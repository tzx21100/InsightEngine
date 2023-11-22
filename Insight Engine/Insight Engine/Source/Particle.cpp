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

        for (int id = 0; id < mParticleAmount;id++) {

            //particles death

            if (mParticleList[id].mLifespan <= 0) {
                DeleteParticle(id);
                continue;
            }

            if (mParticleList[id].mAlpha <= 0) {
                DeleteParticle(id);
                continue;
            }
                
                
            // particle types

            switch (mParticleList[id].mParticleType) 
            {
            case pt_square:

                Sprite::draw_colored_quad(mParticleList[id].mParticlePos, mParticleList[id].mRotation, mParticleList[id].mScale, { mParticleList[id].mColor.R, mParticleList[id].mColor.G ,mParticleList[id].mColor.B ,mParticleList[id].mColor.A });
                break;

            case pt_circle:

                for (int i = 0; i < 8; i++) {
                    Sprite::draw_colored_quad(mParticleList[id].mParticlePos, mParticleList[id].mRotation +(i*45), mParticleList[id].mScale, {mParticleList[id].mColor.R, mParticleList[id].mColor.G ,mParticleList[id].mColor.B ,mParticleList[id].mColor.A});
                }

                break;

            case pt_texture:

                Sprite::draw_textured_quad(mParticleList[id].mParticlePos, mParticleList[id].mRotation, mParticleList[id].mScale,*asset->GetImage(mParticleList[id].mImageName), 1.f);

                break;
                
            }

            //affectors
            mParticleList[id].mLifespan -= deltaTime;
            mParticleList[id].mParticlePos += mParticleList[id].mVelocity * deltaTime * DirectionToVector2D(mParticleList[id].mDirection);
            mParticleList[id].mScale.x += mParticleList[id].mSizeGrowth * deltaTime;
            mParticleList[id].mScale.y += mParticleList[id].mSizeGrowth * deltaTime;
            mParticleList[id].mAlpha += mParticleList[id].mAlphaGrowth * deltaTime;


        }

        for (auto const& emitters : mEntities) {
            auto emitter= engine.GetComponent<ParticleEmitter>(emitters);
            for (int i = 0; i < emitter.mParticlesAmount; i++) {
                SpawnParticles(emitter.mParticle);
            }

        }   

    }




    void ParticleSystem::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            //IS_CORE_INFO("Handling Debug");
        }
    }





}