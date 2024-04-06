using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
namespace IS
{

    

    class FinalSelection
    {
        // ids
        static private int BLUE_ID;
        static private int RED_ID;

        // fade in white bg
        static private float fade_in_timer = 2f;

        // text
        static string text0 = "The Boss has gone, you have left with two choices, which will you choose? ";
        static string text1 = "The red fragment to restore the world and your loved ones. Or";
        static string text2 = "The blue fragment, which gives you infinite power, infinite possibilities to recreate the world as you desire";

        static bool textshow = false;

        // timer
        static private float zoom_out_timer = 1.5f;

        static public void Init()
        {
            CameraScript.CameraTargetZoom(0.4f, 1f);
            fade_in_timer = 2f;

            BLUE_ID = InternalCalls.CreateEntityPrefab("FinalBlueFragment");
            RED_ID = InternalCalls.CreateEntityPrefab("FinalRedFragment");

            InternalCalls.TransformSetPositionEntity(-1800, -700, RED_ID);
            InternalCalls.TransformSetPositionEntity(1800, -700, BLUE_ID);

            // text
            textshow = false;

            zoom_out_timer = 1.5f;
        }

        static public void Update()
        {
            if (fade_in_timer > 0f)
            {
                fade_in_timer -= InternalCalls.GetDeltaTime();
                CameraScript.CameraTargetZoom(0.4f, 1f);
                InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 7000, 7000, 1, 1, 1, fade_in_timer, InternalCalls.GetTopLayer());

                return;
            }

            if (!textshow) {

                textshow = true;
                TextBox.CreateTextBox(text0);
                TextBox.AddTextLines(text1);
                TextBox.AddTextLines(text2);
            }

            if (textshow)
            {
                CameraPanToFragment();
            }
        }

        static public void CleanUp()
        {

        }

        static private void CameraPanToFragment()
        {
            //Console.WriteLine(TextBox.PAGE_NUMBER);
            if (TextBox.PAGE_NUMBER == 1)
            {
                CameraScript.CameraTargetZoom(0.6f, 0.3f);
                Vector2D red_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(RED_ID));
                CameraScript.CameraPanTo(red_pos, 1.6f);
            }
            else if (TextBox.PAGE_NUMBER == 2)
            {
                CameraScript.CameraTargetZoom(0.6f, 0.3f);
                Vector2D blue_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(BLUE_ID));
                CameraScript.CameraPanTo(blue_pos, 1.6f);
            }

            if (TextBox.isVisible == false)
            {
                CameraScript.CameraPanTo(PlayerScript.player_pos, 1.5f);
                CameraScript.CameraTargetZoom(0.4f, 2f);

                if (zoom_out_timer > 0f)
                {
                    zoom_out_timer -= InternalCalls.GetDeltaTime();
                }
                else
                {
                    CameraScript.StopCameraPan();
                }
                
            }
        }

        static private void PanToRed()
        {

        }

        static private void PanToBlue()
        {

        }

    }
}
