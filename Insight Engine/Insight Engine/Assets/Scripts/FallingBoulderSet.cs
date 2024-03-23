using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace IS
{
    class FallingBoulderSet
    {
        static public Dictionary<int, FallingBoulder> falling_boulder_set = new Dictionary<int, FallingBoulder>();

        static public void Init()
        {
            int entity_id = InternalCalls.GetCurrentEntityID();
            falling_boulder_set[entity_id] = new FallingBoulder();
            falling_boulder_set[entity_id].init();
        }

        static public void Update()
        {
            int entity_id = InternalCalls.GetCurrentEntityID();
            falling_boulder_set[entity_id].update(); // Call Update on each GlitchingPlatform instance

        }



        static public void CleanUp()
        {
            //InternalCalls.SetSpriteImageEntity(InternalCalls.GetSpriteImage("glitched_platform_vfx 2R6C.png"), InternalCalls.GetCurrentEntityID());
        }

    }

    class FallingBoulder
    {
        private const float MAX_INTENSITY  = 20f;
        private const float MIN_INTENSITY  = .5f;
        private const float SHAKE_DURATION = .55f;
        private const float RESET_DURATION = 5f;
        private static Random mRandom = new Random();

        private static Dictionary<int, BoulderHelper> mBoulders = new Dictionary<int, BoulderHelper>();

        public void init()
        {
            GetActiveBoulder().InitializeBoulder();
        }

        public void update()
        {
            GetActiveBoulder().UpdateBoulder();
        }

        public void CleanUp()
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
                // Reset Boulder properties
                mIsActivated = false;
                mIsGrounded = false;
                mPlayBreakAudio = false;
                mPlayShakeAudio = false;
                mPlayGroundedAudio = false;

                mShakeIntensity = mShakeTimer = mGroundedTimer = 0f;
                mOriginalPosition.x = InternalCalls.GetTransformPosition().x;
                mOriginalPosition.y = InternalCalls.GetTransformPosition().y;

                mActivateRange = InternalCalls.CreateEntityPrefab("BoulderRange");

                float boulder_height = InternalCalls.GetTransformScaling().y;
                float offsetx = boulder_height * .0f;
                float offsety = boulder_height * .75f;
                InternalCalls.TransformSetScaleEntity(1200f, 800f, mActivateRange);
                InternalCalls.TransformSetPositionEntity(mOriginalPosition.x - offsetx, mOriginalPosition.y - offsety, mActivateRange);
            }

            public void UpdateBoulder()
            {
                //int player = PlayerScript.PLAYER_ID;
                SimpleArray array = InternalCalls.GetCollidingEntityArray(mActivateRange);

                if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
                    //if (InternalCalls.GetCollidingEntityCheck(mActivateRange, player))
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

                // draw particles
                DrawParticles(InternalCalls.GetCurrentEntityID());
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
                        PlayerScript.screen_flash_timer = 1.5f;
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
                mIsActivated = false;
                mIsGrounded = false;
                mPlayBreakAudio = false;
                mPlayShakeAudio = false;
                mPlayGroundedAudio = false;

                mShakeIntensity = mShakeTimer = mGroundedTimer = 0f;
                InternalCalls.SetSpriteAlpha(1f); // Reset sprite alpha
                InternalCalls.SetGravityScale(0f); // Reset gravity scale

                // Reset boulder position to original position
                InternalCalls.TransformSetPosition(mOriginalPosition.x, mOriginalPosition.y);
                InternalCalls.TransformSetRotation(8f, 0f);
                InternalCalls.RigidBodySetForce(0f, 0f);
            }

            private void DrawParticles(int entity_id)
            {
                Vector2D pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(entity_id));
                uint r = (uint)(12314 * InternalCalls.GetRandomFloat());
                MyRandom my_rand = new MyRandom(r);
                int num = (int)(3f * my_rand.NextFloat());
                int top_layer = InternalCalls.GetTopLayer();
                Vector2D scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScalingEntity(entity_id));
                for (int i = 0; i < num; i++)
                {

                    float rand = InternalCalls.GetRandomFloat();
                    float dir = 270 + 30 * (rand - 0.5f);

                    rand = InternalCalls.GetRandomFloat();
                    float size = 20f + 10f * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float size_scale = -10 * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float alpha = 0.5f + 0.5f * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float lifetime = 0.3f + 1f * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float speed = 1000f + 500f * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float x = pos.x + scaling.x / 2f * (rand - 0.5f);

                    rand = InternalCalls.GetRandomFloat();
                    float y = pos.y + scaling.y / 2f * (rand - 0.2f);

                    InternalCalls.GameSpawnParticleExtraLayer(
                        x, y, dir, size, size_scale, alpha, 0.5f, lifetime, speed, "ParticleDust.txt",
                    (1f, 1f, 1f), top_layer);
                }
            }
        }
    }
}
