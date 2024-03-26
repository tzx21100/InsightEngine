/* Start Header **************************************************************/
/*!
 * \file Particle.cpp
 * \author Tab Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2451
 * \date 25-03-2024
 * \brief
 * Implementation of ParticleSystem
 *
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 */
/* End Header ****************************************************************/
#include "Pch.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "Engine/Core/CoreEngine.h"
#include "Engine/Systems/Asset/Asset.h"
#include "Graphics/System/Animation.h"
#include "Graphics/System/Sprite.h"
#include "../Graphics/System/Transform.h"
#include "../Graphics/System/Light.h"
#include "Graphics/Core/Graphics.h"

namespace IS {
    Animation run_anim;

    std::string ParticleSystem::GetName() {
        return "Particle";
    }

    void ParticleSystem::Initialize() {
        //Subscirbe to messages
        Subscribe(MessageType::DebugInfo);

        run_anim.initAnimation(1, 4, 1.f);
    }

    void ParticleSystem::Update(float deltaTime) {
        auto& engine = InsightEngine::Instance();
        auto asset = engine.GetSystem<AssetManager>("Asset");


            for (int id = 0; id < mParticleAmount; id++) {
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
                    Sprite::draw_colored_quad(mParticleList[id].mParticlePos, mParticleList[id].mRotation, mParticleList[id].mScale, { mParticleList[id].mColor.R, mParticleList[id].mColor.G ,mParticleList[id].mColor.B ,mParticleList[id].mColor.A },mParticleList[id].mLayer);
                    break;

                case pt_circle:
                    for (int i = 0; i < 8; i++) {
                        Sprite::draw_colored_quad(mParticleList[id].mParticlePos, mParticleList[id].mRotation + (i * 45), mParticleList[id].mScale, { mParticleList[id].mColor.R, mParticleList[id].mColor.G ,mParticleList[id].mColor.B ,mParticleList[id].mColor.A }, mParticleList[id].mLayer);
                    }

                    break;

                case pt_texture:
                    Sprite::draw_textured_quad_colour(mParticleList[id].mParticlePos, mParticleList[id].mRotation, mParticleList[id].mScale, *asset->GetImage(mParticleList[id].mImageName), mParticleList[id].mAlpha, mParticleList[id].mLayer, std::tuple<float,float,float>(mParticleList[id].mColor.R, mParticleList[id].mColor.G, mParticleList[id].mColor.B));
                    break;

                case pt_texture_frames:
                    //Sprite::draw_textured_quad(mParticleList[id].mParticlePos, mParticleList[id].mRotation, mParticleList[id].mScale,*asset->GetImage(mParticleList[id].mImageName), mParticleList[id].mAlpha);
                    Sprite::drawSpritesheetFrame(mParticleList[id].mRowIndex, mParticleList[id].mColIndex, mParticleList[id].mTotalRows, mParticleList[id].mTotalCols, mParticleList[id].mParticlePos, mParticleList[id].mRotation, mParticleList[id].mScale, *asset->GetImage(mParticleList[id].mImageName), mParticleList[id].mAlpha, mParticleList[id].mLayer);
                    break;

                case pt_anim:
                    auto system = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
                    Image* img = system->GetImage(mParticleList[id].mImageName);
                    run_anim.drawNonEntityAnimation(deltaTime, mParticleList[id].mParticlePos, mParticleList[id].mRotation, mParticleList[id].mScale, *img, 1.f, mParticleList[id].mLayer);
                    break;
                }
                for (int step = 0; step < InsightEngine::Instance().GetCurrentNumberOfSteps(); ++step)
                {
                    //affectors
                    mParticleList[id].mLifespan -= deltaTime;
                    mParticleList[id].mParticlePos += mParticleList[id].mVelocity * deltaTime * DirectionToVector2D(mParticleList[id].mDirection);
                    mParticleList[id].mScale.x += mParticleList[id].mSizeGrowth * deltaTime;
                    mParticleList[id].mScale.y += mParticleList[id].mSizeGrowth * deltaTime;
                    mParticleList[id].mAlpha += mParticleList[id].mAlphaGrowth * deltaTime;
                    mParticleList[id].mParticlePos.y -= 98.f * deltaTime * static_cast<int>(mParticleList[id].mGravity);
                }

                //particle effect
                switch (mParticleList[id].mEffect) {
                    case effect_normal:
                        break;

                    case effect_swing:
                    mParticleList[id].mParticleEffectTimer -= deltaTime;
                    if (mParticleList[id].mParticleEffectTimer <= 0.f) {
                        if (mParticleList[id].mDirection > 180 && mParticleList[id].mDirection < 270) {
                            mParticleList[id].mDirection += 70;
                        }
                        else if (mParticleList[id].mDirection > 270 && mParticleList[id].mDirection < 360) {
                            mParticleList[id].mDirection -= 70;
                        }
                        mParticleList[id].mRotation += 180;
                        mParticleList[id].mParticleEffectTimer= mParticleList[id].mParticleEffectTimerSet;
                    }
                        break;

                    case effect_spin:
                        mParticleList[id].mRotation += mParticleList[id].mRotationSpeed;
                        break;

                    case effect_animate:
                        mParticleList[id].mParticleEffectTimer -= deltaTime;
                        if (mParticleList[id].mParticleEffectTimer <= 0.f) {
                            if (mParticleList[id].mColIndex < mParticleList[id].mTotalCols-1) {
                                mParticleList[id].mColIndex++;
                            }
                            else {
                                mParticleList[id].mColIndex = 0;
                            }

                            mParticleList[id].mParticleEffectTimer = mParticleList[id].mParticleEffectTimerSet;
                        }
                        break;

                    case effect_light: 
                        Sprite::instanceData lightData;

                        lightData.color = { mParticleList[id].mColor.R, mParticleList[id].mColor.G, mParticleList[id].mColor.B, mParticleList[id].mColor.A };

                        Transform lightXform(mParticleList[id].mParticlePos, 0.f, mParticleList[id].mScale);
                        lightData.model_to_ndc_xform = lightXform.Return3DXformMatrix();
                        lightData.entID = 2; // no idea what its supposed to do

                        Light::lightPos.emplace_back(mParticleList[id].mParticlePos.x, mParticleList[id].mParticlePos.y);
                        Light::lightClr.emplace_back(lightData.color);
                        ISGraphics::lightInstances.emplace_back(lightData);
                        ISGraphics::lightRadius.emplace_back(mParticleList[id].mScale.x);
                        break;
                    

                }


                
            }

            for (auto const& emitters : mEntities) {
                auto& emitter = engine.GetComponent<ParticleEmitter>(emitters);
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