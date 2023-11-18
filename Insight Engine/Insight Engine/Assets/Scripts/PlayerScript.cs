using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.ComponentModel;

namespace IS
{
    public class PlayerScript
    {
        
        //private entity

        static SimpleImage backGround;
        static SimpleImage player_walk;
        static SimpleImage player_idle;



        //private variables for entity

        static private float xCoord;
        static private float yCoord;
        static private float width;
        static private float height;

        static private int jump_amount;
        static private bool isJumping;
        static private float dash_timer;
        static private bool isDashing;

        static private bool isGrounded;



        static private int entityA;


        public static int BoolToInt(bool boolValue)
        {
            return boolValue ? 1 : 0;
        }

        static public void Init()
        {
            backGround = InternalCalls.GetSpriteImage("black_background.png");
            //player_walk = InternalCalls.GetSpriteImage("Assets/Textures/player_walking.png");
            //player_idle = InternalCalls.GetSpriteImage("Assets/Textures/player_idle.png");

            player_walk = InternalCalls.GetSpriteImage("Player run 1R12C.png");
            player_idle = InternalCalls.GetSpriteImage("Player idle 1R12C.png");
            // Initialization code
            Console.WriteLine("ctor!");
            //InternalCalls.NativeLog("Entity Initialized", (int)entity);
            InternalCalls.ResetAnimations();
            InternalCalls.CreateAnimationFromSprite(1,12,3f);
            InternalCalls.CreateAnimationFromSprite(1,12,3f);

            entityA = InternalCalls.CreateEntity("FeetCollider");
            width = InternalCalls.GetTransformScaling().x;
            height = InternalCalls.GetTransformScaling().y;
            InternalCalls.AddCollider(entityA);
            InternalCalls.CameraSetZoom(300f);


        }

        static public void Update()
        {
            if (GameManager.isGamePaused == true) {
                return;
            }

            //Player x y coord
            xCoord = InternalCalls.GetTransformPosition().x;
            yCoord = InternalCalls.GetTransformPosition().y;

            //Feet Collider
            InternalCalls.TransformSetScaleEntity(width, 2f, entityA);
            InternalCalls.TransformSetPositionEntity(xCoord, yCoord-height/1.5f,entityA);

            InternalCalls.TransformSetRotationEntity(0, 0, entityA);   

            InternalCalls.AttachCamera();

            //player_walk.texture_index = 2;
            //player_idle.texture_index = 3;

            // Update code

            //movement
            int hori_movement = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.D)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.A));
            int verti_movement = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.W)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.S));
            InternalCalls.RigidBodyAddForce(hori_movement * 1000f * InternalCalls.GetDeltaTime(), verti_movement * 1000f * InternalCalls.GetDeltaTime());

            // scaling transform with movement
            Vector2D trans_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
            Vector2D trans_scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
            float trans_rotate = InternalCalls.GetTransformRotation();
            if (InternalCalls.KeyHeld((int)KeyCodes.A)) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } }
            if (InternalCalls.KeyHeld((int)KeyCodes.D)) { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }
            InternalCalls.TransformSetScale(trans_scaling.x, trans_scaling.y);

            int rotate = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.Q)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.E));
            trans_rotate += rotate * InternalCalls.GetRigidBodyAngularVelocity();
            trans_rotate = trans_rotate < 0 ? 360 : trans_rotate %= 360;
            InternalCalls.TransformSetRotation(trans_rotate, 0);

            if (hori_movement != 0 || verti_movement != 0) {
                InternalCalls.SetSpriteAnimationIndex(0);
                //InternalCalls.NativeLog("texture_index", player_walk.texture_index);
                InternalCalls.SetSpriteImage(player_walk);
                //InternalCalls.TransformSetScale(player_walk.width, player_walk.height);
                
            }
            else
            {
                InternalCalls.SetSpriteAnimationIndex(1);
                InternalCalls.SetSpriteImage(player_idle);
                
            }


            //if is grounded
            if (InternalCalls.EntityCheckCollide(entityA))
            {
                isGrounded = true;
            }
            else {
                isGrounded = false;
            }
            
            if (isGrounded)
            {
                Console.WriteLine(isGrounded);
                if (InternalCalls.KeyPressed((int)KeyCodes.Space)) {
                    Jump();
                }
            }

        }

        static public void CleanUp()
        {
            InternalCalls.FreeSpriteImage(backGround);
            InternalCalls.FreeSpriteImage(player_idle);
            InternalCalls.FreeSpriteImage(player_walk);
            InternalCalls.DestroyEntity(entityA);
        }



        static private void Dashing() { 
            

            
        }

        static private void Jump()
        {
            InternalCalls.RigidBodyAddForce(0f, 2000f);
        }
       

    }



}

