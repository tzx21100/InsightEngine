using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.ComponentModel;
using System.Diagnostics;
using System.Reflection.Metadata;

namespace IS
{
    public class PlayerScript
    {
        //Powerup triggers
        static public bool Reward_DoubleJump = false;
        static public bool Reward_Dash = false;
        static public bool Reward_WallClimb = true;

        //public player variables


        //private entity

        static SimpleImage player_walk;
        static SimpleImage player_idle;
        static SimpleImage player_climb;
        static SimpleImage player_climb1;
        static SimpleImage player_climb2;
        static SimpleImage player_climb3;
        static SimpleImage player_climb4;
        static SimpleImage player_climb5;
        static SimpleImage player_climb6;
        static SimpleImage player_climb7;
        static SimpleImage player_climb8;
        static SimpleImage player_climb9;
        static SimpleImage player_climb10;
        static SimpleImage player_climb11;
        static SimpleImage player_transparent;
        static SimpleImage player_land;

        //psuedo animations for images
        static private float animation_speed = 0.07f;
        static private float animation_speed_set = 0.07f;
        static private int animation_current_frame = 0;

        static private int land_entity;
        static private bool land_exists;

        static private int PLAYER_ID;

        //private variables for entity

        static private float xCoord;
        static private float yCoord;
        static private float width;
        static private float height;

        //double jump and above
        static private int jump_amount;
        static private int jump_amount_set = 1;
        static private float jump_timer = 0.2f;
        static private float jump_timer_set = 0.2f;
        static private bool isJumping = false;
        static private bool initial_land = false;
        static private float jumpHeight = 1000f;

        //dashing 
        static private float bullet_time_timer = 1f;
        static private float bullet_time_set = 1f;
        static private float dash_timer =0.2f;
        static private float dash_set = 0.2f;
        static private bool canDash = false;
        static private bool isDashing;
        static private float dashAngle;
        static private float dashSpeed = 5000f;

        static private bool isGrounded;
        static private Vector2D apply_force= new Vector2D(0,0);//dash dir


        static private int entityA;

        //movement
        static private float acceleration=0f;
        static private float acceleration_base = 50f;
        static private float acceleration_increment = 10f;
        static private float max_acceleration=100;
        static private float max_speed=700f;
        static private float move_speed = 0f;

        static private int hori_movement;

        // climbing
        static private bool isClimbing = false;
        static private float climbSpeed = 1500f;
        static private int entityWall;
        static int climbdir;


        //force calculations
        static private float Xforce=0f;
        static private float Yforce=0f;

        // player pos
        static public Vector2D player_pos =new Vector2D(0,0);
        static private Vector2D trans_scaling = new Vector2D(0,0);


        public static int BoolToInt(bool boolValue)
        {
            return boolValue ? 1 : 0;
        }

        static public void Init()
        {

            //player_walk = InternalCalls.GetSpriteImage("Assets/Textures/player_walking.png");
            //player_idle = InternalCalls.GetSpriteImage("Assets/Textures/player_idle.png");

            player_walk = InternalCalls.GetSpriteImage("running_anim 4R3C.png");
            player_idle = InternalCalls.GetSpriteImage("idle_anim 4R3C.png");
            player_climb = InternalCalls.GetSpriteImage("WallClimb_0000.png");
            player_climb1 = InternalCalls.GetSpriteImage("WallClimb_0001.png");
            player_climb2 = InternalCalls.GetSpriteImage("WallClimb_0002.png");
            player_climb3 = InternalCalls.GetSpriteImage("WallClimb_0003.png");
            player_climb4 = InternalCalls.GetSpriteImage("WallClimb_0004.png");
            player_climb5 = InternalCalls.GetSpriteImage("WallClimb_0005.png");
            player_climb6 = InternalCalls.GetSpriteImage("WallClimb_0006.png");
            player_climb7 = InternalCalls.GetSpriteImage("WallClimb_0007.png");
            player_climb8 = InternalCalls.GetSpriteImage("WallClimb_0008.png");
            player_climb9 = InternalCalls.GetSpriteImage("WallClimb_0009.png");
            player_climb10 = InternalCalls.GetSpriteImage("WallClimb_0010.png");
            player_climb11 = InternalCalls.GetSpriteImage("WallClimb_0011.png");
            player_transparent = InternalCalls.GetSpriteImage("transparent.png");
            player_land = InternalCalls.GetSpriteImage("land_vfx 2R7C.png");

            // Initialization code
            Console.WriteLine("ctor!");
            //InternalCalls.NativeLog("Entity Initialized", (int)entity);
            InternalCalls.ResetAnimations();
            InternalCalls.CreateAnimationFromSprite(4,3,1f);
            InternalCalls.CreateAnimationFromSprite(4,3,1f);
            

            entityA = InternalCalls.CreateEntity("FeetCollider");
            entityWall = InternalCalls.CreateEntity("WallCollider");
            width = InternalCalls.GetTransformScaling().x /3f;
            height = InternalCalls.GetTransformScaling().y;
            InternalCalls.AddCollider(entityA);
            InternalCalls.AddCollider(entityWall);


            InternalCalls.CameraSetZoom(1.5f);

            land_entity = InternalCalls.CreateEntityVFX("jump", player_land);
            InternalCalls.CreateAnimationFromSpriteEntity(2, 7, 0.3f, land_entity);


        }

        static public void Update()
        {
            if (GameManager.isGamePaused == true) {
                return;
            }

            //Attach Camera
            InternalCalls.AttachCamera();

            PLAYER_ID = InternalCalls.GetCurrentEntityID();


            //movement
            hori_movement = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.D)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.A));
            player_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());

            // scaling transform with movement
            Vector2D trans_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
            trans_scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
            float trans_rotate = InternalCalls.GetTransformRotation();
            if (InternalCalls.KeyHeld((int)KeyCodes.A)) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } }
            if (InternalCalls.KeyHeld((int)KeyCodes.D)) { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }


            /*            int rotate = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.Q)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.E));
                        trans_rotate += rotate * InternalCalls.GetRigidBodyAngularVelocity();*/
            /*            if (trans_rotate > 45 && trans_rotate < 270) { trans_rotate = 45; }
                        if (trans_rotate < -45) { trans_rotate = -45; }*/



            //SPRITE

            if (hori_movement != 0)
            {
                acceleration += acceleration_increment;
                if (acceleration > max_acceleration) { acceleration = max_acceleration; }
                move_speed += acceleration;
                if (move_speed > max_speed) { move_speed = max_speed; }

                InternalCalls.SetSpriteAnimationIndex(0);
                InternalCalls.SetSpriteImage(player_walk);

            }
            else
            {
                move_speed -= acceleration;
                acceleration -= acceleration_increment;
                if (acceleration < acceleration_base) { acceleration = acceleration_base; }
                if (move_speed < 0) { move_speed = 0; }


                InternalCalls.SetSpriteAnimationIndex(1);
                InternalCalls.SetSpriteImage(player_idle);

            }







            //wall checking
            if (InternalCalls.EntityCheckCollide(entityWall) && hori_movement != 0 && InternalCalls.GetCollidingEntity(entityWall) != InternalCalls.GetCurrentEntityID() && InternalCalls.CollidingObjectIsStatic(InternalCalls.GetCollidingEntity(entityWall)) && Reward_WallClimb)
            {
                if (!isClimbing) { InternalCalls.RigidBodySetForce(0, 0); climbdir = hori_movement; }
                isClimbing = true;
            }
            else { isClimbing = false; animation_current_frame = 0; animation_speed = animation_speed_set; }

            if (isClimbing) {


                InternalCalls.SetSpriteImage(player_transparent);
                InternalCalls.SetSpriteAnimationIndex(0);

                float collided_angle = InternalCalls.GetCollidedObjectAngle(entityWall) - (90 * hori_movement);

                InternalCalls.TransformSetRotation(collided_angle, 0);
                Vector2D force_from_angle = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(collided_angle * -hori_movement));

                InternalCalls.RigidBodySetForce(climbSpeed * force_from_angle.x * hori_movement, climbSpeed * force_from_angle.y);

                if (hori_movement != climbdir) {
                    isClimbing = false;
                    Xforce += hori_movement * 2000f;
                }


                float x_offset = 28 * hori_movement;
                SimpleVector2D pos = InternalCalls.GetTransformPosition();
                pos.x += x_offset;
                SimpleVector2D scale = new SimpleVector2D(InternalCalls.GetTransformScaling().x * 1.1f, InternalCalls.GetTransformScaling().y * 1.1f);

                SimpleImage curr_image = player_climb;

                switch (animation_current_frame) {
                    case 0:
                        curr_image = player_climb;
                        break; case 1:
                        curr_image = player_climb1;
                        break; case 2:
                        curr_image = player_climb2;
                        break; case 3:
                        curr_image = player_climb3;
                        break; case 4:
                        curr_image = player_climb4;
                        break; case 5:
                        curr_image = player_climb5;
                        break; case 6:
                        curr_image = player_climb6;
                        break; case 7:
                        curr_image = player_climb7;
                        break; case 8:
                        curr_image = player_climb8;
                        break; case 9:
                        curr_image = player_climb9;
                        break; case 10:
                        curr_image = player_climb10;
                        break; case 11:
                        curr_image = player_climb11;
                        break;

                }

                animation_speed -= InternalCalls.GetDeltaTime();

                if (animation_speed <= 0) {

                    animation_speed = animation_speed_set;
                    animation_current_frame++;
                    if (animation_current_frame >= 10) {
                        animation_current_frame = 0;
                    }

                }


                InternalCalls.DrawImageAt(pos, 0f, scale, curr_image, 1);

            }
            else
            {
                InternalCalls.TransformSetScale(1f, 1f);
            }

            if (InternalCalls.RigidBodyGetVelocity().y < -1600f)
            {
                initial_land = false;
            }

            //jumping bool
            if (isJumping && jump_timer > 0f) { jump_timer -= InternalCalls.GetDeltaTime(); if (jump_timer <= 0) { isJumping = false; jump_timer = jump_timer_set; } }
                //if is grounded
            if (InternalCalls.EntityCheckCollide(entityA) && isClimbing==false && InternalCalls.GetCollidingEntity(entityA) != InternalCalls.GetCurrentEntityID())
            {

                if (initial_land == false)
                {
                    initial_land = true;
                    InternalCalls.ResetSpriteAnimationFrameEntity(land_entity);
                    InternalCalls.TransformSetScaleEntity(200, 200, land_entity);
                    InternalCalls.TransformSetPositionEntity(player_pos.x,player_pos.y,land_entity);
                }

                isGrounded = true;


            }
            else
            {
                isGrounded = false;
            }

            if (InternalCalls.GetCurrentAnimationEntity(land_entity) >= 13) {
                InternalCalls.TransformSetScaleEntity(0,0,land_entity);
                InternalCalls.TransformSetPositionEntity(-999,-9999,land_entity);
            }

            if (isGrounded)
            {
                if (!isJumping)
                {
                    float aangle = InternalCalls.GetTransformRotation();
                    /*                if (hori_movement != 0) {aangle+=180f; }*/
                    Vector2D f_angle = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(aangle));
                    //set move speed when grounded

                    InternalCalls.RigidBodySetForce(hori_movement * (move_speed + ((BoolToInt(isDashing)) * dashSpeed) *f_angle.x*-1f), f_angle.y  * move_speed *hori_movement);

                    // Set the rotation to be the same as the detected one
                    float collided_angle = InternalCalls.GetCollidedObjectAngle(entityA);
                    if ((collided_angle > 0 && collided_angle < 45) || (collided_angle > 315 && collided_angle < 360))
                    {
                        InternalCalls.TransformSetRotation(collided_angle, 0);
                    }
                    else
                    {
                        InternalCalls.TransformSetRotation(0, 0);
                    }

                    jump_amount = jump_amount_set;
                    canDash = true;

                    if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                    {
                        Jump();
                        isJumping = true;
                    }
                }
            }
            else if(!isClimbing){ //while in the air
                
                trans_rotate = 0;
                InternalCalls.TransformSetRotation(trans_rotate, 0);

                

                if (jump_amount > 0 && Reward_DoubleJump)
                {
                    if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                    {
                        InternalCalls.RigidBodySetForce(hori_movement*move_speed, 0f);
                        Jump();
                        jump_amount--;
                    }
                }


                
            }
            Xforce += hori_movement * move_speed;
            //check for ground



            if (canDash && isDashing==false && Reward_Dash)
            {
                if (InternalCalls.KeyPressed((int)KeyCodes.LeftShift)) {
                   isDashing = true;
                }

            }
            if (isDashing) {
                isGrounded = false;
                if (bullet_time_timer > 0)
                {
                    bullet_time_timer-=InternalCalls.GetDeltaTime();
                    InternalCalls.RigidBodySetForce(0, 0);

                    //Get mouse
                    Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());
                    
                    float angle = CustomMath.AngleBetweenPoints(player_pos, mouse_pos);

                    hori_movement = 0;
                    InternalCalls.SetSpriteImage(player_idle);

                    if (mouse_pos.x>player_pos.x) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } } else { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }

                    apply_force = Vector2D.DirectionFromAngle(angle);
                    InternalCalls.DrawLineBetweenPoints(player_pos.x,player_pos.y, mouse_pos.x, mouse_pos.y);
                    InternalCalls.DrawCircle(player_pos.x,player_pos.y,trans_scaling.x*50* (bullet_time_timer/bullet_time_set),trans_scaling.y* 50 * (bullet_time_timer / bullet_time_set));
                    InternalCalls.DrawCircle(player_pos.x,player_pos.y,trans_scaling.x*50* (bullet_time_timer/2f/bullet_time_set),trans_scaling.y* 50 * (bullet_time_timer / 2f / bullet_time_set));
                    InternalCalls.DrawCircle(player_pos.x,player_pos.y,trans_scaling.x*50* (bullet_time_timer/ 3f / bullet_time_set),trans_scaling.y* 50 * (bullet_time_timer / 3f / bullet_time_set));
                    InternalCalls.DrawCircle(player_pos.x,player_pos.y,trans_scaling.x*50* (bullet_time_timer/ 4f / bullet_time_set),trans_scaling.y* 50 * (bullet_time_timer / 4f / bullet_time_set));




                }
                else
                {
                    Dashing();
                }
            }

            FloorCheckerUpdate();
            WallCheckerUpdate();
            InternalCalls.TransformSetScale(trans_scaling.x, trans_scaling.y);//setting image flips
            InternalCalls.RigidBodyAddForce(Xforce *InternalCalls.GetDeltaTime(), Yforce * InternalCalls.GetDeltaTime());
            Xforce = 0f;Yforce = 0f;



        }

        static public void CleanUp()
        {
            InternalCalls.FreeSpriteImage(player_climb);
            InternalCalls.FreeSpriteImage(player_climb1);
            InternalCalls.FreeSpriteImage(player_climb2);
            InternalCalls.FreeSpriteImage(player_climb3);
            InternalCalls.FreeSpriteImage(player_climb4);
            InternalCalls.FreeSpriteImage(player_climb5);
            InternalCalls.FreeSpriteImage(player_climb6);
            InternalCalls.FreeSpriteImage(player_climb7);
            InternalCalls.FreeSpriteImage(player_climb8);
            InternalCalls.FreeSpriteImage(player_climb9);
            InternalCalls.FreeSpriteImage(player_climb10);
            InternalCalls.FreeSpriteImage(player_climb11);
            InternalCalls.FreeSpriteImage(player_idle);
            InternalCalls.FreeSpriteImage(player_walk);
            InternalCalls.FreeSpriteImage(player_transparent);
            InternalCalls.FreeSpriteImage(player_land);
            InternalCalls.DestroyEntity(entityA);
            InternalCalls.DestroyEntity(entityWall);
            InternalCalls.DestroyEntity(land_entity);
            
        }



        static private void Dashing() {
            for (int i = 0; i < 36; i++)
            {

                InternalCalls.GameSpawnParticleExtra(player_pos.x, player_pos.y, -1 ^ i * 10, 20 * (bullet_time_timer / bullet_time_set), 2, 1, -0.005f, bullet_time_set, 1000, "Particle Test.txt");
                

            }

            InternalCalls.GameSpawnParticleExtraImage(player_pos.x, player_pos.y,
                                                        0.0f, trans_scaling.x*2, trans_scaling.y*2, 1, 0.8f, -0.1f, 1,
                                                        0, "Particle Empty.txt", "Dash AfterImage.png");
            canDash = false;
            isDashing = true;
            dash_timer -= InternalCalls.GetDeltaTime();

            if (dash_timer<=0)
            {
                isDashing = false;
                dash_timer = dash_set;
                bullet_time_timer = bullet_time_set;
                InternalCalls.RigidBodySetForce(0f, 0f);
                return;
            }

            InternalCalls.RigidBodySetForce(apply_force.x*dashSpeed  *-1, apply_force.y *dashSpeed *-1);

        }

        static private void Jump()
        {
            InternalCalls.RigidBodyAddForce(0, jumpHeight);
        }

        static private void FloorCheckerUpdate()
        {

            //Player x y coord
            xCoord = InternalCalls.GetTransformPosition().x;
            yCoord = InternalCalls.GetTransformPosition().y;
            float rotationAngle = InternalCalls.GetTransformRotation();
            float angleRadians = rotationAngle * (CustomMath.PI / 180.0f);
            float distanceBelow = height / 2f;

            Vector2D relativePosition = new Vector2D(0, distanceBelow);

            // Apply rotation to the relative position
            // Rotation matrix in 2D: [cos(theta), -sin(theta); sin(theta), cos(theta)]
            Vector2D rotatedRelativePosition = new Vector2D(
                (float)(relativePosition.x * CustomMath.Cos(angleRadians) + relativePosition.y * CustomMath.Sin(angleRadians)),
                (float)(relativePosition.x * CustomMath.Sin(angleRadians) - relativePosition.y * CustomMath.Cos(angleRadians))
                );


            // Calculate the absolute position for the floor checker
            Vector2D checkerPosition = new Vector2D(
                xCoord + rotatedRelativePosition.x,
                yCoord + rotatedRelativePosition.y
            );

            // Set the floor checker's position
            InternalCalls.TransformSetPositionEntity(checkerPosition.x, checkerPosition.y, entityA);
            InternalCalls.TransformSetRotationEntity(rotationAngle, 0, entityA);
            InternalCalls.TransformSetScaleEntity(width/2f, 2f, entityA);

        }

        static private void WallCheckerUpdate()
        {
            if (hori_movement == 0) { InternalCalls.TransformSetPositionEntity(-999, -99999, entityWall);  return; }
            xCoord = InternalCalls.GetTransformPosition().x;
            yCoord = InternalCalls.GetTransformPosition().y;
            float rotationAngle = InternalCalls.GetTransformRotation();
            float angleRadians = rotationAngle * (CustomMath.PI / 180.0f);
            float distanceLeft = width * -hori_movement;

            Vector2D relativePosition = new Vector2D(distanceLeft  , 0);

            // Apply rotation to the relative position
            Vector2D rotatedRelativePosition = new Vector2D(
                (float)(relativePosition.x * CustomMath.Cos(angleRadians) + relativePosition.y * CustomMath.Sin(angleRadians)),
                (float)(relativePosition.x * CustomMath.Sin(angleRadians) - relativePosition.y * CustomMath.Cos(angleRadians))
            );

            // Calculate the absolute position for the wall checker
            Vector2D checkerPosition = new Vector2D(
                xCoord + rotatedRelativePosition.x,
                yCoord + rotatedRelativePosition.y
            );

            InternalCalls.TransformSetPositionEntity(checkerPosition.x, checkerPosition.y, entityWall);
            InternalCalls.TransformSetRotationEntity(rotationAngle, 0, entityWall);
            InternalCalls.TransformSetScaleEntity(2f, height/2f, entityWall);
            
        }





    }



}

