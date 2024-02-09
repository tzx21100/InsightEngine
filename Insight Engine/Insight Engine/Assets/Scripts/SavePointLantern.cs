/*!
 * \file SavePointLantern.cs
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This source file is meant for the player movement and camera.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

using System.Diagnostics;
using System.Runtime.CompilerServices;
namespace IS
{
    class SavePointLantern
    {

        static private SimpleImage lantern_off;
        static private SimpleImage lantern_on;
        static public bool save_point=false;
        static private int my_ID;
        static private Vector2D my_position= new Vector2D(0,0);

        static public void Init(){
            PlayerScript.SAVE_POINT_ID = -1;
            save_point = false;
            my_ID=InternalCalls.GetCurrentEntityID();
            lantern_off = InternalCalls.GetSpriteImage("Prop_Lantern_NoLights.png");
            lantern_on = InternalCalls.GetSpriteImage("Prop_Lantern.png");
            InternalCalls.SetSpriteImage(lantern_off);
            my_position.x = InternalCalls.GetTransformPosition().x;
            my_position.y = InternalCalls.GetTransformPosition().y;
            InternalCalls.ColliderNone(my_ID);
        }


        static MyRandom rand = new MyRandom(129248189);
        static public void Update(){
            my_ID = InternalCalls.GetCurrentEntityID();
            my_position.x = InternalCalls.GetTransformPosition().x;
            my_position.y = InternalCalls.GetTransformPosition().y;
            if (InternalCalls.EntityCheckCollide(my_ID)) {

                if (InternalCalls.GetCollidingEntityCheck(my_ID, PlayerScript.PLAYER_ID))
                {
                    save_point = true;
                    //PlayerScript.Health = PlayerScript.Max_Health; // not allow player to heal at save point
                    PlayerScript.respawn_x = my_position.x;
                    PlayerScript.respawn_y = my_position.y;
                    InternalCalls.SetSpriteImage(lantern_on);
                    PlayerScript.SAVE_POINT_ID = my_ID;
                }

                

                
            
            }

            if(PlayerScript.SAVE_POINT_ID!=my_ID)
            {
                InternalCalls.AudioEmitterEnableEntity(false, my_ID);
                save_point =false;
                InternalCalls.SetSpriteImage(lantern_off);
                InternalCalls.SetLightToggleEntity(my_ID, false);
            }
            else{
                save_point = true;
            }

            if(save_point)
            {

                if (Vector2D.Distance(my_position, PlayerScript.player_pos) > 500f)
                {
                    return;
                }
                InternalCalls.AudioEmitterEnableEntity(true, my_ID);
                InternalCalls.SetLightToggleEntity(my_ID, true);
                InternalCalls.SetSpriteImage(lantern_on);
                float direction=rand.NextFloat() *360;
                float ran=rand.NextFloat();
                float ran2=rand.NextFloat();
                if (ran2 < 0.3f)
                {
                    InternalCalls.GameSpawnParticleExtra(my_position.x, my_position.y + 80f, direction, ran * 10, ran * 2, 0.5f*ran, 0f, 2f * ran, ran * 200, "Particle2.txt");
                }
                else if (ran2 <0.6f) {
                    InternalCalls.GameSpawnParticleExtra(my_position.x, my_position.y + 70f, direction, ran * 10, ran * 2, 0.5f * ran, 0f, 2f * ran, ran * 200, "Particle Test.txt");
                }
                else if (ran2 <0.9f)
                {
                    InternalCalls.GameSpawnParticleExtra(my_position.x, my_position.y + 80f, direction, ran * 10, ran * 2, 0.5f * ran, 0f, 2f * ran, ran * 200, "Particle3.txt");
                }

            }

            
        }
        
        static public void CleanUp(){
/*            InternalCalls.FreeSpriteImage(lantern_on);
            InternalCalls.FreeSpriteImage(lantern_off);*/
        }

    }
}
