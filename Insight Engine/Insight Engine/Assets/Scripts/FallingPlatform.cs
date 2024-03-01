using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class FallingPlatform
    {
        private static int mFallingPlatform;
        private static bool mStartFalling;

        static public void Init()
        {
            mFallingPlatform = InternalCalls.GetCurrentEntityID();
            Console.WriteLine(mFallingPlatform);
            mStartFalling = false;
        }

        static public void Update()
        {
            if (!mStartFalling)
            {
                int player = PlayerScript.PLAYER_ID;
                if (InternalCalls.GetCollidingEntityCheck(mFallingPlatform, player))
                {
                    Console.WriteLine("Contact!");
                    //InternalCalls.TransformSetRotation(88f, 0f);
                    InternalCalls.RigidBodySetBodyTypeEntity(1, mFallingPlatform);
                    mStartFalling = true;
                }
            }
            if (mStartFalling)
            {
                //InternalCalls.SetGravityScale(InternalCalls.GetTransformRotation() > 0f ? 5f : 1f);
                //if (InternalCalls.GetTransformRotation() > 0f)
                //{
                //    InternalCalls.SetGravityScale(5f);
                //}
                //else
                //{
                //    InternalCalls.SetGravityScale(1f);
                //}
            }
        }
        
        static public void CleanUp()
        {

        }

    }
}
