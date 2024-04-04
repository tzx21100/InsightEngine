using System.Runtime.CompilerServices;
namespace IS
{
    class BossDeath
    {

        static private float death_animation_timer = 2f;
        static SimpleImage boss_death_image;
        static public void Init(){
            death_animation_timer = 2f;
            boss_death_image = InternalCalls.GetSpriteImage("BossDeath 6R4C.png");
        }

        static public void Update(){
            
            death_animation_timer-=InternalCalls.GetDeltaTime();

            //shake camera
            CameraScript.CameraShake(2f);
            CameraScript.camera_shake_duration = 0.2f;
            CameraScript.camera_shake_intensity = 1f;

            if(death_animation_timer < 0) 
            {
                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());


                // create the red n blue fragments
                int blue=InternalCalls.CreateEntityPrefab("FinalBlueFragment");
                int red=InternalCalls.CreateEntityPrefab("FinalRedFragment");
                //-1800 1800 y -700

                //setting positions
                InternalCalls.TransformSetPositionEntity(-1800, -700, red);
                InternalCalls.TransformSetPositionEntity(1800, -700, blue);
                InternalCalls.TransformSetPositionEntity(0, -700, PlayerScript.PLAYER_ID);


            
            }

        }
        
        static public void CleanUp(){

        }

    }
}
