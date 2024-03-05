using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace IS
{
    class RollingBoulder
    {
        static float angle = 0f;
        static public void Init(){
            angle = 0f;
        }

        static public void Update(){
            angle += 10f * InternalCalls.GetDeltaTime();
            //Console.WriteLine(angle);
            InternalCalls.TransformSetRotation(angle, 10f);
        }
        
        static public void CleanUp(){

        }

    }
}
