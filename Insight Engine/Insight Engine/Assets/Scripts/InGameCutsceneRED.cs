using System.Runtime.CompilerServices;

namespace IS
{
    class InGameCutsceneRED
    {
        static string text0 = "Welcome Cipher. You have reached the last checkpoint.";
        static string text1 = "You have overcomed obstacles, defeated the mad being and yet your world remains ruined.";
        static string text2 = "There is after all... one final FRAGMENT. A red one that can save this world.";
        static string text3 = "It is not in any ruins or scattered somewhere else. You can find it here.";
        static string text4 = "Within you.";
        static string text5 = "You will have to sacrifice yourself... or choose not to, the choice is yours."; // pan here
        static string text6 = "Take care. Cipher.";

        static SimpleImage MCImage = InternalCalls.GetSpriteImage("MC.png");
        static SimpleImage NPCImage = InternalCalls.GetSpriteImage("NPC.png");

        static float switch_view_timer = 2.5f;
        static bool play_audio = true;
        static bool textshow = true;

        static public int FINAL_SPIKE = 999;

        static public void drawBigNPC()
        {
            SimpleVector2D position_of_NPC = new SimpleVector2D(CameraScript.camera_pos.x - InternalCalls.GetWindowWidth() / CameraScript.camera_zoom / 3.1f,
                                                                    CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 5.0f);
            float xScale = 1887f / InternalCalls.GetWindowWidth();
            float yScale = 2160.0f / InternalCalls.GetWindowHeight();
            float NPCImageWidth = InternalCalls.GetWindowWidth() * -xScale / CameraScript.camera_zoom / 2.5f;
            float NPCImageHeight = InternalCalls.GetWindowHeight() * yScale / CameraScript.camera_zoom / 2.5f;
            SimpleVector2D image_scale = new SimpleVector2D(NPCImageWidth, NPCImageHeight);
            InternalCalls.DrawImageAt(position_of_NPC, 0, image_scale, NPCImage, 1, InternalCalls.GetTopLayer() - 1);
        }

        static public void drawBigMC()
        {
            SimpleVector2D position_of_MC = new SimpleVector2D(CameraScript.camera_pos.x - InternalCalls.GetWindowWidth() / CameraScript.camera_zoom / 3.1f,
                                                                    CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 5.0f);

            float xScale = 1273f / InternalCalls.GetWindowWidth();
            float yScale = 2031f / InternalCalls.GetWindowHeight();
            float MCImageWidth = InternalCalls.GetWindowWidth() * xScale / CameraScript.camera_zoom / 2.5f;
            float MCImageHeight = InternalCalls.GetWindowHeight() * yScale / CameraScript.camera_zoom / 2.5f;
            SimpleVector2D image_scale = new SimpleVector2D(MCImageWidth, MCImageHeight);
            InternalCalls.DrawImageAt(position_of_MC, 0, image_scale, MCImage, 1, InternalCalls.GetTopLayer() - 1);
        }

        static public void Init()
        {
            textshow = true;
            play_audio = true;
            switch_view_timer = 2.5f;
            TextBox.CleanUp();
        }

        static public void Update()
        {

            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1 && textshow)
            {
                textshow = false;
                TextBox.CreateTextBox(text0);
                TextBox.AddTextLines(text1);
                TextBox.AddTextLines(text2);
                TextBox.AddTextLines(text3);
                TextBox.AddTextLines(text4);
                TextBox.AddTextLines(text5);
                TextBox.AddTextLines(text6);

                Vector2D pan_position = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);
                //CameraScript.CameraPanTo(pan_position, 1f);

            }


            if(array.FindIndex(FINAL_SPIKE)!=-1)
            {
                //gg

                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());



            }

            // draw npc
            if (!textshow && TextBox.isVisible)
            {
                drawBigNPC();
            }





            if (TextBox.PAGE_NUMBER == 6)
            {

                Vector2D pos = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);
                if(play_audio)
                {
                    int spike = InternalCalls.CreateEntityPrefab("FinalSpike");
                    InternalCalls.TransformSetPositionEntity(pos.x - 1500f, pos.y, spike);
                    FINAL_SPIKE = spike;
                    play_audio = false;
                }

                Vector2D pan_pos = new Vector2D(pos.x - 1500f, pos.y);
                CameraScript.CameraPanTo(pan_pos, 1f);
            }


            //end of camera pan and dialogue
            if (textshow == false && TextBox.isVisible == false)
            {
                CameraScript.StopCameraPan();
                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID() );
                int npc=InternalCalls.CreateEntityPrefab("PushableNPC");
                Vector2D pos = new Vector2D(InternalCalls.GetTransformPosition().x, InternalCalls.GetTransformPosition().y);
                InternalCalls.TransformSetPositionEntity(pos.x, pos.y, npc);

            }


        }

        static public void CleanUp()
        {

        }

    }
}

