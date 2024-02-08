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

        static private float image_swap_timer = 2.6f;
        static private float image_swap_set = 2.6f;
        static private int current_image = 1;

        static private bool play_audio_collapse = false;


        static public void Init()
        {
            play_audio_collapse = false;
            InternalCalls.HideCursor();
            current_image = 0;
            image_swap_timer = 2.6f;
            InternalCalls.AudioPlayMusic("JOHN_The Choice_No Clock_Loop_60bpm.wav",0.4f);
            InternalCalls.AttachCamera(0, 0);
            //InternalCalls.AudioPlayMusic("Fragments_Music_Tension 2.wav", 0.4f);
        }

        static public void Update(){
            InternalCalls.TransformSetScale(InternalCalls.GetWindowWidth(),InternalCalls.GetWindowHeight());

            image_swap_timer -= InternalCalls.GetDeltaTime();
            if(image_swap_timer < 0)
            {
                image_swap_timer = image_swap_set;
                current_image++;
                if(current_image == 17) {
                    InternalCalls.LoadScene("Assets/Scenes/CaveLevel.insight");
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




            switch (current_image)
            {
                case 1:
                    InternalCalls.SetSpriteImage(image1);
                    InternalCalls.RenderTextFont("My name is Cipher. And I live a normal life here with my family.", "Semplicita_Light", 0.5f, 0.2f, 12f, (1f, 1f, 1f));
                    break;
                case 2:
                    InternalCalls.SetSpriteImage(image2);
                    InternalCalls.RenderTextFont("But everything changed one day when the creators unleashed their wrath...", "Semplicita_Light", 0.5f, 0.2f, 12f, (1f, 1f, 1f));
                    break;
                case 3:
                    InternalCalls.SetSpriteImage(image3);
                    break;
                case 4:
                    InternalCalls.SetSpriteImage(image4);
                    break;
                case 5:
                    InternalCalls.SetSpriteImage(image5);
                    break;
                case 6:
                    InternalCalls.SetSpriteImage(image6);
                    CameraScript.CameraTargetZoom(2f, 0.2f);
                    InternalCalls.DrawSquare(0, 0, 10000, 1000, 1, 1, 1, 1 - (image_swap_timer / image_swap_set), InternalCalls.GetTopLayer());
                    break;
                case 7:
                    InternalCalls.SetSpriteImage(image7);
                    CameraScript.CameraTargetZoom(1f, 1.5f);
                    InternalCalls.DrawSquare(0, 0, 10000, 1000, 0, 0, 0, 1 - (image_swap_timer / image_swap_set), InternalCalls.GetTopLayer());
                    break;
                case 8:
                    InternalCalls.SetSpriteImage(image8);
                    InternalCalls.RenderTextFont("Urgh...What was that... Mom...? Dad?", "Semplicita_Light", 0.5f, 0.2f, 12f, (1f, 1f, 1f));
                    break;
                case 9:
                    InternalCalls.SetSpriteImage(image9);
                    InternalCalls.RenderTextFont("MOM! DAD! ARE YOU GUYS ALRIGHT?", "Semplicita_Light", 0.5f, 0.2f, 12f, (1f, 1f, 1f));
                    break;
                case 10:
                    InternalCalls.SetSpriteImage(image10);
                    InternalCalls.RenderTextFont("Stayback Cipher! Something's wrong...", "Semplicita_Light", 0.5f, 0.2f, 12f, (1f, 1f, 1f));
                    break;
                case 11:
                    InternalCalls.SetSpriteImage(image11);
                    break;
                case 12:
                    InternalCalls.SetSpriteImage(image11_2);
                    InternalCalls.RenderTextFont("MOM? DAD?? NO NO NO PLEASE!", "Semplicita_Light", 0.5f, 0.2f, 12f, (1f, 1f, 1f));
                    break;
                case 13:
                    InternalCalls.SetSpriteImage(image12);
                    CameraScript.CameraShake(0.2f);
                    CameraScript.camera_shake_duration = 0.1f;
                    CameraScript.camera_shake_intensity = 0.2f;
                    InternalCalls.RenderTextFont("Huh?", "Semplicita_Light", 0.5f, 0.2f, 12f, (1f, 1f, 1f));
                    break;
                case 14:
                    InternalCalls.SetSpriteImage(image13);
                    if (!play_audio_collapse)
                    {
                        InternalCalls.AudioPlayMusic("ROCK-COLLAPSE_GEN-HDF-20004.wav", 0.1f);
                    }
                    play_audio_collapse = true;
                    break;
                case 15:
                    InternalCalls.SetSpriteImage(image14);
                    InternalCalls.DrawSquare(0, 0, 10000, 1000, 0, 0, 0, 1 - (image_swap_timer / image_swap_set), InternalCalls.GetTopLayer());
                    break;
                case 16:
                    InternalCalls.SetSpriteImage(image15);
                    InternalCalls.FadeOutAudio(1.2f);
                    break;
            }







        }
        
        static public void CleanUp()
        {
            InternalCalls.ShowCursor();
        }

    }
}
