using System.Runtime.CompilerServices;
namespace IS
{
    class NarrateEnemy
    {
        static public void Init(){

        }

        static public void Update()
        {

            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Enemy_Alert.wav", false, 0.4f);
                NarratorVoices.narrate_timer = NarratorVoices.narrater_timer_set;
                InternalCalls.DestroyEntity(entity_id);

            }


        }

        static public void CleanUp(){

        }

    }
}
