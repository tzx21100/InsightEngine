using System;
using System.Runtime.CompilerServices;

namespace IS
{
    class PuzzleCamera
    {
        static public void Init()
        {

        }

        static public void Update()
        {
            CameraScript.CameraTargetZoom(0.5f, 1f);
        }
        
        static public void CleanUp()
        {

        }

    }
}
