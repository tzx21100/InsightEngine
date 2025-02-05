/*!
 * \file StartButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the StartButtonScript class, used for the start button 
 * in the main menu
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System.Runtime.CompilerServices;
namespace IS
{
    class StartButtonScript
    {

        static public bool first_hovering = false;
        static public bool start_button_click = false;
        static public float timer = 0f;
        static SimpleVector2D dark_circle = new SimpleVector2D(10000f, 10000f);
        static SimpleImage scene_circle;

        //static int button_entity;

        static public void Init() {
            timer = 0.5f;
            scene_circle = InternalCalls.GetSpriteImage("dark_circle.png");
            //button_entity = InternalCalls.GetCurrentEntityID();
            start_button_click = false;
            InternalCalls.AttachCamera(0f, 0f);
            InternalCalls.CameraSetZoom(1f);
        }

        static public void Update(){
            // default scale size
            //InternalCalls.SetButtonSizeScale(button_entity, InternalCalls.GetButtonIdleScale(button_entity));
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hovering) {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
                    first_hovering = true;
                }
            }
            else
            {
                first_hovering = false;
            }

            // clicking
            if (InternalCalls.GetButtonState() == 2) 
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                start_button_click = true;
            }

            if (start_button_click)
            {
                //SimpleImage cirlce = scene_circle;
                SimpleVector2D circle_scale = new SimpleVector2D(dark_circle.x * timer, dark_circle.y * timer);
                //InternalCalls.DrawImageAt(new SimpleVector2D(0,0), 0f, circle_scale, scene_circle, 1f, 4);
                timer -= InternalCalls.GetDeltaTime();
                if (timer <= 0f)
                {
                    InternalCalls.LoadScene("Assets/Scenes/GameLevel.insight");
                }
            }

            // draw text
            //InternalCalls.ButtonRenderText(button_entity, 0.4f, 0.54f, 35f, (1f, 1f, 1f));

        }
        
        static public void CleanUp(){

        }

    }
}
