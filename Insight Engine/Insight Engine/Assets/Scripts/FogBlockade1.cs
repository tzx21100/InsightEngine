using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class FogBlockade1
    {

       static public Vector2D current_pos =new Vector2D(0,0);
        static private float width;
        static private float height;

        static public void Init()
        {
            current_pos.x = InternalCalls.GetTransformPosition().x; current_pos.y = InternalCalls.GetTransformPosition().y;
            width = InternalCalls.GetTransformScaling().x;
            height= InternalCalls.GetTransformScaling().y;
        }

        static public void Update()
        {

            FogCover.fog_position = current_pos;
            FogCover.fog_height = height;
            FogCover.fog_width = width;
            FogCover.fog_density = 14f;
            FogCover.fog_size = 300f;
            FogCover.fog_timer_set = 0.02f;
            FogCover.is_random_life_time = true;
            FogCover.SpawnFog();

            if (PlayerScript.collection_count >=1)
            {
                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());
            }
        }


        static public void CleanUp(){

        }


    }
}
