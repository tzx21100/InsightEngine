using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace IS
{
    class Firefly
    {

        static private List<int> firefly_list = new List<int>();
        static private Dictionary<int, (Vector2D dir, float speed )> firefly_dir = new Dictionary<int, (Vector2D, float)>(); // keep a map of projectiles and the speed and direction
        static private Dictionary<int,float> firefly_lifetime = new Dictionary<int, float>();
        static private Dictionary<int, float> firefly_flickerlifetime = new Dictionary<int, float>();

        static float lifespan = 5f;
        static float direction;
        static public void Init(){
            ClearAll();
        }

        static public void Update(){

            for(int i=0; i < firefly_list.Count; i++)
            {


                int entity = firefly_list[i];

                if (!firefly_dir.ContainsKey(entity) || !firefly_lifetime.ContainsKey(entity) || !firefly_flickerlifetime.ContainsKey(entity))
                {
                    // Skip this iteration if the entity does not exist in all required dictionaries
                    continue;
                }


                Vector2D dir = firefly_dir[entity].Item1;
                float speed = firefly_dir[entity].Item2;
                float lifetime = firefly_lifetime[entity];
                float flicker = firefly_flickerlifetime[entity];
                SimpleVector2D pos = InternalCalls.GetTransformPositionEntity(entity);

                float new_x = pos.x + dir.x * speed;
                float new_y = pos.y + dir.y * speed;

                flicker -= InternalCalls.GetDeltaTime();
                firefly_flickerlifetime[entity] = flicker;
                if (flicker <= 0)
                {
                    InternalCalls.SetLightIntensityEntity(entity, InternalCalls.GetRandomFloat() + 0.2f);
                    firefly_flickerlifetime[entity] = 2f;
                    dir = new Vector2D(InternalCalls.GetRandomFloat() - 0.5f, InternalCalls.GetRandomFloat() - 0.5f);
                    var tuple1 = (dir, speed);
                    firefly_dir[entity] = tuple1;
                }
                

                InternalCalls.TransformSetPositionEntity(new_x, new_y, entity);

                lifetime -= InternalCalls.GetDeltaTime();
                firefly_lifetime[entity] = lifetime;
                if (lifetime <= 0f)
                {
                    DestroyFirefly(entity);
                }




            }


            //int gameEnt = InternalCalls.CreateEntityPrefab("Firefly");
            /*InternalCalls.TransformSetRotation(direction, 0f);
            Vector2D dir = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(direction));
            dir.Multiply(20f);
            InternalCalls.RigidBodyAddForce(dir.x,dir.y);*/
        }
        
        static public void CleanUp(){
            ClearAll();
        }

        static public void CreateFirefly(string Prefab, float direction, float rotation, float speed, float lifespan, float Xpos, float Ypos)
        {
            int entity = InternalCalls.CreateEntityPrefab(Prefab);
            Vector2D dir = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(direction));
            firefly_dir.TryAdd(entity, (dir, speed)); //add prefab to the dictionary
            firefly_lifetime.TryAdd(entity, lifespan);
            firefly_flickerlifetime.TryAdd(entity, InternalCalls.GetRandomFloat());
            firefly_list.Add(entity);

            //set the positon rotation etc
            InternalCalls.TransformSetPositionEntity(Xpos, Ypos, entity);
            InternalCalls.TransformSetRotationEntity(rotation, 0, entity);
        }



        static public void DestroyFirefly(int entity)
        {
            firefly_list.Remove(entity);
            firefly_dir.Remove(entity);
            firefly_lifetime.Remove(entity);
            firefly_flickerlifetime.Remove(entity);
            InternalCalls.DestroyEntity(entity);
        }

        static void ClearAll()
        {
            for(int i = 0; i<firefly_list.Count; i++)
            {
                DestroyFirefly(firefly_list[i]);
            }

            firefly_dir.Clear();
            firefly_lifetime.Clear();
            firefly_flickerlifetime.Clear();
            firefly_list.Clear();
        }


    }
}
