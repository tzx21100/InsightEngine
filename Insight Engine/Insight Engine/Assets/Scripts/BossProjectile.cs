using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace IS
{
    class BossProjectile
    {
        static public List<int> Projectile_list = new List<int>();
        static public Dictionary<int, (Vector2D dir,float speed)> bullet_direction = new Dictionary<int, (Vector2D,float)>(); // keep a map of projectiles and the speed and direction
        static public void Init(){
            CLEAN();
        }

        static public void Update(){

            //loop through list and update bullet direction
            for (int i = 0; i < Projectile_list.Count; i++)
            {
                int entity_id= Projectile_list[i];
                SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

                //update bullet position
                SimpleVector2D pos = InternalCalls.GetTransformPositionEntity(entity_id);
                float speed = bullet_direction[entity_id].Item2;
                InternalCalls.TransformSetPositionEntity(pos.x + bullet_direction[entity_id].Item1.x * speed, pos.y + bullet_direction[entity_id].Item1.y *speed,entity_id);
                ParticleTrail(entity_id);


                //hit player
                if (array.FindIndex(PlayerScript.PLAYER_ID) != -1 && PlayerScript.invulnerable == false)
                {
                    // Console.WriteLine("Collision Happened At playerid: "+ PlayerScript.PLAYER_ID);
                    PlayerScript.Health -= 1;
                    PlayerScript.screen_flash_timer += 1.5f;
                    PlayerScript.invulnerable = true;
                    PlayerScript.is_colliding_enemy = true;
                    InternalCalls.AudioPlaySoundSFX("DieSound.wav", false, 0.2f);
                    DestroyProjectile(entity_id);
                }

                // hit enemy
                foreach (var key in Enemy.enemies.Keys)
                {
                    if (array.FindIndex(key) != -1)
                    {
                        Enemy.enemies[key].health = 0;
                        DestroyProjectile(entity_id);
                    }
                }

                // hit hand enemy
                foreach (var key in HandEnemy.enemies.Keys)
                {
                    if (array.FindIndex(key) != -1)
                    {
                        HandEnemy.enemies[key].health = 0;
                        DestroyProjectile(entity_id);
                    }
                }

                //hit spikes
                for (int j = 0; j < array.length; j++)
                {
                    int entity = array.GetValue(j);
                    if (InternalCalls.RigidBodyGetBodyTypeEntity(entity) == 5)
                    {
                        DestroyProjectile(entity_id);

                    }
                }


            }






           








        }
        
        static public void CleanUp(){

        }

        static public void CLEAN()
        {
            bullet_direction.Clear();
            Projectile_list.Clear();
        }

        static public void DestroyAllProjectiles()
        {
            while(Projectile_list.Count > 0) 
            {
                int entity_id = Projectile_list[0];
                DestroyProjectile(entity_id);
            }

        }

        static public void DestroyProjectile(int entity)
        {
            bullet_direction.Remove(entity);
            Projectile_list.Remove(entity);
            InternalCalls.DestroyEntity(entity);
        }

        static void ParticleTrail(int entity)
        {
            MyRandom rand = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));

            SimpleVector2D my_position = InternalCalls.GetTransformPositionEntity(entity);

            float direction = rand.NextFloat() * 360;
            float ran = rand.NextFloat();
            float ran2 = rand.NextFloat();
            if (ran2 < 0.3f)
            {
                InternalCalls.GameSpawnParticleExtra(my_position.x, my_position.y + 80f, direction, ran * 10, ran * 2, 0.5f * ran, 0f, 2f * ran, ran * 200, "Particle2.txt");
            }
            else if (ran2 < 0.6f)
            {
                InternalCalls.GameSpawnParticleExtra(my_position.x, my_position.y + 70f, direction, ran * 10, ran * 2, 0.5f * ran, 0f, 2f * ran, ran * 200, "Particle Test.txt");
            }
            else if (ran2 < 0.9f)
            {
                InternalCalls.GameSpawnParticleExtra(my_position.x, my_position.y + 80f, direction, ran * 10, ran * 2, 0.5f * ran, 0f, 2f * ran, ran * 200, "Particle3.txt");
            }
        }



        static public void CreateProjectile(string Prefab ,float direction, float rotation, float speed ,float Xpos,float Ypos )
        {
            int entity =InternalCalls.CreateEntityPrefab(Prefab);
            Vector2D dir = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(direction));
            bullet_direction.Add(entity,(dir,speed)); //add prefab to the dictionary
            Projectile_list.Add(entity);

            //set the positon rotation etc
            InternalCalls.TransformSetPositionEntity(Xpos,Ypos,entity);
            InternalCalls.TransformSetRotationEntity(rotation, 0, entity);


        }

        static public void SetAllProjectileSpeed(float speed)
        {
            for (int i = 0; i < Projectile_list.Count; i++)
            {
                int entity_id = Projectile_list[i];
                if (bullet_direction.ContainsKey(entity_id))
                {
                    Vector2D currentDirection = bullet_direction[entity_id].Item1;

                    // Create a new tuple with the unchanged direction and the new speed
                    var newTuple = (currentDirection, speed);

                    // Update the dictionary with the new tuple
                    bullet_direction[entity_id] = newTuple;

                }
            }
        }

        static public void ReverseAllProjectile()
        {
            for (int i = 0; i < Projectile_list.Count; i++)
            {
                int entity_id = Projectile_list[i];
                if (bullet_direction.ContainsKey(entity_id))
                {
                    float speed = bullet_direction[entity_id].Item2;
                    Vector2D currentDirection = bullet_direction[entity_id].Item1;
                    currentDirection = currentDirection.Multiply(-1f);
                    // Create a new tuple with the unchanged direction and the new speed
                    var newTuple = (currentDirection, speed);

                    // Update the dictionary with the new tuple
                    bullet_direction[entity_id] = newTuple;

                    //flip the rotation of the projectiles
                    float rotate = InternalCalls.GetTransformRotationEntity(entity_id);
                    InternalCalls.TransformSetRotationEntity(rotate + 180, 0, entity_id);

                }
            }
        }



    }
}
