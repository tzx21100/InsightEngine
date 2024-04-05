using System.Runtime.CompilerServices;
using System.Security;

namespace IS
{
    class CutSceneScript
    {

        static public string cutsceneVideo = "Assets/Videos/cutscene.mp4";

        static private float image_swap_timer = 3.0f;
        static private float image_swap_set = 3.0f;
        static private int current_image = 1;

        static private float wait_for_video = 1.5f;
        static private float video_timer = 0f;

        static private bool play_yk_voicelines = false;
        static private bool play_first_explosion= false;
        static private bool play_second_explosion=false;
        static private bool play_third_explosion = false;
        static private bool play_audio_collapse = false;


        //setting text window size
        static public float textbox_width_ratio = 0.7f;
        static public float textbox_height_ratio = 0.26f;
        static private float textbox_width = InternalCalls.GetWindowWidth();
        static private float textbox_height = InternalCalls.GetWindowHeight();

        static private SimpleVector2D position_of_textbox = new SimpleVector2D();
        static private SimpleVector2D image_scale = new SimpleVector2D(1f, 1f);

        static private bool showText = false;

        static public void Init()
        {
            video_timer = 0f;

            InternalCalls.ChangeLightType(0);
            play_yk_voicelines = false;
            play_first_explosion = false;
            play_second_explosion = false;
            play_third_explosion = false;
            play_audio_collapse = false;


            InternalCalls.HideCursor();

            current_image = 0;
            image_swap_timer = 3.0f;
            wait_for_video = 1.8f;
            InternalCalls.AudioPlayMusic("BENNY - Jazz MSCLJZ1_52.wav", 0.4f);
            InternalCalls.AttachCamera(0, 0);
            //InternalCalls.AudioPlayMusic("Fragments_Music_Tension 2.wav", 0.4f);

            InternalCalls.loadVideo(cutsceneVideo, 1f, 1f, 0f, 0f, true);
            InternalCalls.SetLightsToggle(true); // to draw videos first then video
                                                  // (so that the textbox can still be shown)
                                                  // will not matter in the future because videos will have
                                                  // their own textboxes. fucking duct tape i know 8(
        }

        static public void Update() {



            InternalCalls.AttachCamera(0, 0);
            CameraScript.camera_pos.x = 0;
            CameraScript.camera_pos.y = 0;

            /// FOR M5 PRESENTATION ///
            // InternalCalls.TransformSetScale(InternalCalls.GetWindowWidth(),InternalCalls.GetWindowHeight());

            video_timer += InternalCalls.GetDeltaTime();
            if (video_timer > 7.7f)
            {   if (!play_yk_voicelines)
                {
                    InternalCalls.AudioStopAllSounds();
                    InternalCalls.AudioPlayMusicSFX("VO-MadCoder-001.wav", 0.2f);
                    InternalCalls.AudioPlayMusicSFX("SCI-FI-AMBIENCE_GEN-HDF-20503.wav", 0.1f);
                    play_yk_voicelines = true;
                }
            }

            if (video_timer > 18f)
            {
                if (!play_first_explosion)
                {
                    InternalCalls.AudioStopAllSounds();
                    InternalCalls.AudioPlayMusicSFX("SCI-FI-EXPLOSION_GEN-HDF-20662.wav", 0.2f);
                    play_first_explosion = true;
                }
            }

            if (video_timer > 20.2f)
            {
                if (!play_second_explosion)
                {
                    //InternalCalls.AudioStopAllSounds();
                    InternalCalls.AudioPlayMusicSFX("EXPLOSION-LARGE_GEN-HDF-10849.wav", 0.2f);
                    InternalCalls.AudioPlayMusicSFX("Fragments_Music_Tension 2.wav", 0.3f);
                    InternalCalls.AudioPlayMusicBGM("JOHN_The Choice_Loop_60bpm.wav", 0.3f);
                    play_second_explosion = true;
                }
            }

            if (video_timer > 38f)
            {
                if (!play_third_explosion)
                {

                    InternalCalls.AudioPlayMusic("SCI-FI-GROUND-BREAK_GEN-HDF-20675.wav", 0.2f);
                    play_third_explosion = true;
                }
            }

            if (video_timer > 39f)
            {
                if (!play_audio_collapse)
                {
                    InternalCalls.AudioPlayMusicSFX("ROCK-COLLAPSE_GEN-HDF-20004.wav", 0.1f);
                    play_audio_collapse = true;
                }
            }

            if (video_timer > 43.6f || InternalCalls.KeyPressed((int)KeyCodes.Escape))
            {
                InternalCalls.AudioStopAllSounds();
                InternalCalls.LoadScene("Assets/Scenes/CaveLevel.insight");
                InternalCalls.unloadVideos();
                InternalCalls.SetLightsToggle(true);
            }

            InternalCalls.RenderTextFont("Press 'ESC' to skip cutscene", "Semplicita_Light", 0.9f, 0.95f, 8f, (1f, 1f, 1f, 1f));
        }
        
        static public void CleanUp()
        {
            InternalCalls.AudioStopAllSounds();
            InternalCalls.ShowCursor();
            InternalCalls.LoadScene("Assets/Scenes/CaveLevel.insight");
            InternalCalls.unloadVideos();
            InternalCalls.SetLightsToggle(true);
        }

    }
}
