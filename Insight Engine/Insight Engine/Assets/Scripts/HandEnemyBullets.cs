
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
            if (bullets.ContainsKey(bullet_id))
                bullets[bullet_id].update(); // Call Update on each bullet instance

        }
    }

    class EachBullet
    {
        public float speed = 200f;
        public Vector2D direction = new Vector2D(0f, 0f);
        public Vector2D pos = new Vector2D(0f, 0f);
        private Vector2D scaling = new Vector2D(100f, 100f);

        static public int BULLET_ID;

        SimpleImage bullet_image;
        public int attack_damage = 1;

        public void Init()
        {
            BULLET_ID = InternalCalls.GetCurrentEntityID();

            bullet_image = InternalCalls.GetSpriteImage("dark_circle.png");
        }

        public void update()
        {
            BULLET_ID = InternalCalls.GetCurrentEntityID();
            // update bullet position
            //UpdatePosition();

            // draw bullet
            //DrawBullet();

            // check collision with player
            CheckCollideWithPlayer();

            //Console.WriteLine(direction.x);
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

                    // remove out from dic
                    if (HandEnemyBullets.bullets.ContainsKey(BULLET_ID))
                        HandEnemyBullets.bullets.Remove(BULLET_ID);
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
    }
}
