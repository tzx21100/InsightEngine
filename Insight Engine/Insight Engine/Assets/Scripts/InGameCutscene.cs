using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
namespace IS
{
    class InGameCutscene
    {
        static string text0 = "Hello there Cipher. I've been expecting you.";
        static string text1 = ""; //yiming voice line (you try to speak, however you find yourself baffled, unable to muster even the slightest of sounds.)
        static string text2 = "Try as you will, as you can tell however, your ability to speak is hindered.";
        static string text3 = "Listen closely, your world as you know it is slowly being deleted.";
        static string text4 = "A being of another dimension has unfortunately went mad. You must collect FRAGMENTS to save it.";
        static string text5 = "The RED ones restore the world. The BLUE ones give you the ability."; // pan here
        static string text6 = "Though I wish I could explain further, there is no helping it. You must GO.";

        static float switch_view_timer = 2.5f;
        static bool play_audio = true;
        static bool textshow = true;
        static public void Init(){
            textshow = true;
            play_audio = true;
            switch_view_timer = 2.5f;
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
                TextBox.AddTextLines(text5);
                TextBox.AddTextLines(text6);

                Vector2D pan_position = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);
                CameraScript.CameraPanTo(pan_position, 1f);
            
            }

            if (TextBox.PAGE_NUMBER == 1) { //play yiming VA
                if(play_audio)
                {
                    InternalCalls.AudioPlaySound("Narrator_1.wav", false, 0.4f);
                    play_audio = false;
                }
                
                
            }

            if(TextBox.PAGE_NUMBER == 5) {
                CameraScript.CameraPanToTime(new Vector2D(15658.83f, 1579.83f), 1f, 2.5f);
                switch_view_timer -= InternalCalls.GetDeltaTime();
                if (switch_view_timer <= 0)
                {
                    CameraScript.CameraPanTo(new Vector2D(17541.90f, 335.983f), 1f);
                }
            }

            if (TextBox.PAGE_NUMBER == 6)
            {
                Vector2D pan_position = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);
                CameraScript.CameraPanTo(pan_position, 1f);
            }





            //end of camera pan and dialogue
            if (textshow==false && TextBox.isVisible == false)
            {
                InternalCalls.AudioPlaySound("Narrator_2.wav", false, 0.4f);
                CameraScript.StopCameraPan();

                //yiming VA plays "Left without a choice, you can only do as the strange man says"

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
