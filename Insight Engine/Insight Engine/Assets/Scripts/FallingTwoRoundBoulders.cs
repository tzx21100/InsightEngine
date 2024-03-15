/*!
 * \file FallingTwoRoundBoulders.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2451
 * \date 15-03-2024
 * \brief
 * This header file contains the implementation of the Falling Two Round Boulders class
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace IS
{
    class FallingTwoRoundBoulders
    {
        private const float MAX_INTENSITY = 20f;
        private const float MIN_INTENSITY = 0.5f;
        private const float SHAKE_DURATION = 0.55f;
        private const float RESET_DURATION = 5f;
        private const float TWO_BOULDERS_FALLING_INTERVAL = 0.05f;
        static private Random mRandom = new Random();

        static private int SecondBoulderID;
        static private int FirstBoulderID;
        static private int mActivateRangeID;

        static private Vector2D ActiveRangeScaling = new Vector2D(300f, 1050f);
        //static private Vector2D ActiveRangeOffset = new Vector2D(0f, 0f);

        static private bool mSecondIsActivated;
        static private bool mFirstIsActivated;
        static private bool mSecondIsGrounded;
        static private bool mFirstIsGrounded;

        static private bool mPlaySecondBreakAudio;
        static private bool mPlayFirstBreakAudio;
        static private bool mPlaySecondShakeAudio;
        static private bool mPlayFirstShakeAudio;
        static private bool mPlaySecondGroundedAudio;
        static private bool mPlayFirstGroundedAudio;

        static private float mSecondShakeIntensity;
        static private float mFirstShakeIntensity;
        static private float mSecondShakeTimer;
        static private float mFirstShakeTimer;
        //static private float FallingIntervalTimer;
        //private float mGroundedTimer; // Timer for how long the boulder has been grounded
        static private Vector2D mSecondOriginalPosition = new Vector2D(0f, 0f);
        static private Vector2D mFirstOriginalPosition = new Vector2D(0f, 0f);
        static private Vector2D BouldersOffset = new Vector2D(-1200f, 0f);

        //static SimpleImage FirstBoulder;


        static public void Init()
        {
            InitializeBoulder();
        }

        static public void Update()
        {
            UpdateBoulder();
            //Console.WriteLine(mSecondIsActivated);
        }
        
        static public void CleanUp()
        {

        }

        static public void InitializeBoulder()
        {
            SecondBoulderID = InternalCalls.GetCurrentEntityID();
            FirstBoulderID = InternalCalls.CreateEntityPrefab("FirstBoulder");
            //FirstBoulder = InternalCalls.GetSpriteImage("Env_Round_Boulder.png");
            //InternalCalls.SetSpriteImageEntity(FirstBoulder, FirstBoulderID);
            //InternalCalls.AddCollider(FirstBoulderID);

            mActivateRangeID = InternalCalls.CreateEntityPrefab("BoulderRange");

            mSecondIsActivated = false;
            mFirstIsActivated = false;
            mSecondIsGrounded = false;
            mFirstIsGrounded = false;

            mPlaySecondBreakAudio = false;
            mPlayFirstBreakAudio = false;
            mPlaySecondShakeAudio = false;
            mPlayFirstShakeAudio = false;
            mPlaySecondGroundedAudio = false;
            mPlayFirstGroundedAudio = false;

            mSecondShakeIntensity 
            = mFirstShakeIntensity 
            = mSecondShakeTimer 
            = mFirstShakeTimer 
            = 0f;

            mSecondOriginalPosition = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(SecondBoulderID));
            mFirstOriginalPosition = new Vector2D(mSecondOriginalPosition.x + BouldersOffset.x, mSecondOriginalPosition.y + BouldersOffset.y);

            InternalCalls.TransformSetScaleEntity(InternalCalls.GetTransformScaling().x, InternalCalls.GetTransformScaling().y, FirstBoulderID);
            InternalCalls.TransformSetPositionEntity(mFirstOriginalPosition.x, mFirstOriginalPosition.y, FirstBoulderID);

            float boulder_height = InternalCalls.GetTransformScaling().y;
            float offsetx = boulder_height * 1.1f;
            float offsety = boulder_height * .2f;
            //ActiveRangeOffset = new Vector2D(offsetx, offsety);
            InternalCalls.TransformSetScaleEntity(ActiveRangeScaling.x, ActiveRangeScaling.y, mActivateRangeID);
            InternalCalls.TransformSetPositionEntity(mSecondOriginalPosition.x - offsetx, mSecondOriginalPosition.y - offsety, mActivateRangeID);
        }

        static public void UpdateBoulder()
        {
            //int player = PlayerScript.PLAYER_ID;
            SimpleArray array = InternalCalls.GetCollidingEntityArray(mActivateRangeID);

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
                //if (InternalCalls.GetCollidingEntityCheck(mActivateRange, player))
                mSecondIsActivated = true;

            CheckSecondActivated();
            CheckFirstActivated();

            CheckSecondGrounded();
            CheckFirstGrounded();
        }


        // Helper function for second Boulder
        static public void CheckSecondActivated()
        {
            if (mSecondIsActivated)
            {
                mSecondShakeTimer += InternalCalls.GetDeltaTime();

                if (mSecondShakeTimer < SHAKE_DURATION)
                    SecondBoulderShake();
                else
                    SecondBoulderFall();

                if (mSecondShakeTimer > TWO_BOULDERS_FALLING_INTERVAL)
                {
                    mFirstIsActivated = true;
                }
            }
        }

        static public void CheckSecondGrounded()
        {
            if (mSecondIsGrounded)
            {
                if (!mPlaySecondGroundedAudio)
                {
                    mPlaySecondGroundedAudio = true;
                    InternalCalls.AudioPlaySound("ROCK-DROP_GEN-HDF-20027.wav", false, .15f);
                }

                //mGroundedTimer += InternalCalls.GetDeltaTime();

                // Slowly fade the boulder out
                //float alpha = Vector2D.lerp(1f, 0f, mGroundedTimer / RESET_DURATION);
                //InternalCalls.SetSpriteAlpha(alpha);

                // Check if it's time to reset the boulder
                /*if (mGroundedTimer >= RESET_DURATION)
                {
                    ResetBoulder();
                }*/
            }
        }

        static public void SecondBoulderShake()
        {
            if (!mPlaySecondShakeAudio)
            {
                mPlaySecondShakeAudio = true;
                InternalCalls.AudioPlaySound("ROCK-DEBRIS_GEN-HDF-20011_SHAKE.wav", false, 1f);
            }

            mSecondShakeIntensity = MAX_INTENSITY * Math.Max(MIN_INTENSITY, 1 - mSecondShakeTimer / SHAKE_DURATION);

            float offsetx = (float)(mRandom.NextDouble() * 2 - 1) * mSecondShakeIntensity;
            float offsety = (float)(mRandom.NextDouble() * 2 - 1) * mSecondShakeIntensity;

            InternalCalls.TransformSetPosition(mSecondOriginalPosition.x + offsetx, mSecondOriginalPosition.y + offsety);

        }

        static public void SecondBoulderFall()
        {
            if (!mPlaySecondBreakAudio)
            {
                mPlaySecondBreakAudio = true;
                InternalCalls.AudioPlaySound("ROCK-COLLAPSE_GEN-HDF-20003_BREAKOFF.wav", false, .15f);
            }

            mSecondShakeIntensity = 0f;
            InternalCalls.SetEntityGravityScale(3f, SecondBoulderID);

            if (!mSecondIsGrounded)
            {
                if (InternalCalls.CompareEntityCategory(SecondBoulderID, "Ground"))
                {
                    mSecondIsGrounded = true;
                    return;
                }

                int player = PlayerScript.PLAYER_ID;
                int boulder = SecondBoulderID;
                if (InternalCalls.GetCollidingEntityCheck(player, boulder))
                {
                    if (PlayerScript.player_pos.y < InternalCalls.GetTransformPositionEntity(SecondBoulderID).y)
                    {
                        // if the player contact with lower part of the boulder, he dies
                        PlayerScript.isDead = true;
                    }
                }
            }
            if (mSecondIsGrounded)
            {
                InternalCalls.SetStaticEntity(SecondBoulderID);
            }
        }

        // Helper function for first boulder
        static public void CheckFirstActivated()
        {
            if (mFirstIsActivated)
            {
                mFirstShakeTimer += InternalCalls.GetDeltaTime();

                if (mFirstShakeTimer < SHAKE_DURATION)
                    FirstBoulderShake();
                else
                    FirstBoulderFall();
            }
        }

        static public void CheckFirstGrounded()
        {
            if (mFirstIsGrounded)
            {
                if (!mPlayFirstGroundedAudio)
                {
                    mPlayFirstGroundedAudio = true;
                    InternalCalls.AudioPlaySound("ROCK-DROP_GEN-HDF-20027.wav", false, .15f);
                }
            }
        }
        static uint r = (uint)(12314 * InternalCalls.GetRandomFloat());
        static MyRandom my_rand = new MyRandom(r);
        static public void FirstBoulderShake()
        {
            if (!mPlayFirstShakeAudio)
            {
                mPlayFirstShakeAudio = true;
                InternalCalls.AudioPlaySound("ROCK-DEBRIS_GEN-HDF-20011_SHAKE.wav", false, 1f);
            }

            mFirstShakeIntensity = MAX_INTENSITY * Math.Max(MIN_INTENSITY, 1 - mFirstShakeTimer / SHAKE_DURATION);

            float offsetx = (float)(mRandom.NextDouble() * 2 - 1) * mFirstShakeIntensity;
            float offsety = (float)(mRandom.NextDouble() * 2 - 1) * mFirstShakeIntensity;

            InternalCalls.TransformSetPositionEntity(mFirstOriginalPosition.x + offsetx, mFirstOriginalPosition.y + offsety, FirstBoulderID);

            // draw shaking particles
            Vector2D pos = mFirstOriginalPosition;
            
            int num = (int)(3f * my_rand.NextFloat());
            int top_layer = InternalCalls.GetTopLayer();
            Vector2D scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScalingEntity(FirstBoulderID));
            for (int i = 0; i < num; i++)
            {


                float rand = InternalCalls.GetRandomFloat();
                float dir = 270 + 30 * (rand-0.5f);

                rand = InternalCalls.GetRandomFloat();
                float size = 20f * rand;

                rand = InternalCalls.GetRandomFloat();
                float size_scale = 20 * rand;

                rand = InternalCalls.GetRandomFloat();
                float alpha = 0.5f + 0.5f * rand;

                rand = InternalCalls.GetRandomFloat();
                float lifetime = 0.3f+1f*rand;
                
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

        static public void FirstBoulderFall()
        {
            if (!mPlayFirstBreakAudio)
            {
                mPlayFirstBreakAudio = true;
                InternalCalls.AudioPlaySound("ROCK-COLLAPSE_GEN-HDF-20003_BREAKOFF.wav", false, .15f);
            }

            mFirstShakeIntensity = 0f;
            InternalCalls.SetEntityGravityScale(3f, FirstBoulderID);

            if (!mFirstIsGrounded)
            {
                if (InternalCalls.CompareEntityCategory(FirstBoulderID, "Ground"))
                {
                    mFirstIsGrounded = true;
                    return;
                }

                int player = PlayerScript.PLAYER_ID;
                int boulder = FirstBoulderID;
                if (InternalCalls.GetCollidingEntityCheck(player, boulder))
                {
                    if (PlayerScript.player_pos.y < InternalCalls.GetTransformPositionEntity(FirstBoulderID).y)
                    {
                        // if the player contact with lower part of the boulder, he dies
                        PlayerScript.isDead = true;
                    }
                }
            }
            if (mFirstIsGrounded)
            {
                InternalCalls.SetStaticEntity(FirstBoulderID);
            }
        }

    }
}
