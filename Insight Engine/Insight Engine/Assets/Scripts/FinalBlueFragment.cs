using System.Runtime.CompilerServices;
namespace IS
{
    class FinalBlueFragment
    {
        static public void Init(){

        }

        static public void Update(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                int grabber=InternalCalls.CreateEntityPrefab("BossGrab");
                SimpleVector2D pos = InternalCalls.GetTransformPosition();
                InternalCalls.TransformSetPositionEntity(pos.x,pos.y,grabber);
                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());

            }
        }
        
        static public void CleanUp(){

        }

    }
}
