using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
namespace IS
{
    class InGameCutscene
    {
        static string text0 = "Hello there Cipher. I have been expecting you.";
        static string text1 = "<You try to speak, however, you find yourself baffled - unable to muster even the slightest of sounds.>"; //yiming voice line (you try to speak, however you find yourself baffled, unable to muster even the slightest of sounds.)
        static string text2 = "I am Archivist X3-R0, your guide in the fragments of a hopeful, broken world.";
        static string text3 = "Listen closely, the very fabric of your reality is unraveling.";
        static string text4 = "A malevolent force from another dimension has descended into madness, threatening to erase your loved ones.";
        static string text4Point1 = "<There's not much time to waste...>";
        static string text4Point2 = "You alone possess the power to gather the fragments needed to salvage our world.";
        static string text5 = "The RED fragments restore the portions of the world. The BLUE ones empower you with an ability."; // pan here
        static string text6 = "You are our last hope for restoring peace and order. Venture forth courageously, and bring back your loved ones.";

        static SimpleImage MCImage = InternalCalls.GetSpriteImage("MC.png");
        static SimpleImage NPCImage = InternalCalls.GetSpriteImage("NPC.png");

        static float switch_view_timer = 2.5f;
        static bool play_audio = true;
        static bool textshow = true;

        static public void drawBigNPC()
        {
            SimpleVector2D position_of_NPC = new SimpleVector2D(CameraScript.camera_pos.x - InternalCalls.GetWindowWidth() / CameraScript.camera_zoom / 3.1f,
                                                                   CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 5.0f);
            float xScale = 1887f / InternalCalls.GetWindowWidth();
            float yScale = 2160.0f / InternalCalls.GetWindowHeight();
            float NPCImageWidth = InternalCalls.GetWindowWidth() * -xScale / CameraScript.camera_zoom / 2.5f;
            float NPCImageHeight = InternalCalls.GetWindowHeight() * yScale / CameraScript.camera_zoom / 2.5f;
            SimpleVector2D image_scale = new SimpleVector2D(NPCImageWidth, NPCImageHeight);
            InternalCalls.DrawImageAt(position_of_NPC, 0, image_scale, NPCImage, 1, InternalCalls.GetTopLayer() - 1);
        }

        static public void drawBigMC()
        {
            SimpleVector2D position_of_MC = new SimpleVector2D(CameraScript.camera_pos.x - InternalCalls.GetWindowWidth() / CameraScript.camera_zoom / 3.1f,
                                                                   CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 5.0f);

            float xScale = 1273f / InternalCalls.GetWindowWidth();
            float yScale = 2031f / InternalCalls.GetWindowHeight();
            float MCImageWidth = InternalCalls.GetWindowWidth() * xScale / CameraScript.camera_zoom / 2.5f;
            float MCImageHeight = InternalCalls.GetWindowHeight() * yScale / CameraScript.camera_zoom / 2.5f;
            SimpleVector2D image_scale = new SimpleVector2D(MCImageWidth, MCImageHeight);
            InternalCalls.DrawImageAt(position_of_MC, 0, image_scale, MCImage, 1, InternalCalls.GetTopLayer() - 1);
        }

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
                TextBox.AddTextLines(text4Point1);
                TextBox.AddTextLines(text4Point2);
                TextBox.AddTextLines(text5);
                TextBox.AddTextLines(text6);

                Vector2D pan_position = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);
                CameraScript.CameraPanTo(pan_position, 1f);
            
            }
            

            if (TextBox.PAGE_NUMBER == 1) { //play yiming VA
                drawBigMC();
                if (play_audio)
                {
                    InternalCalls.AudioPlaySoundSFX("Narrator_1.wav", false, 0.4f);
                    play_audio = false;
                }
                
            }
            else if (TextBox.PAGE_NUMBER == 5)
            {
                drawBigMC();
            }
            else if (!textshow)
            {
                drawBigNPC();
            }
            

            if (TextBox.PAGE_NUMBER == 7) {
                CameraScript.CameraPanToTime(new Vector2D(15658.83f, 1579.83f), 1f, 2.5f);
                switch_view_timer -= InternalCalls.GetDeltaTime();
                if (switch_view_timer <= 0)
                {
                    CameraScript.CameraPanTo(new Vector2D(17541.90f, 335.983f), 1f);
                }
            }

            if (TextBox.PAGE_NUMBER == 8)
            {
                Vector2D pan_position = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);
                CameraScript.CameraPanTo(pan_position, 1f);
            }





            //end of camera pan and dialogue
            if (textshow==false && TextBox.isVisible == false)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_2.wav", false, 0.4f);
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
