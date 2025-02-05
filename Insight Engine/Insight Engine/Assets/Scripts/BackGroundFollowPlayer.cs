using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
namespace IS
{


    class BackGroundFollowPlayer
    {


        static float leaves_timer=3f;
        static int leaves_amount=50;
        static float leaves_timer_set=3f;

        //static SimpleImage bg_image1;
        static SimpleImage bg_image2;
        static SimpleImage bg_image3;
        //static SimpleImage bg_image4;
        static SimpleImage bg_image5;

        static private Vector2D bg1_pos=new Vector2D(0,0);
        static private Vector2D bg2_pos=new Vector2D(0,0);
        static private Vector2D bg3_pos=new Vector2D(0,0);
        static private Vector2D bg4_pos=new Vector2D(0,0);
        static private Vector2D bg5_pos=new Vector2D(0,0);
        static private Vector2D bg_scale=new Vector2D(4096*2.5f,756*2.5f);


        static private float fade_out_timer = 1.7f;

        static public void Init(){

            fade_out_timer = 1.7f;
            //bg_image1 = InternalCalls.GetSpriteImage("transparent.png");
            bg_image2 = InternalCalls.GetSpriteImage("1st.png");
            bg_image3 = InternalCalls.GetSpriteImage("3rd.png");
            // bg_image4 = InternalCalls.GetSpriteImage("transparent.png");
            bg_image5 = InternalCalls.GetSpriteImage("5th.png");
            CameraScript.CameraTargetZoom(0.7f, 0.5f);
            InternalCalls.AudioStopAllSounds();
            InternalCalls.AudioPlayMusicSFX("01.Daytime_Forest_Field_Sun_Loop.wav", 0.4f);

        }


        static public void Update()
        {


            //InternalCalls.DrawImageAt(PlayerScript.camera_pos.ToSimpleVector2D() ,0, bg_scale.ToSimpleVector2D(), bg_image5, 0);
            // Calculate the player's offset
            float playerOffsetX = PlayerScript.player_pos.x;
            float playerOffsetY = PlayerScript.player_pos.y;

            // Set offset ratios for each layer (farther layers have smaller ratios)
            float[] offsetRatios = new float[] { 0.01f, 0.02f, 0.03f, 0.04f, 0.2f };

            // Adjust the position of each background layer based on the player's offset
            for (int i = 0; i < offsetRatios.Length; i++)
            {
                if (i == 1 || i == 4) continue;

                // Calculate the offset for this layer
                float layerOffsetX = playerOffsetX * offsetRatios[i];
                float layerOffsetY = playerOffsetY * offsetRatios[i];

                // Get the initial position of this layer
                Vector2D initialPos = GetCurrentPositionForLayer(i);

                // Update the position
                Vector2D newPos = new Vector2D(CameraScript.camera_pos.x+initialPos.x - layerOffsetX, CameraScript.camera_pos.y + initialPos.y - layerOffsetY);

                // Draw the layer at its new position
                InternalCalls.DrawImageAt(newPos.ToSimpleVector2D(), 0, bg_scale.ToSimpleVector2D(), GetBackgroundImage(i), 1f, 0);
            }

            //InternalCalls.GameSpawnParticle(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y, "ParticleLeaves.txt");

            // Update the camera position
            InternalCalls.TransformSetPosition(CameraScript.camera_pos.x, CameraScript.camera_pos.y);

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 7000, 7000, 0, 0, 0, 0.2f, 0);

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 7000, 7000, 0, 0, 0, fade_out_timer, InternalCalls.GetTopLayer());
            leaves_timer -= InternalCalls.GetDeltaTime();
            if (fade_out_timer > 0)
            {
                fade_out_timer -= InternalCalls.GetDeltaTime() * 0.8f;
            }

            if (fade_out_timer > 1)
            {
                
                InternalCalls.SetLightsToggle(false);
            }
            else
            {
                if (fade_out_timer > 0)
                {
                    InternalCalls.SetLightsToggle(true);
                }
            }
            if(leaves_timer <= 0 )
            {


                for (int i = 0;i<leaves_amount;i++)
                {
                    int col= (int)(InternalCalls.GetRandomFloat()*2);
                    float direction =InternalCalls.GetRandomFloat() * 360;
                    float size = InternalCalls.GetRandomFloat() * 150;
                    float speed = InternalCalls.GetRandomFloat() * 800f;
                    float lifespan = InternalCalls.GetRandomFloat() * 12f;
                    InternalCalls.GameSpawnParticleExtraFramesFull((CameraScript.camera_pos.x - 2000) + (2000 * InternalCalls.GetRandomFloat()), CameraScript.camera_pos.y+2000f,
                        direction, size, 0f, 1f,
                        0f, lifespan, speed, "ParticleLeaves.txt"
                        ,col,0,direction,2,1f,lifespan * CustomMath.Normalize(lifespan-6f) /5f , 3, (1,1,1));

                }

                leaves_timer = leaves_timer_set;
            }


        }



        static SimpleImage GetBackgroundImage(int index)
        {
            switch (index)
            {
                case 0: return bg_image5;
                //case 1: return SimpleImage;
                case 2: return bg_image3;
                case 3: return bg_image2;
                //case 4: return bg_image1;
                default: return bg_image5;
            }
        }

        static Vector2D GetCurrentPositionForLayer(int index)
        {
            switch (index)
            {
                case 0: return bg1_pos;
                //case 1: return;
                case 2: return bg3_pos;
                case 3: return bg4_pos;
                //case 4: return;
                default: return bg1_pos;
            }

        }



        static public void CleanUp(){

        }

    }
}
