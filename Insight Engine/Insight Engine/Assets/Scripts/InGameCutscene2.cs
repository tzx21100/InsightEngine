using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
namespace IS
{
    class InGameCutscene2
    {
        static string text0 = "Hello Cipher, it has been a while. I am so proud of your progress.";
        static string text1 = "It looks like things are progressing well, little by little, the world is being restored.";
        static string text2 = "Just a couple more fragments, and our world's back in one piece.";
        static string text3 = "Wait... something's not right."; //camera shake
        static string text4 = "FLEE CIPHER! HE HAS FOUND US!"; // pm appears and smacks after this

        static private int entity_boss;
        static private bool triggered;

        static bool textshow = true;
        static public void Init(){
            triggered = false;
            textshow = true;
            entity_boss = InternalCalls.CreateEntityPrefab("BossSmash");
            InternalCalls.SetSpriteAlphaEntity(0, entity_boss);
            InternalCalls.TransformSetScaleEntity(0,0, entity_boss);
            InternalCalls.TransformSetPositionEntity(2485, 3600, entity_boss);
            

        }

        static public void Update(){

            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1 && textshow) 
            {
                textshow = false;
                TextBox.CreateTextBox(text0);
                TextBox.AddTextLines(text1);
                TextBox.AddTextLines(text2);
                TextBox.AddTextLines(text3);
                TextBox.AddTextLines(text4);


                Vector2D pan_position = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);
                CameraScript.CameraPanTo(pan_position, 1f);
            
            }

            if (TextBox.PAGE_NUMBER == 3) { //play yiming VA
                CameraScript.camera_shake_intensity = 0.4f;
                CameraScript.camera_shake_duration = 0.2f;
                CameraScript.CameraShake(0.3f);
                                          }

            if(TextBox.PAGE_NUMBER == 4) {


                triggered = true;

                CameraScript.camera_shake_intensity = 0.8f;
                CameraScript.camera_shake_duration = 0.2f;
                CameraScript.CameraShake(0.3f);
                CameraScript.CameraPanToTime(new Vector2D(2485, 3600), 1f, 2.5f);
            }

            if (!textshow) InGameCutscene.drawBigNPC();

            if (triggered)
            {
                BossCutScenePush.triggered = true;
            }


            //end of camera pan and dialogue
            if (textshow==false && TextBox.isVisible == false)
            {
                CameraScript.StopCameraPan();
                BossCutScenePush.can_move = true;

                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());

                for(int i = 0;i<72;i++)
                {
                    int col = (int)(InternalCalls.GetRandomFloat() * 9);
                    float direction =45+ InternalCalls.GetRandomFloat() * 90;
                    float size = InternalCalls.GetRandomFloat() * 42f;
                    float speed = InternalCalls.GetRandomFloat() * 800f;
                    float lifespan = InternalCalls.GetRandomFloat() * 3f;
                    InternalCalls.GameSpawnParticleExtraFrames(InternalCalls.GetTransformPosition().x-50+ InternalCalls.GetRandomFloat() * 100f, 
                        InternalCalls.GetTransformPosition().y - 100 + InternalCalls.GetRandomFloat() * 200f,
                    direction, size, 0f, 1f,
                    0f, lifespan, speed, "ParticleDust.txt"
                    , col, 0);
                }

            }

        }
        
        static public void CleanUp(){

        }

    }
}
