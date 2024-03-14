using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class BossChase
    {
        static float chase_speed = 9f;
        static Vector2D current_pos = new Vector2D(0,0);

        static public void Init()
        {

        }

        static public void Update()
        {
            if (PauseButtonScript.paused) { return; }

            current_pos = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);

            Vector2D direction = new Vector2D(InternalCalls.GetTransformPositionEntity(PlayerScript.PLAYER_ID).x-current_pos.x,
                InternalCalls.GetTransformPositionEntity(PlayerScript.PLAYER_ID).y - current_pos.y);
            direction= direction.Normalize();
            InternalCalls.TransformSetPosition(current_pos.x+direction.x * chase_speed,current_pos.y+ direction.y * chase_speed);

            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                InternalCalls.LoadScene("Assets/Scenes/ChaseLevel.insight");
            }

            if(PlayerScript.isDead)
            {
                PlayerScript.isDead = false;
                InternalCalls.LoadScene("Assets/Scenes/ChaseLevel.insight");
            }


            }

        static public void CleanUp()
        {
        }

    }
}