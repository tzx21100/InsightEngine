
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System;
using System.Linq.Expressions;
using System.ComponentModel;
using static IS.BossBattle;

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
        public float speed = 800f;
        public Vector2D direction = new Vector2D(0f, 0f);
        public Vector2D pos = new Vector2D(0f, 0f);
        public Vector2D scaling = new Vector2D(160.5f, 40f);

        // life time
        public float max_life_timer = 2f;
        public float life_timer = 0f;
        public bool is_alive = true;

        public bool being_hit_by_weapon = false;

        static public int BULLET_ID;

        SimpleImage bullet_image;
        public int attack_damage = 1;

        public void Init()
        {
            BULLET_ID = InternalCalls.GetCurrentEntityID();

            bullet_image = InternalCalls.GetSpriteImage("HandEnemy bullet.png");
            InternalCalls.TransformSetScaleEntity(scaling.x, scaling.y, BULLET_ID);
            life_timer = max_life_timer;
            is_alive = true;
            //Console.WriteLine("spawn-");
            //Console.WriteLine(BULLET_ID);
        }

        public void update()
        {
            BULLET_ID = InternalCalls.GetCurrentEntityID();
            pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(BULLET_ID));
            //InternalCalls.TransformSetPositionEntity(pos.x, pos.y, BULLET_ID);
            // update bullet position
            //UpdatePosition();
            
            // draw bullet
            //DrawBullet();

            // check collision with player
            CheckCollideWithPlayer();
            CheckLifeTime();

            // render bullet flying particles
            RenderFlyingParticles();
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

                    //RenderDestoryParticle();
                    is_alive = false;

                }

                // if bullet collide with ground or wall, destory it
                if (InternalCalls.CompareEntityCategory(BULLET_ID, "Ground") ||
                    InternalCalls.CompareEntityCategory(BULLET_ID, "Wall") ||
                    InternalCalls.CompareEntityCategory(BULLET_ID, "RightHandBoss") ||
                    InternalCalls.CompareEntityCategory(BULLET_ID, "LeftHandBoss"))
                {
                    is_alive = false; // destory bullet if collide with sth other than player
                }

                // check collide with player attack weapon
                if (InternalCalls.CompareEntityCategory(BULLET_ID, "Weapon"))
                {
                    if (!being_hit_by_weapon) // check collide once only
                    {
                        // reverse bullet
                        InternalCalls.TransformSetScaleEntity(scaling.x * -1, scaling.y, BULLET_ID);

                        SimpleVector2D vel = InternalCalls.RigidBodyGetVelocityEntity(BULLET_ID);
                        InternalCalls.RigidBodySetVelocityEntity(vel.x * -1, vel.y, BULLET_ID); // make the bullet fly backs
                        being_hit_by_weapon = true;
                    }
                    
                }

                // check collide with other bullets, destory both bullets when colliding
                if (InternalCalls.CompareEntityCategory(BULLET_ID, "Bullet"))
                {
                    for (int i = 0; i < array.length; i++)
                    {
                        int entity = array.GetValue(i);
                        if (InternalCalls.CheckEntityCategory(entity, "Bullet"))
                        {
                            if (HandEnemyBullets.bullets.ContainsKey(entity))
                            {
                                HandEnemyBullets.bullets[entity].is_alive = false;
                            }
                            
                        }
                    }
                    is_alive = false;

                }

                // check collide with enemy
                if (InternalCalls.CompareEntityCategory(BULLET_ID, "Enemy"))
                {
                    for (int i = 0; i < array.length; i++)
                    {
                        int entity = array.GetValue(i);
                        if (InternalCalls.CheckEntityCategory(entity, "Enemy"))
                        {
                            if (HandEnemy.enemies.ContainsKey(entity))
                            {
                                HandEnemy.enemies[entity].current_state = HandEnemyState.BEING_HIT;
                                HandEnemy.enemies[entity].health -= 20;
                            }
                        }
                    }

                    is_alive = false;
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
                }
            }
            else // bullet not alive, destory it
            {
                // destory bullet and render destory particle
                DestoryBullet();
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

            InternalCalls.SetLightIntensityEntity(BULLET_ID, 0.8f);
        }

        private void RenderDestoryParticle()
        {
            // render particle
            //Console.WriteLine("Destory and Render particle");
            //Console.WriteLine(BULLET_ID);

            // load destory bullet particles
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            int particle_count = (int)(my_rand.Next(20, 31)); // random from 20 to 30 particles
            
            for (int i = 0; i < particle_count; i++)
            {
                int rand_color = (int)(my_rand.Next(0, 3)); // random color

                float rand = my_rand.NextFloat();
                float dir = 360 * rand;

                rand = my_rand.NextFloat();
                float size = 20f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = -10 * rand; // pariticles going smaller

                rand = my_rand.NextFloat();
                float alpha = 0.6f + 0.3f * rand; // 0.6 to 0.9

                rand = my_rand.NextFloat();
                float lifetime = 0.3f + 0.3f * rand; // 0.3s to 0.6s

                rand = my_rand.NextFloat();
                float speed = 300f + 200f * rand;

                rand = my_rand.NextFloat();
                float x = pos.x;

                rand = my_rand.NextFloat();
                float y = pos.y;

                switch (rand_color)
                {
                    case 0:
                        InternalCalls.GameSpawnParticleExtra(x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bullet Flying.txt");
                        break;
                    case 1:
                        InternalCalls.GameSpawnParticleExtra(x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bullet Flying2.txt");
                        break;
                    case 2:
                        InternalCalls.GameSpawnParticleExtra(x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bullet Flying3.txt");
                        break;
                }
            }
        }

        private void RenderFlyingParticles()
        {
            // load flying particles
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            int particle_count = (int)(my_rand.Next(2, 6)); // random from 2 to 5 particles
            SimpleVector2D vel = InternalCalls.RigidBodyGetVelocityEntity(BULLET_ID);
            for (int i = 0; i < particle_count; i++)
            {
                int rand_color = (int)(my_rand.Next(0, 3)); // random color

                float rand = my_rand.NextFloat();
                float dir = (vel.x > 0) ? 180 + 30 * (rand - 0.5f): 0 + 30 * (rand - 0.5f);

                rand = my_rand.NextFloat();
                float size = 5f + 5f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = -20 * rand; // pariticles going smaller

                rand = my_rand.NextFloat();
                float alpha = 0.5f + 0.3f * rand; // 0.6 to 0.9

                rand = my_rand.NextFloat();
                float lifetime = 0.2f + 0.2f * rand; // 0.3s to 0.6s

                rand = my_rand.NextFloat();
                float speed = 300f + 200f * rand;

                rand = my_rand.NextFloat();
                float x = (vel.x > 0) ? pos.x - scaling.x / 2f * (rand) : pos.x + scaling.x / 2f * (rand);

                rand = my_rand.NextFloat();
                float y = pos.y + scaling.x / 2f * (rand - 0.5f);

                switch (rand_color)
                {
                    case 0:
                        InternalCalls.GameSpawnParticleExtra(x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bullet Flying.txt");
                        break;
                    case 1:
                        InternalCalls.GameSpawnParticleExtra(x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bullet Flying2.txt");
                        break;
                    case 2:
                        InternalCalls.GameSpawnParticleExtra(x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bullet Flying3.txt");
                        break;
                }
            }
        }
    }
}
