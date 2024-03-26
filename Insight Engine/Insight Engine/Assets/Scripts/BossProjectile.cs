using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace IS
{
    class BossProjectile
    {

        static public Dictionary<int, (float,float)> bullet_direction = new Dictionary<int, (float,float)>();
        static public void Init(){

        }

        static public void Update(){

            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if (PlayerScript.attack_trigger)
            {
                Console.WriteLine("Calling from entity" + entity_id + "ARray size: " + array.length);
            }
           

            SimpleVector2D pos = InternalCalls.GetTransformPosition();
            if (bullet_direction.ContainsKey(entity_id))
            {
                InternalCalls.TransformSetPosition(pos.x + bullet_direction[entity_id].Item1, pos.y + bullet_direction[entity_id].Item2);
                ParticleTrail();
            }


            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1 &&PlayerScript.invulnerable==false)
            {
                Console.WriteLine("Collision Happened At playerid: "+ PlayerScript.PLAYER_ID);
                PlayerScript.Health -= 1;
                DestroySelf();
                
            }

            foreach (var key in Enemy.enemies.Keys)
            {
                if (array.FindIndex(key) != -1)
                {
                    Enemy.enemies[key].health = 0;
                    DestroySelf();
                }
            }

            foreach (var key in HandEnemy.enemies.Keys)
            {
                if (array.FindIndex(key) != -1)
                {
                    HandEnemy.enemies[key].health = 0;
                    DestroySelf();
                }
            }

            for (int i = 0; i < array.length; i++)
            {
                int entity = array.GetValue(i);
                if (InternalCalls.RigidBodyGetBodyTypeEntity(entity) == 5)
                {
                    DestroySelf();

                }
            }

        }
        
        static public void CleanUp(){
            
        }

        static private void DestroySelf()
        {
            int entity_id = InternalCalls.GetCurrentEntityID();
            InternalCalls.DestroyEntity(entity_id);
            bullet_direction.Remove(entity_id);
        }

        static void ParticleTrail()
        {
            MyRandom rand = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));

            SimpleVector2D my_position = InternalCalls.GetTransformPosition();

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


    }
}
