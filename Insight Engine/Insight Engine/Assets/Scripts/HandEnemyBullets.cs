
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System;
using System.Linq.Expressions;
using System.ComponentModel;
namespace IS
{
    class HandEnemyBullets
    {
        static public Dictionary<int, EachBullet> bullets = new Dictionary<int, EachBullet>();

        static public void Init()
        {
            int bullet_id = InternalCalls.GetCurrentEntityID();
            bullets[bullet_id] = new EachBullet();
            bullets[bullet_id].Init();
        }

        static public void Update()
        {
            int bullet_id = InternalCalls.GetCurrentEntityID();
            if (bullets.ContainsKey(bullet_id)) // only update when it is exist in the dictionary
                bullets[bullet_id].update();

        }
    }

    class EachBullet
    {
        public float speed = 200f;
        public Vector2D direction = new Vector2D(0f, 0f);
        public Vector2D pos = new Vector2D(0f, 0f);
        private Vector2D scaling = new Vector2D(100f, 100f);

        // life time
        public float max_life_timer = 2f;
        public float life_timer = 0f;
        public bool is_alive = true;

        static public int BULLET_ID;

        SimpleImage bullet_image;
        public int attack_damage = 1;

        public void Init()
        {
            BULLET_ID = InternalCalls.GetCurrentEntityID();

            bullet_image = InternalCalls.GetSpriteImage("dark_circle.png");

            life_timer = max_life_timer;
            is_alive = true;
        }

        public void update()
        {
            BULLET_ID = InternalCalls.GetCurrentEntityID();
            //pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(BULLET_ID));
            //InternalCalls.TransformSetPositionEntity(pos.x, pos.y, BULLET_ID);
            // update bullet position
            //UpdatePosition();

            // draw bullet
            //DrawBullet();

            // check collision with player
            CheckCollideWithPlayer();
            CheckLifeTime();
        }

        public void CleanUp()
        {
            
        }

        public void UpdateBullet(int id, Vector2D dir, Vector2D position)
        {
            HandEnemyBullets.bullets[id].direction = dir;
            HandEnemyBullets.bullets[id].pos = position;

        }

        private void UpdatePosition()
        {
            SimpleVector2D p = InternalCalls.GetTransformPositionEntity(BULLET_ID);

            p.x += speed * direction.x * InternalCalls.GetDeltaTime();
            p.y += speed * direction.y * InternalCalls.GetDeltaTime(); // actually 0

            InternalCalls.TransformSetPositionEntity(p.x, p.y, BULLET_ID);

            pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(BULLET_ID));
        }

        private void DrawBullet()
        {
            
            InternalCalls.DrawImageAt
                (
                    new SimpleVector2D(pos.x, pos.y), 0, new SimpleVector2D(scaling.x, scaling.y), bullet_image, 0.8f, 6
                );
        }

        private void CheckCollideWithPlayer()
        {
            // check enemy colliding with enemy
            if (InternalCalls.OnCollisionEnter())
            {
                SimpleArray array = InternalCalls.GetCollidingEntityArray(BULLET_ID);

                if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
                //if (InternalCalls.CompareCategory("Player"))
                {
                    PlayerScript.is_colliding_enemy = true;
                    PlayerScript.colliding_enemy_id = BULLET_ID;
                    PlayerScript.colliding_enemy_type = (int)EnemyType.Bullet;

                    // damage player and destory bullet was handled in player script
                    // render destory particle
                    RenderDestoryParticle();

                }

                /*if (InternalCalls.CompareCategory("Weapon"))
                {
                    PlayerScript.hitting_enemy_id = ENEMY_ID;
                    BEING_ATTACK_ENEMY_ID = ENEMY_ID;
                }
                else
                {
                    PlayerScript.hitting_enemy_id = -1;
                    BEING_ATTACK_ENEMY_ID = -1;
                }*/
            }
        }

        private void CheckLifeTime()
        {
            if (is_alive)
            {
                life_timer -= InternalCalls.GetDeltaTime();

                if (life_timer < 0f)
                {
                    is_alive = false;

                    // destory bullet and render destory particle
                    DestoryBullet();
                }
            }
        }

        private void DestoryBullet()
        {
            if (HandEnemyBullets.bullets.ContainsKey(BULLET_ID))
            {
                HandEnemyBullets.bullets.Remove(BULLET_ID);
                InternalCalls.DestroyEntity(BULLET_ID);
            }
            // render particle
            RenderDestoryParticle();
        }

        private void RenderDestoryParticle()
        {
            // render particle
            Console.WriteLine("Render particle");
        }
    }
}
