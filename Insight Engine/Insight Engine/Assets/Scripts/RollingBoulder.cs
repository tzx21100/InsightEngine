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

            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if (array.FindIndex(PlayerScript.entity_feet) != -1)
            {
                InternalCalls.TransformSetPositionEntity(PlayerScript.player_pos.x - 10f, PlayerScript.player_pos.y-9.8f, PlayerScript.PLAYER_ID);
                //InternalCalls.TransformSetPositionEntity(PlayerScript.player_pos.x - 10f, PlayerScript.player_pos.y - 9.8f, PlayerScript.entity_feet);
                //InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotation(),0f, PlayerScript.entity_feet);
                //PlayerScript.prev_rotation = InternalCalls.GetTransformRotation();

            }

        }
        
        static public void CleanUp(){

        }

    }
}
