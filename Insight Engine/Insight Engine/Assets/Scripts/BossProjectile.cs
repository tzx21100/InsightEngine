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

            SimpleVector2D pos = InternalCalls.GetTransformPosition();
            if (bullet_direction.ContainsKey(entity_id))
            {
                InternalCalls.TransformSetPosition(pos.x + bullet_direction[entity_id].Item1, pos.y + bullet_direction[entity_id].Item2);
            }


            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1 &&PlayerScript.invulnerable==false)
            {
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

    }
}
