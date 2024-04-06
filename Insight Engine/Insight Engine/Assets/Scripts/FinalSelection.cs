using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
namespace IS
{

    // fade in white bg

    class FinalSelection
    {

        static private float fade_in_timer = 2f;

        static public void Init()
        {
            CameraScript.CameraTargetZoom(0.4f, 1f);
            fade_in_timer = 2f;
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

    }
}
