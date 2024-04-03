/*!
 * \file Animation.cpp
 * \author Koh Yan Khang, yankhang.k@digipen.edu
 * \par Course: CSD2451
 * \date 25-11-2023
 * \brief
 * This source file defines the Animation class, which manages sprite animations.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

#include "Pch.h"
#include "Animation.h"
#include "Graphics/Core/Graphics.h"
#include "Graphics/System/Sprite.h"

namespace IS {

    void Animation::initAnimation(int rows, int columns, float animation_time) {
        // init animation object with values
        x_frames = columns;
        y_frames = rows;
        animation_duration = animation_time;
        time_per_frame = animation_time / (x_frames * y_frames);
        frame_dimension = glm::vec2{ (1.f / x_frames), (1.f / y_frames) };
    }

    void Animation::updateAnimation(float deltaTime) {
        for (int step = 0; step < InsightEngine::Instance().GetCurrentNumberOfSteps(); ++step)
        {
            //Loop using Fixed DT
            //deltaTime = static_cast<float>(InsightEngine::Instance().mFixedDeltaTime);
            // Update function to advance the animation

            if (time_per_frame == 0 || !is_playing)
                return;

            frame_timer += deltaTime;
            frame_dimension = glm::vec2{ (1.f / x_frames), (1.f / y_frames) };
            time_per_frame = animation_duration / (x_frames * y_frames);

            

            while (frame_timer >= time_per_frame) {
                frame_timer -= time_per_frame;
                // go to next column
                ++frame_index.x;

                // after last column
                if (frame_index.x >= x_frames) {
                    // return to first 
                    frame_index.x = 0;
                    // go to next row
                    ++frame_index.y;
                    // after last frame, reset to first
                    if (frame_index.y >= y_frames) frame_index.y = 0;
                }
            }
        }
        
    }

    void Animation::drawNonEntityAnimation(float deltaTime, Vector2D const& pos, float rotation, Vector2D const& scale, Image const& texture, float alpha, int layer) {
        for (int step = 0; step < InsightEngine::Instance().GetCurrentNumberOfSteps(); ++step)
        {
            this->updateAnimation(deltaTime);

            if (Sprite::layersToIgnore.find(layer) == Sprite::layersToIgnore.end()) {

                Transform quadTRS(pos, rotation, scale);
                glm::mat4 world_to_NDC_xform = quadTRS.Return3DXformMatrix();

                Sprite::instanceData instData;
                instData.color = { 1.f, 1.f, 1.f, alpha };
                instData.tex_index = static_cast<float>(texture.texture_index);
                instData.model_to_ndc_xform = world_to_NDC_xform;
                instData.layer = layer;
                instData.anim_frame_dimension = this->frame_dimension;
                instData.anim_frame_index = this->frame_index;

                ISGraphics::layeredQuadInstances.insert(instData);

            }
        }   
    }

    float Animation::getFrameWidth() const {
        // returns width of each frame
        return 1.f / x_frames; 
    }

    float Animation::getFrameHeight() const {
        // returns height of each frame
        return 1.f / y_frames; 
    }

    void Animation::resetAnimation() {
        // set indexes and timer to 0
        frame_index.x = 0;
        frame_index.y = 0;
        frame_timer = 0;
    }

    void Animation::setFrameIndex(int frameIndex) {
        if (frameIndex < 0 || frameIndex >= (x_frames * y_frames)) {
            IS_CORE_ERROR("setFrameIndex function - invalid index entered.");
            return;
        }

        frame_index.x = static_cast<float>(frameIndex % x_frames);
        frame_index.y = static_cast<float>(frameIndex / x_frames);
    }
}