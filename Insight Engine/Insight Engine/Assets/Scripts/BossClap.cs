

using System.Runtime.CompilerServices;
namespace IS
{
    class BossClap
    {
        static private int LEFT_HAND_ID;
        static private int RIGHT_HAND_ID;

        static private Vector2D clapping_pos = new Vector2D(0f, 0f);
        static private Vector2D clapping_scaling = new Vector2D(500f, 500f);
        static private Vector2D clapping_vel = new Vector2D(100f, 100f);
        static private Vector2D left_hand_pos = new Vector2D(0f, 0f);
        static private Vector2D right_hand_pos = new Vector2D(0f, 0f);
        static private float speed = 15f;
        static private bool is_two_hand_clapping = false;
        static private bool is_one_hand_waiting = false;

        static private float resting_timer_set = 0.5f;
        static private float resting_timer = 0.5f;

        private enum ClapAttackTypes : int
        {
            TwoHand = 0,
            OneHandWaiting = 1
        }

        static private ClapAttackTypes attack_type = 0;

        static public void Init()
        {
            LEFT_HAND_ID = InternalCalls.GetCurrentEntityID();
            RIGHT_HAND_ID = InternalCalls.CreateEntityPrefab("BossRightHand");

            // init scale and pos
            InternalCalls.TransformSetPositionEntity(-999999f, -999999f, LEFT_HAND_ID);
            InternalCalls.TransformSetPositionEntity(-999999f, -999999f, RIGHT_HAND_ID);

            InternalCalls.TransformSetScaleEntity(clapping_scaling.x, clapping_scaling.y, LEFT_HAND_ID);
            InternalCalls.TransformSetScaleEntity(clapping_scaling.x * 2f, clapping_scaling.y * 2f, RIGHT_HAND_ID);
        }

        static public void Update()
        {
            RandomAttackType();
            HandClapFSM();
        }

        static public void CleanUp()
        {

        }

        static private void HandClapFSM()
        {
            switch (attack_type)
            {
                case ClapAttackTypes.TwoHand:
                    TwoHandClap();
                    break;
                case ClapAttackTypes.OneHandWaiting:
                    OneHandWaitingForClapping();
                    break;
            }
        }

        static private void RandomAttackType() 
        { 
            MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
            uint random = rnd.Next(0,5); // random from 0 to 4
            //random = 2;
            if (random == 0)
            {
                attack_type = ClapAttackTypes.TwoHand;
                return;
            }
            if (random == 1)
            {
                attack_type = ClapAttackTypes.OneHandWaiting;
            }
        }

        static private void TwoHandClap()
        {

            if (!is_two_hand_clapping) // init clapping position, left and right hand spawn position
            {
                clapping_pos = PlayerScript.player_pos;

                Vector2D offset = new Vector2D(2000f, 500f);

                left_hand_pos = new Vector2D(clapping_pos.x - offset.x, clapping_pos.y + offset.y);
                right_hand_pos = new Vector2D(clapping_pos.x + offset.x, clapping_pos.y + offset.y);
            }

            Vector2D left_hand_direction = new Vector2D(clapping_pos.x - left_hand_pos.x, clapping_pos.y - left_hand_pos.y);
            left_hand_direction = left_hand_direction.Normalize();

            Vector2D right_hand_direction = new Vector2D(clapping_pos.x - right_hand_pos.x, clapping_pos.y - right_hand_pos.y);
            right_hand_direction = right_hand_direction.Normalize();

            left_hand_pos.x += left_hand_direction.x * speed;
            left_hand_pos.y += left_hand_direction.y * speed;
            right_hand_pos.x += right_hand_direction.x * speed;
            right_hand_pos.y += right_hand_direction.y * speed;

            InternalCalls.TransformSetPositionEntity(left_hand_pos.x, left_hand_pos.y, LEFT_HAND_ID);
            InternalCalls.TransformSetPositionEntity(right_hand_pos.x, right_hand_pos.y, RIGHT_HAND_ID);

            // if one of the hands collides with player, it will push the player to the center

            // if two hand collide with player, damage player, render particles and disappear
        }

        static private void OneHandWaitingForClapping()
        {

        }

    }
}
