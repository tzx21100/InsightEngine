using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class NarratorVoices
    {

        static bool announce_help = true;

        static public void Init()
        {
            announce_help = true;
        }


        static public void Update()
        {
            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1 && announce_help)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_3.wav", false, 0.4f);
                announce_help = false;
            }
        }

        static public void CleanUp(){

        }



        static public float narrate_timer = 0f;
        static public float narrater_timer_set = 60f;
        static public void NarrateUpdate()
        {
            narrate_timer-=InternalCalls.GetDeltaTime();
            if (narrate_timer < 0f)
            {
                narrate_timer = narrater_timer_set;
                NarrateAnnoying();
            }
        }

        static public void NarrateUpdateFlight()
        {
            narrate_timer -= InternalCalls.GetDeltaTime();
            if (narrate_timer < 0f)
            {
                narrate_timer = narrater_timer_set;
                NarrateFlight();
            }
        }



        static MyRandom rando_footsteps = new MyRandom(12312415);
        static public void NarrateAnnoying()
        {
            float volume = 0.5f;
            float num = rando_footsteps.NextFloat();

            if (num <= 0.16f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_4.wav", false, volume);
                return;
            }
            if (num <= 0.32f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_5.wav", false, volume);
                return;
            }
            if (num <= 0.48f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_6.wav", false, volume);
                return;
            }
            if (num <= 0.64f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_7.wav", false, volume);
                return;
            }
            if (num <= 0.80f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_8.wav", false, volume);
                return;
            }
            if (num <= 1f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_9.wav", false, volume);
                return;
            }  
        }


        static public void NarrateFlight()
        {
            float volume = 0.5f;
            float num = rando_footsteps.NextFloat();

            if (num <= 0.16f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Flight_2.wav", false, volume);
                return;
            }
            if (num <= 0.32f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Flight_1.wav", false, volume);
                return;
            }
            if (num <= 0.48f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Flight_1.wav", false, volume);
                return;
            }
            if (num <= 0.64f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Flight_2.wav", false, volume);
                return;
            }
            if (num <= 0.80f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Flight_3.wav", false, volume);
                return;
            }
            if (num <= 1f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Flight_3.wav", false, volume);
                return;
            }
        }


        static public void NarrateDeath()
        {
            //reset narration timer
            narrate_timer = narrater_timer_set;
            InternalCalls.AudioStopAllSounds();

            float volume = 0.5f;
            float num = rando_footsteps.NextFloat();

            if (num <= 0.16f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_1.wav", false, volume);
                return;
            }
            if (num <= 0.32f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_2.wav", false, volume);
                return;
            }
            if (num <= 0.48f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_3.wav", false, volume);
                return;
            }
            if (num <= 0.64f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_4.wav", false, volume);
                return;
            }
            if (num <= 0.80f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_5.wav", false, volume);
                return;
            }
            if (num <= 1f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_5.wav", false, volume);
                return;
            }
        }

        static public void NarrateDeathFlight()
        {
            //reset narration timer
            narrate_timer = narrater_timer_set;


            float volume = 0.5f;
            float num = rando_footsteps.NextFloat();

            if (num <= 0.16f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_1.wav", false, volume);
                return;
            }
            if (num <= 0.32f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Flight_Death.wav", false, volume);
                return;
            }
            if (num <= 0.48f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Flight_Death.wav", false, volume);
                return;
            }
            if (num <= 0.64f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_4.wav", false, volume);
                return;
            }
            if (num <= 0.80f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_5.wav", false, volume);
                return;
            }
            if (num <= 1f)
            {
                InternalCalls.AudioPlaySoundSFX("Narrator_Death_VO_5.wav", false, volume);
                return;
            }
        }



    }
}
