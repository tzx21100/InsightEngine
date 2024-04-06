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
        static string text0 = "Which will you choose? The red fragment to restore the world and your loved ones. Or";
        static string text1 = "The blue fragment, which gives you infinite power, infinite possibilities to recreate the world as you desire";



        static public void Init()
        {
            CameraScript.CameraTargetZoom(0.4f, 1f);
            fade_in_timer = 2f;

            int BLUE_ID = InternalCalls.CreateEntityPrefab("FinalBlueFragment");
            int RED_ID = InternalCalls.CreateEntityPrefab("FinalRedFragment");

            InternalCalls.TransformSetPositionEntity(-1800, -700, RED_ID);
            InternalCalls.TransformSetPositionEntity(1800, -700, BLUE_ID);
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



        }

        static public void CleanUp()
        {

        }

        static private void PanToRed()
        {

        }

        static private void PanToBlue()
        {

        }

    }
}
