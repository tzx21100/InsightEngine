using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace IS
{
    class FlightLevelFallingBoulderSet
    {
        static public Dictionary<int, FlightLevelFallingBoulder> flight_level_falling_boulder_set = new Dictionary<int, FlightLevelFallingBoulder>();

        static public void Init()
        {
            int entity_id = InternalCalls.GetCurrentEntityID();
            flight_level_falling_boulder_set[entity_id] = new FlightLevelFallingBoulder();
            flight_level_falling_boulder_set[entity_id].init();
        }

        static public void Update()
        {
            int entity_id = InternalCalls.GetCurrentEntityID();
            flight_level_falling_boulder_set[entity_id].update(); // Call Update on each GlitchingPlatform instance

        }


        static public void CleanUp()
        {
            //InternalCalls.SetSpriteImageEntity(InternalCalls.GetSpriteImage("glitched_platform_vfx 2R6C.png"), InternalCalls.GetCurrentEntityID());
        }

    }

    class FlightLevelFallingBoulder
    {
        private const float MAX_INTENSITY  = 20f;
        private const float MIN_INTENSITY  = .5f;
        private const float SHAKE_DURATION = .05f;
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
            private int check_colliding_breakable = 0;
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
                check_colliding_breakable = 0;

                mShakeIntensity = mShakeTimer = mGroundedTimer = 0f;
                mOriginalPosition.x = InternalCalls.GetTransformPosition().x;
                mOriginalPosition.y = InternalCalls.GetTransformPosition().y;

                mActivateRange = InternalCalls.CreateEntityPrefab("BoulderRange");

                float boulder_height = InternalCalls.GetTransformScaling().y;
                float offsetx = boulder_height * .0f;
                float offsety = boulder_height * .05f;
                InternalCalls.TransformSetScaleEntity(2200f, 3500f, mActivateRange);
                InternalCalls.TransformSetPositionEntity(mOriginalPosition.x - offsetx, mOriginalPosition.y + offsety, mActivateRange);
                InternalCalls.TransformSetRotation(0f, 0f); // self rotation
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

                CheckCollideWithBreakableGround();

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

                // camera shake
                BoulderShakingCameraShake();
            }

            public void BoulderFall()
            {
                if (!mPlayBreakAudio)
                {
                    mPlayBreakAudio = true;
                    InternalCalls.AudioPlaySound("ROCK-COLLAPSE_GEN-HDF-20003_BREAKOFF.wav", false, .15f);
                }

                mShakeIntensity = 0f;
                InternalCalls.SetGravityScale(1f);

                if (!mIsGrounded)
                {
                    if (InternalCalls.CompareCategory("Ground"))
                    {
                        mIsGrounded = true;
                        BoulderLandingCameraShake(); // camera shake
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

                if (check_colliding_breakable >= 3)
                {
                    mIsActivated = false;
                }
                if (mIsGrounded)
                {
                    //InternalCalls.SetStatic();
                }
            }

            private void CheckCollideWithBreakableGround()
            {
                if (InternalCalls.CompareCategory("BreakableGround"))
                {
                    int entity_id = InternalCalls.GetCurrentEntityID();
                    SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);
                    for (int i = 0; i < array.length; i++)
                    {
                        int entity = array.GetValue(i);

                        if (InternalCalls.CheckEntityCategory(entity, "BreakableGround"))
                        {
                            check_colliding_breakable += 1;
                            InternalCalls.RigidBodySetBodyTypeEntity(1, entity);
                            InternalCalls.SetEntityGravityScale(5f, entity);

                            // play landing sound
                            if (!mPlayGroundedAudio)
                            {
                                mPlayGroundedAudio = true;
                                InternalCalls.AudioPlaySound("SCI-FI-EXPLOSION_GEN-HDF-20662.wav", false, .55f);
                            }

                            // render particles
                            if (check_colliding_breakable < 3 && check_colliding_breakable > 0)
                            {
                                DrawBreakableGroundParticles(entity);
                            }
                            
                        }
                    }
                }

                if (check_colliding_breakable < 3 && check_colliding_breakable > 0)
                {
                    BoulderLandingCameraShake(); // camera shake
                }

                if (check_colliding_breakable > 0)
                {
                    mIsActivated = false;
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
                InternalCalls.TransformSetRotation(0f, 0f);
                InternalCalls.RigidBodySetForce(0f, 0f);
            }

            private void BoulderShakingCameraShake()
            {
                CameraScript.CameraShake(2f);
                CameraScript.camera_shake_intensity = 1f;
                CameraScript.camera_shake_duration = 0.8f;
            }

            private void BoulderLandingCameraShake()
            {
                CameraScript.CameraShake(2f);
                CameraScript.camera_shake_intensity = 3f;
                CameraScript.camera_shake_duration = 1f;
            }

            private void DrawParticles(int entity_id)
            {
                Vector2D pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(entity_id));
                Vector2D scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScalingEntity(entity_id));
                uint r = (uint)(12314 * InternalCalls.GetRandomFloat());
                MyRandom my_rand = new MyRandom(r);
                int num = (int)(scaling.x / 200f + 20 * my_rand.NextFloat());
                int top_layer = InternalCalls.GetTopLayer();
                for (int i = 0; i < num; i++)
                {

                    float rand = InternalCalls.GetRandomFloat();
                    float dir = 270 + 30 * (rand - 0.5f);

                    rand = InternalCalls.GetRandomFloat();
                    float size = scaling.x / 100f + 20f * rand; // base on image size

                    rand = InternalCalls.GetRandomFloat();
                    float size_scale = -10 * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float alpha = 0.5f + 0.5f * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float lifetime = 0.5f + 1f * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float speed = 1000f + scaling.x / 2f * rand; // base on size

                    rand = InternalCalls.GetRandomFloat();
                    float x = pos.x + scaling.x / 2f * (rand - 0.5f);

                    rand = InternalCalls.GetRandomFloat();
                    float y = pos.y + scaling.y / 2f * (rand - 0.2f);

                    InternalCalls.GameSpawnParticleExtraLayer(
                        x, y, dir, size, size_scale, alpha, 0.5f, lifetime, speed, "ParticleDust.txt",
                    (1f, 1f, 1f), top_layer);
                }
            }

            private void DrawBreakableGroundParticles(int entity_id)
            {
                Vector2D pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(entity_id));
                Vector2D scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScalingEntity(entity_id));
                uint r = (uint)(12314 * InternalCalls.GetRandomFloat());
                MyRandom my_rand = new MyRandom(r);
                int num = (int)(40 + 30 * my_rand.NextFloat());
                int top_layer = InternalCalls.GetTopLayer();
                for (int i = 0; i < num; i++)
                {

                    float rand = InternalCalls.GetRandomFloat();
                    float dir = 90 + 30 * (rand - 0.5f);

                    rand = InternalCalls.GetRandomFloat();
                    float size = 30 + 20f * rand; // base on image size

                    rand = InternalCalls.GetRandomFloat();
                    float size_scale = -10 * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float alpha = 0.5f + 0.5f * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float lifetime = 0.5f + 1f * rand;

                    rand = InternalCalls.GetRandomFloat();
                    float speed = 500f + 200f * rand; // base on size

                    rand = InternalCalls.GetRandomFloat();
                    float x = pos.x + scaling.x / 3f * (rand - 0.5f);

                    rand = InternalCalls.GetRandomFloat();
                    float y = pos.y + scaling.y / 4f + scaling.y / 4f * (rand - 0.2f);

                    InternalCalls.GameSpawnParticleExtraLayer(
                        x, y, dir, size, size_scale, alpha, 0.5f, lifetime, speed, "ParticleDust.txt",
                    (1f, 1f, 1f), top_layer);
                }
            }
        }
    }
}
