using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.ComponentModel;
using System.Diagnostics;
using System.Reflection.Metadata;

namespace IS
{


    class BackGroundFollowPlayer
    {

        static SimpleImage bg_image;

        static public void Init(){
            bg_image = InternalCalls.GetSpriteImage("placeholder_background.png");


        }

        static public void Update(){
            // Target position (Player's position)
            Vector2D targetPos = PlayerScript.player_pos;

            // Current position of the object
            Vector2D currentPos = new Vector2D(InternalCalls.GetTransformPosition().x,InternalCalls.GetTransformPosition().y);

            // Interpolation speed (adjust as needed)
            float speed = 25f; // Units per second

            // Calculate the interpolated position
            Vector2D interpolatedPos = Vector2D.Lerp(currentPos, targetPos, speed * InternalCalls.GetDeltaTime());

            // Set the new position
            InternalCalls.TransformSetPosition(interpolatedPos.x, interpolatedPos.y);
        }
        
        static public void CleanUp(){
            InternalCalls.FreeSpriteImage(bg_image);
        }

    }
}
