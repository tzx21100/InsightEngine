using System.Runtime.CompilerServices;
namespace IS
{

    class BreakableWalls
    {
        static Dictionary<int, int> created_entities = new Dictionary<int, int>();


        static public void Init(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            created_entities[entity_id] = new int();
        }

        static public void Update(){
            if (InternalCalls.GetCollidingEntityCheck(InternalCalls.GetCurrentEntityID(), PlayerScript.PLAYER_ID) && InternalCalls.RigidBodyGetBodyTypeEntity(InternalCalls.GetCurrentEntityID())==0)
            {
                if (PlayerScript.isDashing && PlayerScript.bullet_time_timer<=0) {


                    InternalCalls.RigidBodySetBodyTypeEntity(1, InternalCalls.GetCurrentEntityID());

                    int entity = InternalCalls.CreateEntitySprite("BROKEN WALL");
                    
                    Vector2D trans =  Vector2D.FromSimpleVector2D( InternalCalls.GetTransformScaling());
                    Vector2D pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
                    InternalCalls.RigidBodySetBodyTypeEntity(1,entity);
                    InternalCalls.ColliderComponentAdd(entity,1,1);
                    InternalCalls.TransformSetPositionEntity(pos.x, pos.y,entity);
                    InternalCalls.TransformSetScale(trans.x / 2f, trans.y );
                    InternalCalls.TransformSetScaleEntity(trans.x / 2, trans.y , entity);


                }
            }
        }



        static public void CleanUp(){

        }

    }




}
