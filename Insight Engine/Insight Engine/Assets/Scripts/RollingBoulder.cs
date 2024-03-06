using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace IS
{
    class RollingBoulder
    {
        static float angle = 0f;
        static float angular_velocity = 10f;
        static float linear_veloctiy = 10f;

        //determine the change in angle each game loop << CHANGE THIS VALUE FOR SPEED
        static float angle_change = 0.3f;

        //set falling angle
        static float min_angle = -75f;
        static float max_angle = 75f;
        
        static public void Init(){
            angle = 0f;
        }

        static public void Update(){

            //angular velocity = change in angle/ change in time
            angular_velocity = angle_change / InternalCalls.GetDeltaTime();

            float radius = InternalCalls.GetTransformScaling().x / 2f;
            // linear velocity = r * angular velocity
            linear_veloctiy = radius * angular_velocity;

            //shift the angle each frame by rotation
            angle += angle_change;

            //Console.WriteLine(angle);
            InternalCalls.TransformSetRotation(angle, 0f);

            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if (array.FindIndex(PlayerScript.entity_feet) != -1 || array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                PushToNormalDirection();
                //InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotation(),0f, PlayerScript.entity_feet);
                // PlayerScript.prev_rotation = InternalCalls.GetTransformRotation();
                PlayerScript.isFirstGrounded = false;
/*                InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotationEntity(PlayerScript.PLAYER_ID) + angle_change, 0f, PlayerScript.PLAYER_ID);
                PlayerScript.prev_rotation = InternalCalls.GetTransformRotationEntity(PlayerScript.PLAYER_ID);*/
            }


        }
        
        static public void CleanUp(){

        }


        static private void PushToNormalDirection()
        {
            // Get the line from the 2 points
            SimpleVector2D currentPoint = InternalCalls.GetTransformPosition();
            // Get the player position
            SimpleVector2D playerPos = InternalCalls.GetTransformPositionEntity(PlayerScript.PLAYER_ID);

            //Line vector
            Vector2D line_vector = new Vector2D(currentPoint.x - playerPos.x,currentPoint.y-playerPos.y);

            //Calclualted line_direction to the left assuming rotating left
            Vector2D push_direction = new Vector2D(line_vector.y, -line_vector.x);

            // get the angle of the normal vector
            double angle = Math.Atan((double)(push_direction.y / push_direction.x));
            angle = CustomMath.ToDegrees((float)angle);
            Console.WriteLine(angle);
            if(angle >= min_angle && angle <= max_angle)
            {
                InternalCalls.TransformSetRotationEntity((float)angle, 0f, PlayerScript.PLAYER_ID);
                PlayerScript.FloorCheckerUpdate();
                PlayerScript.prev_rotation = (float)angle;
            }


            // get the push speed
            push_direction =push_direction.Normalize();
            push_direction = push_direction.Multiply(linear_veloctiy/45 *InternalCalls.GetDeltaTime()); //account for SI unit meters


            // Set Direction of player
            Vector2D playerNewPosition = new Vector2D(playerPos.x + push_direction.x, playerPos.y + push_direction.y);
            InternalCalls.TransformSetPositionEntity(playerNewPosition.x , playerNewPosition.y, PlayerScript.PLAYER_ID);





/*            InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotation(),0f, PlayerScript.PLAYER_ID);
            PlayerScript.prev_rotation = InternalCalls.GetTransformRotation();*/

        }

    }
}
