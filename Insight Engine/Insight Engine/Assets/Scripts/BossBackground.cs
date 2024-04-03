using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
namespace IS
{


    class BossBackground
    {


        //static SimpleImage bg_image1;
        static SimpleImage bg_image2;
        static SimpleImage bg_image3;
        static SimpleImage bg_image4;
        static SimpleImage bg_image5;

        static private Vector2D bg1_pos = new Vector2D(0, 0);
        static private Vector2D bg2_pos = new Vector2D(0, 0);
        static private Vector2D bg3_pos = new Vector2D(0, 0);
        static private Vector2D bg4_pos = new Vector2D(0, 0);
        static private Vector2D bg5_pos = new Vector2D(0, 0);
        static private Vector2D bg_scale = new Vector2D(2048 * 2.5f, 1080 * 2.5f);


        static private float fade_out_timer = 1.7f;

        static public void Init()
        {

            fade_out_timer = 1.7f;
            //bg_image1 = InternalCalls.GetSpriteImage("transparent.png");
            bg_image2 = InternalCalls.GetSpriteImage("BossLevel_Foreground.png");
            bg_image3 = InternalCalls.GetSpriteImage("BossLevel_Midground.png");
            bg_image4 = InternalCalls.GetSpriteImage("GlitchTree_Prop.png");
            bg_image5 = InternalCalls.GetSpriteImage("BossLevel_Background.png");
            CameraScript.CameraTargetZoom(0.7f, 0.5f);
            InternalCalls.AudioStopAllSounds();
            InternalCalls.AudioPlayMusicSFX("01.Daytime_Forest_Field_Sun_Loop.wav", 0.4f);

        }


        static public void Update()
        {
            bg_scale = new Vector2D(2048*1.2f /CameraScript.camera_zoom, 1080*1.2f / CameraScript.camera_zoom);

            //InternalCalls.DrawImageAt(PlayerScript.camera_pos.ToSimpleVector2D() ,0, bg_scale.ToSimpleVector2D(), bg_image5, 0);
            // Calculate the player's offset
            float playerOffsetX = PlayerScript.player_pos.x;
            float playerOffsetY = PlayerScript.player_pos.y;

            // Set offset ratios for each layer (farther layers have smaller ratios)
            float[] offsetRatios = new float[] { 0.01f, 0.02f, 0.03f, 0.04f, 0.2f };

            // Adjust the position of each background layer based on the player's offset
            for (int i = 0; i < offsetRatios.Length; i++)
            {
                if (i == 1 ) continue;

                // Calculate the offset for this layer
                float layerOffsetX = playerOffsetX * offsetRatios[i];
                float layerOffsetY = playerOffsetY * offsetRatios[i];

                // Get the initial position of this layer
                Vector2D initialPos = GetCurrentPositionForLayer(i);

                // Update the position
                Vector2D newPos = new Vector2D(CameraScript.camera_pos.x + initialPos.x - layerOffsetX, CameraScript.camera_pos.y + initialPos.y - layerOffsetY);

                // Draw the layer at its new position
                InternalCalls.DrawImageAt(newPos.ToSimpleVector2D(), 0, bg_scale.ToSimpleVector2D(), GetBackgroundImage(i), 1f, 0);
            }

            //InternalCalls.GameSpawnParticle(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y, "ParticleLeaves.txt");

            // Update the camera position
            InternalCalls.TransformSetPosition(CameraScript.camera_pos.x, CameraScript.camera_pos.y);

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 7000, 7000, 0, 0, 0, 0.2f, 0);

            //InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 7000, 7000, 0, 0, 0, fade_out_timer, InternalCalls.GetTopLayer());


        }



        static SimpleImage GetBackgroundImage(int index)
        {
            switch (index)
            {
                //case 0: return bg_image5;
                //case 1: return SimpleImage;
                case 2: return bg_image4;
                case 3: return bg_image3;
                case 4: return bg_image2;
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



        static public void CleanUp()
        {

        }

    }
}
