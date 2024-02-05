using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
namespace IS
{


    class CaveBackGround
    {


        static SimpleImage bg_image1;
        static SimpleImage bg_image2;
        static SimpleImage bg_image3;
        static SimpleImage bg_image4;
        static SimpleImage bg_image5;

        static private Vector2D bg1_pos=new Vector2D(0,0);
        static private Vector2D bg2_pos=new Vector2D(0,0);
        static private Vector2D bg3_pos=new Vector2D(0,0);
        static private Vector2D bg4_pos=new Vector2D(0,0);
        static private Vector2D bg5_pos=new Vector2D(0,0);
        static private Vector2D bg_scale=new Vector2D(5850*0.8f,1080*1.5f);


        static public void Init(){
            bg_image1 = InternalCalls.GetSpriteImage("transparent.png");
            bg_image2 = InternalCalls.GetSpriteImage("Env_cave_foreground.png");
            bg_image3 = InternalCalls.GetSpriteImage("Env_cave_midground.png");
            bg_image4 = InternalCalls.GetSpriteImage("Env_cave_2ndbackground.png");
            bg_image5 = InternalCalls.GetSpriteImage("Env_cave_background.png");


        }


        static public void Update()
        {
            float playerOffsetX = PlayerScript.player_pos.x;
            float playerOffsetY = PlayerScript.player_pos.y;

            // Set offset ratios for each layer (farther layers have smaller ratios)
            // Adjust the ratios to control the movement sensitivity of each layer
            float[] horizontalOffsetRatios = new float[] { 0.02f, 0.05f, 0.1f, 0.01f, 0.0f };
            float[] verticalOffsetRatios = new float[] { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

            for (int i = 0; i < horizontalOffsetRatios.Length; i++)
            {
                // Calculate the offset for this layer
                float layerOffsetX = playerOffsetX * horizontalOffsetRatios[i];
                float layerOffsetY = playerOffsetY * verticalOffsetRatios[i];

                // Get the initial position of this layer
                Vector2D initialPos = GetCurrentPositionForLayer(i);
                bg_scale = new Vector2D((5850 * 0.8f) /CameraScript.camera_zoom, (1080 * 1.1f) / CameraScript.camera_zoom);
                // Update the position
                Vector2D newPos = new Vector2D(PlayerScript.camera_pos.x + initialPos.x - layerOffsetX,
                                               PlayerScript.camera_pos.y + initialPos.y - layerOffsetY);

                // Draw the layer at its new position
                int layer = 0;
                InternalCalls.DrawImageAt(newPos.ToSimpleVector2D(), 0, bg_scale.ToSimpleVector2D(), GetBackgroundImage(i), 1f, layer);
            }

            //InternalCalls.GameSpawnParticle(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y, "ParticleLeaves.txt");

            // Update the camera position
            //InternalCalls.TransformSetPosition(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y);

            
            if (PlayerScript.camera_pos.y > 3500)
            {
                InternalCalls.DrawSquare(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y, 7000, 7000, 0, 0, 0,1+( (PlayerScript.camera_pos.y-4500) /1000), 4);
                if (PlayerScript.player_pos.y > 4600)
                {
                    InternalCalls.ChangeLightType(3);
                    InternalCalls.LoadScene("Assets/Scenes/GameLevel.insight");
                }
            }

                InternalCalls.DrawSquare(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y, 7000, 7000, 0, 0, 0, 0.8f, 0);
/*                InternalCalls.DrawSquare(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y, 7000, 7000, 0, 0, 0, 0.3f, 10);*/
            



        }



        static SimpleImage GetBackgroundImage(int index)
        {
            switch (index)
            {
                case 0: return bg_image5;
                case 1: return bg_image4;
                case 2: return bg_image3;
                case 3: return bg_image2;
                case 4: return bg_image1;
                default: return bg_image5;
            }
        }

        static Vector2D GetCurrentPositionForLayer(int index)
        {
            switch (index)
            {
                case 0: return bg1_pos;
                case 1: return bg2_pos;
                case 2: return bg3_pos;
                case 3: return bg4_pos;
                case 4: return bg5_pos;
                default: return bg1_pos;
            }

        }



        static public void CleanUp(){

        }

    }
}
