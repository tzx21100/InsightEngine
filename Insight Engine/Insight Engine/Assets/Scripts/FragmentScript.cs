using System.Runtime.CompilerServices;
namespace IS
{
    class FragmentScript
    {

        static public void Init(){

        }

        static public void Update(){
            int entity = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity,PlayerScript.PLAYER_ID)) {
                if (!PlayerScript.Reward_DoubleJump) { PlayerScript.Reward_DoubleJump = true; } else
                {
                    if (!PlayerScript.Reward_WallClimb) {
                        PlayerScript.Reward_WallClimb = true;
                    }
                    else
                    {
                        if (!PlayerScript.Reward_Dash) {
                            PlayerScript.Reward_Dash = true;
                        }
                    }

                }
                InternalCalls.DestroyEntity(entity);
            }


        }
        
        static public void CleanUp(){

        }

    }
}
