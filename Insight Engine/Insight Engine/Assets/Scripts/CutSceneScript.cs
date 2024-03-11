using System.Runtime.CompilerServices;
using System.Security;

namespace IS
{
    class CutSceneScript
    {
        static private SimpleImage image1 = InternalCalls.GetSpriteImage("1.jpg");
        static private SimpleImage image2 = InternalCalls.GetSpriteImage("2.jpg");
        static private SimpleImage image3 = InternalCalls.GetSpriteImage("3.jpg");
        static private SimpleImage image4 = InternalCalls.GetSpriteImage("4.jpg");
        static private SimpleImage image5 = InternalCalls.GetSpriteImage("5.jpg");
        static private SimpleImage image6 = InternalCalls.GetSpriteImage("6.jpg");
        static private SimpleImage image7 = InternalCalls.GetSpriteImage("7.jpg");
        static private SimpleImage image8 = InternalCalls.GetSpriteImage("8.jpg");
        static private SimpleImage image9 = InternalCalls.GetSpriteImage("9.jpg");
        static private SimpleImage image10 = InternalCalls.GetSpriteImage("10.jpg");
        static private SimpleImage image11 = InternalCalls.GetSpriteImage("11.jpg");
        static private SimpleImage image11_2 = InternalCalls.GetSpriteImage("11_2.jpg");
        static private SimpleImage image12 = InternalCalls.GetSpriteImage("12.jpg");
        static private SimpleImage image13 = InternalCalls.GetSpriteImage("13.jpg");
        static private SimpleImage image14 = InternalCalls.GetSpriteImage("14.jpg");
        static private SimpleImage image15 = InternalCalls.GetSpriteImage("15.jpg");

        static public string cutsceneVideo = "Assets/Videos/cutscene.mov";

        static private float image_swap_timer = 3.0f;
        static private float image_swap_set = 3.0f;
        static private int current_image = 1;

        static private float wait_for_video = 1.5f;

        static private bool play_audio_collapse = false;
        static private bool play_yk_voicelines = false;
        static private bool play_first_explosion= false;
        static private bool play_second_explosion=false;
        static private bool play_third_explosion = false;


        //setting text window size
        static public float textbox_width_ratio = 0.95f;
        static public float textbox_height_ratio = 0.3f;
        static private float textbox_width = InternalCalls.GetWindowWidth();
        static private float textbox_height = InternalCalls.GetWindowHeight();

        static private SimpleVector2D position_of_textbox = new SimpleVector2D();
        static private SimpleVector2D image_scale = new SimpleVector2D(1f, 1f);

        static private bool showText = false;

        static public void Init()
        {
            InternalCalls.ChangeLightType(0);
            play_yk_voicelines = false;
            play_audio_collapse = false;
            play_first_explosion = false;
            play_second_explosion = false;
            play_third_explosion = false;
            showText = false;
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
            InternalCalls.TransformSetScale(0f, 0f); // dont show cutscene images

            image_swap_timer -= InternalCalls.GetDeltaTime();
            if(image_swap_timer < 0)
            {
                if (wait_for_video >= 0) {
                    wait_for_video -= InternalCalls.GetDeltaTime();
                }
                else
                {
                    image_swap_timer = image_swap_set;
                    current_image++;
                    if (current_image == 17)
                    {
                        InternalCalls.LoadScene("Assets/Scenes/CaveLevel.insight");

                        InternalCalls.unloadVideos();
                        InternalCalls.SetLightsToggle(true);
                    }
                }
            }

            if(current_image==0)
            {
                InternalCalls.CameraSetZoom(1f);
                current_image++;
            }

            if (current_image == 1)
            {
                CameraScript.CameraTargetZoom(1.5f, 0.2f);
            }            
            
            if (current_image == 2)
            {
                CameraScript.CameraTargetZoom(1.2f, 0.5f);
            }
            if (current_image == 3)
            {
                CameraScript.CameraShake(0.5f);
                CameraScript.camera_shake_duration = 0.2f;
                CameraScript.camera_shake_intensity = 0.2f;
            }
            if (current_image == 4)
            {
                CameraScript.CameraTargetZoom(1.2f, 0.5f);
                CameraScript.CameraShake(0.5f);
                CameraScript.camera_shake_intensity = 2f;
                CameraScript.camera_shake_duration = 0.2f;
            }
            if (current_image == 13)
            {
                image_swap_set = 2.0f;
            }

            float offset_text = 0.5005f;
            float text_height = 0.155f;

            if (showText)
            {
                textbox_width = InternalCalls.GetWindowWidth() * textbox_width_ratio / CameraScript.camera_zoom;
                textbox_height = InternalCalls.GetWindowHeight() * textbox_height_ratio / CameraScript.camera_zoom / 2f;
                image_scale = new SimpleVector2D(textbox_width, textbox_height);

                position_of_textbox = new SimpleVector2D(CameraScript.camera_pos.x,
                                                                       CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 2.8f);

                SimpleImage image = InternalCalls.GetSpriteImage("Text box.png");
                InternalCalls.DrawImageAt(position_of_textbox, 0, image_scale, image, 1, InternalCalls.GetTopLayer());
            }

            if (InternalCalls.KeyPressed((int)KeyCodes.Escape)){
                current_image = 16;
                showText = false;
            }


            switch (current_image)
            {
                case 1:
                    InternalCalls.SetSpriteImage(image1);
                    showText = true;
                    InternalCalls.RenderTextFont("My name is Cipher. And I live a normal life here with my family.", "Semplicita_Light", offset_text, text_height, 12f, (0f, 0f, 0f, 1f));
                    InternalCalls.RenderTextFont("My name is Cipher. And I live a normal life here with my family.", "Semplicita_Light", 0.5f, text_height, 12f, (1f, 1f, 1f, 1f));
                    break;
                case 2:
                    InternalCalls.SetSpriteImage(image2);
                    showText = true;
                    InternalCalls.RenderTextFont("But everything changed one day when the creators unleashed their wrath...", "Semplicita_Light", offset_text, text_height, 12f, (0f, 0f, 0f, 1f));
                    InternalCalls.RenderTextFont("But everything changed one day when the creators unleashed their wrath...", "Semplicita_Light", 0.5f, text_height, 12f, (1f, 1f, 1f, 1f));
                    break;
                case 3:
                    InternalCalls.SetSpriteImage(image3);
                    showText = false;
                    if (!play_yk_voicelines)
                    {
                        InternalCalls.AudioPlayMusic("VO-MadCoder-001.wav", 0.2f);
                        InternalCalls.AudioPlayMusic("SCI-FI-AMBIENCE_GEN-HDF-20503.wav", 0.1f);
                        play_yk_voicelines = true;
                    }
                    break;
                case 4:
                    InternalCalls.SetSpriteImage(image4);
                    if (!play_first_explosion)
                    {
                        InternalCalls.FadeOutAudio(1f);
                        InternalCalls.AudioPlayMusic("SCI-FI-EXPLOSION_GEN-HDF-20662.wav", 0.2f);
                        play_first_explosion = true;
                    }
                    break;
                case 5:
                    InternalCalls.SetSpriteImage(image5);
                    if (!play_second_explosion)
                    {
                        InternalCalls.AudioPlayMusic("EXPLOSION-LARGE_GEN-HDF-10849.wav", 0.2f);
                        play_second_explosion = true;
                        InternalCalls.AudioPlayMusic("Fragments_Music_Tension 2.wav", 0.3f);
                        InternalCalls.AudioPlayMusic("JOHN_The Choice_Loop_60bpm.wav", 0.3f);
                    }
                    break;
                case 6:
                    InternalCalls.SetSpriteImage(image6);
                    if (!play_third_explosion)
                    {
                        InternalCalls.AudioPlayMusic("SCI-FI-GROUND-BREAK_GEN-HDF-20675.wav", 0.2f);
                        play_third_explosion = true;
                    }


                    CameraScript.CameraTargetZoom(2f, 0.2f);
                    InternalCalls.DrawSquare(0, 0, 10000, 10000, 1, 1, 1, 1 - (image_swap_timer / image_swap_set), InternalCalls.GetTopLayer());
                    break;
                case 7:
                    InternalCalls.SetSpriteImage(image7);
                    CameraScript.CameraTargetZoom(1f, 1.5f);
                    InternalCalls.DrawSquare(0, 0, 10000, 10000, 0, 0, 0, 1 - (image_swap_timer / image_swap_set), InternalCalls.GetTopLayer());
                    break;
                case 8:
                    InternalCalls.SetSpriteImage(image8);
                    showText = true;
                    InternalCalls.RenderTextFont("Urgh...What was that... Mom...? Dad?", "Semplicita_Light", offset_text, text_height, 12f, (0f, 0f, 0f, 1f));
                    InternalCalls.RenderTextFont("Urgh...What was that... Mom...? Dad?", "Semplicita_Light", 0.5f, text_height, 12f, (1f, 1f, 1f, 1f));
                    break;
                case 9:
                    InternalCalls.SetSpriteImage(image9);
                    showText = true;
                    InternalCalls.RenderTextFont("MOM! DAD! ARE YOU GUYS ALRIGHT?", "Semplicita_Light", offset_text, text_height, 12f, (0f, 0f, 0f, 1f));
                    InternalCalls.RenderTextFont("MOM! DAD! ARE YOU GUYS ALRIGHT?", "Semplicita_Light", 0.5f, text_height, 12f, (1f, 1f, 1f, 1f));
                    break;
                case 10:
                    InternalCalls.SetSpriteImage(image10);
                    showText = true;
                    InternalCalls.RenderTextFont("Stay back Cipher! Something's wrong...", "Semplicita_Light", offset_text, text_height, 12f, (0f, 0f, 0f, 1f));
                    InternalCalls.RenderTextFont("Stay back Cipher! Something's wrong...", "Semplicita_Light", 0.5f, text_height, 12f, (1f, 1f, 1f, 1f));
                    break;
                case 11:
                    InternalCalls.SetSpriteImage(image11);
                    showText = false;
                    break;
                case 12:
                    InternalCalls.SetSpriteImage(image11_2);
                    showText = true;
                    InternalCalls.RenderTextFont("MOM? DAD?? NO NO NO PLEASE!", "Semplicita_Light", offset_text, text_height, 12f, (0f, 0f, 0f, 1f));
                    InternalCalls.RenderTextFont("MOM? DAD?? NO NO NO PLEASE!", "Semplicita_Light", 0.5f, text_height, 12f, (1f, 1f, 1f, 1f));
                    break;
                case 13:
                    InternalCalls.SetSpriteImage(image12);
                    showText = true;
                    CameraScript.CameraShake(0.2f);
                    CameraScript.camera_shake_duration = 0.1f;
                    CameraScript.camera_shake_intensity = 0.2f;
                    InternalCalls.RenderTextFont("Huh?", "Semplicita_Light", 0.5f, text_height, 12f, (1f, 1f, 1f, 1f));
                    break;
                case 14:
                    InternalCalls.SetSpriteImage(image13);
                    showText = false;
                    if (!play_audio_collapse)
                    {
                        InternalCalls.AudioPlayMusic("ROCK-COLLAPSE_GEN-HDF-20004.wav", 0.1f);

                    }
                    play_audio_collapse = true;
                    break;
                case 15:
                    InternalCalls.SetSpriteImage(image14);
                    CameraScript.CameraTargetZoom(0.8f, 0.3f);
                    InternalCalls.DrawSquare(0, 0, 10000, 10000, 0, 0, 0, 1 - (image_swap_timer / image_swap_set), InternalCalls.GetTopLayer());
                    InternalCalls.FadeOutAudio(2.1f);
                    break;
                case 16:
                    InternalCalls.SetSpriteImage(image15);
                    break;
            }







        }
        
        static public void CleanUp()
        {
            InternalCalls.ShowCursor();
        }

    }
}
