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
        private const float RESET_DURATION = 5f;
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
            private bool mPlayBreakAudio;
            private bool mPlayShakeAudio;
            private bool mPlayGroundedAudio;
            private float mShakeIntensity;
            private float mShakeTimer;
            private float mGroundedTimer; // Timer for how long the boulder has been grounded
            private Vector2D mOriginalPosition = new Vector2D(0f, 0f);

            public void InitializeBoulder()
            {
                mIsActivated = mIsGrounded = mPlayBreakAudio = mPlayShakeAudio = mPlayShakeAudio = mPlayGroundedAudio = false;
                mShakeIntensity = mShakeTimer = mGroundedTimer = 0f;
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

                // Update grounded timer if the boulder is grounded
                if (mIsGrounded)
                {
                    if (!mPlayGroundedAudio)
                    {
                        mPlayGroundedAudio = true;
                        InternalCalls.AudioPlaySound("ROCK-DROP_GEN-HDF-20027.wav", false, .15f);
                    }

                    mGroundedTimer += InternalCalls.GetDeltaTime();

                    // Slowly fade the boulder out
                    float alpha = Vector2D.lerp(1f, 0f, mGroundedTimer / RESET_DURATION);
                    InternalCalls.SetSpriteAlpha(alpha);

                    // Check if it's time to reset the boulder
                    if (mGroundedTimer >= RESET_DURATION)
                    {
                        ResetBoulder();
                    }
                }
            }

            public void BoulderShake()
            {
                if (!mPlayShakeAudio)
                {
                    mPlayShakeAudio = true;
                    InternalCalls.AudioPlaySound("ROCK-DEBRIS_GEN-HDF-20011_SHAKE.wav", false, 1f);
                }

                mShakeIntensity = MAX_INTENSITY * Math.Max(MIN_INTENSITY, 1 - mShakeTimer / SHAKE_DURATION);

                float offsetx = (float)(mRandom.NextDouble() * 2 - 1) * mShakeIntensity;
                float offsety = (float)(mRandom.NextDouble() * 2 - 1) * mShakeIntensity;

                InternalCalls.TransformSetPosition(mOriginalPosition.x + offsetx, mOriginalPosition.y + offsety);
            }

            public void BoulderFall()
            {
                if (!mPlayBreakAudio)
                {
                    mPlayBreakAudio = true;
                    InternalCalls.AudioPlaySound("ROCK-COLLAPSE_GEN-HDF-20003_BREAKOFF.wav", false, .15f);
                }

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

            private void ResetBoulder()
            {
                // Reset boulder properties
                mIsActivated = mIsGrounded = mPlayBreakAudio = mPlayShakeAudio = mPlayShakeAudio = mPlayGroundedAudio = false;
                mShakeIntensity = mShakeTimer = mGroundedTimer = 0f;
                InternalCalls.SetSpriteAlpha(1f); // Reset sprite alpha
                InternalCalls.SetGravityScale(0f); // Reset gravity scale

                // Reset boulder position to original position
                InternalCalls.TransformSetPosition(mOriginalPosition.x, mOriginalPosition.y);
                InternalCalls.TransformSetRotation(18f, 0f);
            }
        }
    }
}
