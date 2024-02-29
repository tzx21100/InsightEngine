using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace IS
{
    class FallingBoulder
    {
        private const float MAX_INTENSITY  = 5f;
        private const float MIN_INTENSITY  = .5f;
        private const float SHAKE_DURATION = .55f;
        private static Random mRandom = new Random();

        private static Dictionary<int, BoulderHelper> mBoulders = new Dictionary<int, BoulderHelper>();

        static public void Init()
        {
            GetActiveBoulder().InitializeBoulder();
        }

        static public void Update()
        {
            GetActiveBoulder().UpdateBoulder();
        }

        static public void CleanUp()
        {
            // Empty for now.
        }

        static public BoulderHelper GetActiveBoulder()
        {
            int boulder = InternalCalls.GetCurrentEntityID();
            if (!mBoulders.ContainsKey(boulder))
                mBoulders.Add(boulder, new BoulderHelper());

            return mBoulders[boulder];
        }

        internal class BoulderHelper
        {
            private int mActivateRange;
            private bool mIsActivated;
            private bool mIsGrounded;
            private float mShakeIntensity;
            private float mShakeTimer;
            private Vector2D mOriginalPosition = new Vector2D(0f, 0f);

            public void InitializeBoulder()
            {
                mIsActivated = mIsGrounded = false;
                mShakeIntensity = mShakeTimer = 0f;
                mOriginalPosition.x = InternalCalls.GetTransformPosition().x;
                mOriginalPosition.y = InternalCalls.GetTransformPosition().y;

                mActivateRange = InternalCalls.CreateEntityPrefab("BoulderRange");

                float boulder_height = InternalCalls.GetTransformScaling().y;
                float offsetx = boulder_height * .65f;
                float offsety = boulder_height * .75f;
                InternalCalls.TransformSetPositionEntity(mOriginalPosition.x - offsetx, mOriginalPosition.y - offsety, mActivateRange);
            }

            public void UpdateBoulder()
            {
                int player = PlayerScript.PLAYER_ID;
                if (InternalCalls.GetCollidingEntityCheck(mActivateRange, player))
                    mIsActivated = true;

                if (mIsActivated)
                {
                    mShakeTimer += InternalCalls.GetDeltaTime();

                    if (mShakeTimer < SHAKE_DURATION)
                        BoulderShake();
                    else
                        BoulderFall();
                }
            }

            public void BoulderShake()
            {
                mShakeIntensity = MAX_INTENSITY * Math.Max(MIN_INTENSITY, 1 - mShakeTimer / SHAKE_DURATION);

                float offsetx = (float)(mRandom.NextDouble() * 2 - 1) * mShakeIntensity;
                float offsety = (float)(mRandom.NextDouble() * 2 - 1) * mShakeIntensity;

                InternalCalls.TransformSetPosition(mOriginalPosition.x + offsetx, mOriginalPosition.y + offsety);
            }

            public void BoulderFall()
            {
                mShakeIntensity = 0f;
                InternalCalls.SetGravityScale(5f);

                if (!mIsGrounded)
                {
                    if (InternalCalls.CompareCategory("Ground"))
                    {
                        mIsGrounded = true;
                        return;
                    }

                    int player = PlayerScript.PLAYER_ID;
                    int boulder = InternalCalls.GetCurrentEntityID();
                    if (InternalCalls.GetCollidingEntityCheck(player, boulder))
                    {
                        PlayerScript.isDead = true;
                    }
                }
                if (mIsGrounded)
                {
                    //InternalCalls.SetStatic();
                }
            }
        }

    }
}
