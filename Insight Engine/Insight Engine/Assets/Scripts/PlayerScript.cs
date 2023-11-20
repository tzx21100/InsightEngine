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
        
        //private entity

        static SimpleImage player_walk;
        static SimpleImage player_idle;
        static SimpleImage player_climb;
        static SimpleImage player_transparent;



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

        //dashing 
        static private float bullet_time_timer = 1f;
        static private float bullet_time_set = 1f;
        static private float dash_timer =0.2f;
        static private float dash_set = 0.2f;
        static private bool canDash = false;
        static private bool isDashing;
        static private float dashAngle;
        static private float dashSpeed = 3000f;

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
            player_transparent = InternalCalls.GetSpriteImage("transparent.png");

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

        }

        static public void Update()
        {
            if (GameManager.isGamePaused == true) {
                return;
            }
            
            //Attach Camera
            InternalCalls.AttachCamera();

            //movement
            hori_movement = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.D)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.A));
           

            // scaling transform with movement
            Vector2D trans_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
            Vector2D trans_scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
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




            

            WallCheckerUpdate();
            //wall checking
            if (InternalCalls.EntityCheckCollide(entityWall) && hori_movement!=0 && InternalCalls.GetCollidingEntity(entityWall)!= InternalCalls.GetCurrentEntityID() && InternalCalls.CollidingObjectIsStatic(InternalCalls.GetCollidingEntity(entityWall)))
            {
                if (!isClimbing) { InternalCalls.RigidBodySetForce(0, 0); }
                isClimbing = true;   
            }
            else { isClimbing = false; }

            if(isClimbing) {
                InternalCalls.SetSpriteImage(player_transparent);
                InternalCalls.SetSpriteAnimationIndex(0);
                
                float collided_angle = InternalCalls.GetCollidedObjectAngle(entityWall) - (90 * hori_movement);

                InternalCalls.TransformSetRotation(collided_angle, 0);
                Vector2D force_from_angle = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(collided_angle*-hori_movement));

                InternalCalls.RigidBodySetForce(climbSpeed  *force_from_angle.x * hori_movement, climbSpeed *force_from_angle.y );


                float x_offset = 28 * hori_movement;
                SimpleVector2D pos = InternalCalls.GetTransformPosition();
                pos.x += x_offset;
                SimpleVector2D scale = new SimpleVector2D(InternalCalls.GetTransformScaling().x* 1.2f, InternalCalls.GetTransformScaling().y * 1.2f);
                InternalCalls.DrawImageAt(pos, 0f, scale, player_climb, 1);

            }
            else
            {
                InternalCalls.TransformSetScale(1f,1f);
            }

            //jumping bool
            if (isJumping && jump_timer > 0f) { jump_timer -= InternalCalls.GetDeltaTime(); if (jump_timer <= 0) { isJumping = false; jump_timer = jump_timer_set; } }
                //if is grounded
            if (InternalCalls.EntityCheckCollide(entityA) && isClimbing==false && InternalCalls.GetCollidingEntity(entityA) != InternalCalls.GetCurrentEntityID())
            {

                isGrounded = true;
            }
            else
            {
                isGrounded = false;
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
                InternalCalls.RigidBodyAddForce(hori_movement * move_speed * InternalCalls.GetDeltaTime(), 0f);
                trans_rotate = 0;
                InternalCalls.TransformSetRotation(trans_rotate, 0);

                if (jump_amount > 0)
                {
                    if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                    {
                        InternalCalls.RigidBodySetForce(hori_movement*move_speed, 0f);
                        Jump();
                        jump_amount--;
                    }
                }
            }

            //check for ground



            if (canDash && isDashing==false)
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
                    Vector2D player_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
                    float angle = CustomMath.AngleBetweenPoints(player_pos, mouse_pos);

                    if (angle > -CustomMath.PI / 4 && angle < CustomMath.PI / 4) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } } else { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }

                    apply_force = Vector2D.DirectionFromAngle(angle);
                    InternalCalls.DrawLineBetweenPoints(player_pos.x,player_pos.y, mouse_pos.x, mouse_pos.y);
                    InternalCalls.DrawCircle(player_pos.x,player_pos.y,trans_scaling.x* (bullet_time_timer/bullet_time_set),trans_scaling.y* (bullet_time_timer / bullet_time_set));

                }
                else
                {
                    Dashing();
                }
            }


            InternalCalls.TransformSetScale(trans_scaling.x, trans_scaling.y);//setting image flips
            FloorCheckerUpdate();




        }

        static public void CleanUp()
        {
            InternalCalls.FreeSpriteImage(player_climb);
            InternalCalls.FreeSpriteImage(player_idle);
            InternalCalls.FreeSpriteImage(player_walk);
            InternalCalls.FreeSpriteImage(player_transparent);
            InternalCalls.DestroyEntity(entityA);
            InternalCalls.DestroyEntity(entityWall);
            
        }



        static private void Dashing() {
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
            InternalCalls.RigidBodyAddForce(0f, 1000f);
        }

        static private void FloorCheckerUpdate()
        {

            //Player x y coord
            xCoord = InternalCalls.GetTransformPosition().x;
            yCoord = InternalCalls.GetTransformPosition().y;
            float rotationAngle = InternalCalls.GetTransformRotation();
            float angleRadians = rotationAngle * (CustomMath.PI / 180.0f);
            float distanceBelow = height / 1.8f;

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
            float distanceLeft = width ;

            Vector2D relativePosition = new Vector2D(distanceLeft *hori_movement , 0);

            // Apply rotation to the relative position
            Vector2D rotatedRelativePosition = new Vector2D(
                (float)(relativePosition.x * CustomMath.Cos(angleRadians) - relativePosition.y * CustomMath.Sin(angleRadians)),
                (float)(relativePosition.x * CustomMath.Sin(angleRadians) + relativePosition.y * CustomMath.Cos(angleRadians))
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

