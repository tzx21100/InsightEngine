using System.Runtime.CompilerServices;
namespace IS
{
    // this is a class for other classes to call no need to be added to script engine.
    class Popup_Ability
    {
        static public bool popup_shown = false;

        static public string doublejumpVideo = "Assets/Videos/Double_Jump_Tutorial.mp4";
        static public string vineVideo = "Assets/Videos/Vine_Climb_Tutorial.mp4";
        static public string dashVideo = "Assets/Videos/Dash_Tutorial.mp4";
        static public string flyVideo = "Assets/Videos/Flight_Tutorial.mp4";
        static public bool doublejumpvideoloaded=false;
        static public bool vinevideoloaded=false;
        static public bool dashvideoloaded=false;
        static public bool flyvideoloaded=false;
        static public string font = "Semplicita_Light";
        static public float BG_ALPHA = 0.6f;
        static private float VIDEO_Y = -0.1f;
        static private int winHeight = 0;
        static private int winWidth = 0;

        static public void Init(){

        }

        static public void Update(){
            winWidth = InternalCalls.GetWindowWidth();
            winHeight = InternalCalls.GetWindowHeight();
        }

        static public void CleanUp(){

        }

        static public void DisplayDoubleJumpGuide()
        {
            InternalCalls.SetLightsToggle(false); 
            if (doublejumpvideoloaded == false)
            {
                InternalCalls.loadVideo(doublejumpVideo, 0.4f, 0.4f, 0.5f, VIDEO_Y, true);
                doublejumpvideoloaded = true;
                popup_shown = true;
            }

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 10000, 10000, 0, 0, 0, BG_ALPHA, InternalCalls.GetTopLayer()+2);

            float textbox_width = InternalCalls.GetWindowWidth() * 0.55f / CameraScript.camera_zoom;
            float textbox_height = InternalCalls.GetWindowHeight() * 0.5f / CameraScript.camera_zoom;
            SimpleVector2D image_scale = new SimpleVector2D(textbox_width, textbox_height);

            SimpleVector2D position_of_textbox = new SimpleVector2D(CameraScript.camera_pos.x - InternalCalls.GetWindowWidth() / CameraScript.camera_zoom / 4.3f,
                                                                   CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 30.0f);

            SimpleImage image = InternalCalls.GetSpriteImage("textbox.png");
            InternalCalls.DrawImageAt(position_of_textbox, 0, image_scale, image, 1, InternalCalls.GetTopLayer()+4);


            string Header_text = "DOUBLE JUMP UNLOCKED";
            InternalCalls.RenderTextFont(Header_text,font, 0.5f, 0.8f, 24f, (1, 1, 1, 1));
            string DisplayText = "You can now double jump!\n" +
                                "Use this ability wisely to cross distances.\n" +
                                "Left Click To Close Tutorial...";


            if (InternalCalls.ControllerConnected())
            {
                DisplayText = "You can now double jump!\n" +
                                "Use this ability wisely to cross distances.\n" +
                                "Press 'B' To Close Tutorial...";
            }


            InternalCalls.RenderLeftAlignTextFont(DisplayText, font, 0.09f, 0.5f, 16f, (1, 1, 1, 1));

            if (PlayerScript.select_trigger)
            {
                PlayerScript.hasDoubleJump = false; //boolean for showing double jump
                doublejumpvideoloaded = false;
                InternalCalls.unloadVideos();
                InternalCalls.SetLightsToggle(true);
                popup_shown = false;
            }

            
        }


        static public void DisplayWallGuide()
        {
            InternalCalls.SetLightsToggle(false);
            if (vinevideoloaded == false)
            {
                InternalCalls.loadVideo(vineVideo, 0.4f, 0.4f, 0.5f, VIDEO_Y, true);
                vinevideoloaded = true;
                popup_shown = true;
            }

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 10000, 10000, 0, 0, 0, BG_ALPHA, InternalCalls.GetTopLayer() + 2);
            string Header_text = "VINE MECHANICS UNLOCKED";

            float textbox_width = InternalCalls.GetWindowWidth() * 0.55f / CameraScript.camera_zoom;
            float textbox_height = InternalCalls.GetWindowHeight() * 0.5f / CameraScript.camera_zoom;
            SimpleVector2D image_scale = new SimpleVector2D(textbox_width, textbox_height);

            SimpleVector2D position_of_textbox = new SimpleVector2D(CameraScript.camera_pos.x - InternalCalls.GetWindowWidth() / CameraScript.camera_zoom / 4.3f,
                                                                    CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 30.0f);

            SimpleImage image = InternalCalls.GetSpriteImage("textbox.png");
            InternalCalls.DrawImageAt(position_of_textbox, 0, image_scale, image, 1, InternalCalls.GetTopLayer() + 4);

            InternalCalls.RenderTextFont(Header_text, font, 0.5f, 0.8f, 24f, (1, 1, 1, 1));
            string DisplayText = "You can now Climb Vines!\n" +
                                "Use this ability to go up sloped walls \nand reset abilities.\n" +
                                "If it's too steep, you might fall!\n" +
                                "Left Click To Close Tutorial...";
            if (InternalCalls.ControllerConnected())
            {
                DisplayText = "You can now Climb Vines!\n" +
                                "Use this ability to go up sloped walls \nand reset abilities.\n" +
                                "If it's too steep, you might fall!\n" +
                                "Press 'B' To Close Tutorial...";
            }

            InternalCalls.RenderLeftAlignTextFont(DisplayText, font, 0.09f, 0.545f, 16f, (1, 1, 1, 1));

            if (PlayerScript.select_trigger)
            {
                PlayerScript.hasWallClimb = false; //boolean for showing double jump
                vinevideoloaded = false;
                InternalCalls.unloadVideos();
                InternalCalls.SetLightsToggle(true);
                popup_shown = false;
            }


        }

        static public void DisplayDashGuide()
        {
            InternalCalls.SetLightsToggle(false);
            if (dashvideoloaded == false)
            {
                InternalCalls.loadVideo(dashVideo, 0.4f, 0.4f, 0.5f, VIDEO_Y, true);
                dashvideoloaded = true;
                popup_shown = true;
            }

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 10000, 10000, 0, 0, 0, BG_ALPHA, InternalCalls.GetTopLayer() + 2);
            string Header_text = "DASH UNLOCKED";

            float textbox_width = InternalCalls.GetWindowWidth() * 0.55f / CameraScript.camera_zoom;
            float textbox_height = InternalCalls.GetWindowHeight() * 0.5f / CameraScript.camera_zoom;
            SimpleVector2D image_scale = new SimpleVector2D(textbox_width, textbox_height);

            SimpleVector2D position_of_textbox = new SimpleVector2D(CameraScript.camera_pos.x - InternalCalls.GetWindowWidth() / CameraScript.camera_zoom / 4.3f,
                                                                   CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 30.0f);

            SimpleImage image = InternalCalls.GetSpriteImage("textbox.png");
            InternalCalls.DrawImageAt(position_of_textbox, 0, image_scale, image, 1, InternalCalls.GetTopLayer()+4);

            InternalCalls.RenderTextFont(Header_text, font, 0.5f, 0.8f, 24f, (1, 1, 1, 1));
            string DisplayText = "Press 'Left Shift' to Dash!\n" +
                                 "Use this ability to move around,\nbreak walls and dodge attacks!\n" +
                                 "Climbing on vines will reset this ability.\n" +
                                 "Left Click To Close Tutorial...";


            if (InternalCalls.ControllerConnected())
            {
                DisplayText = "Press 'Y' to Dash!\n" +
                                 "Use this ability to move around,\nbreak walls and dodge attacks!\n" +
                                 "Climbing on vines will reset this ability.\n" +
                                 "Press 'B' To Close Tutorial...";
            }

            InternalCalls.RenderLeftAlignTextFont(DisplayText, font, 0.09f, 0.545f, 16f, (1, 1, 1, 1));

            if (PlayerScript.select_trigger)
            {
                PlayerScript.hasDash = false; //boolean for showing double jump
                dashvideoloaded = false;
                InternalCalls.unloadVideos();
                InternalCalls.SetLightsToggle(true);
                popup_shown = false;
            }


        }


        static public void DisplayFlyGuide()
        {
            InternalCalls.SetLightsToggle(false);
            if (flyvideoloaded == false)
            {
                InternalCalls.loadVideo(flyVideo, 0.4f, 0.4f, 0.5f, VIDEO_Y, true);
                flyvideoloaded = true;
                popup_shown = true;
            }

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 10000, 10000, 0, 0, 0, BG_ALPHA, InternalCalls.GetTopLayer() + 2);
            string Header_text = "FLIGHT UNLOCKED";

            float textbox_width = InternalCalls.GetWindowWidth() * 0.55f / CameraScript.camera_zoom;
            float textbox_height = InternalCalls.GetWindowHeight() * 0.5f / CameraScript.camera_zoom;
            SimpleVector2D image_scale = new SimpleVector2D(textbox_width, textbox_height);

            SimpleVector2D position_of_textbox = new SimpleVector2D(CameraScript.camera_pos.x - InternalCalls.GetWindowWidth() / CameraScript.camera_zoom / 4.3f,
                                                                   CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 30.0f);

            SimpleImage image = InternalCalls.GetSpriteImage("textbox.png");
            InternalCalls.DrawImageAt(position_of_textbox, 0, image_scale, image, 1, InternalCalls.GetTopLayer() + 4);

            InternalCalls.RenderTextFont(Header_text, font, 0.5f, 0.8f, 24f, (1, 1, 1, 1));
            string DisplayText = "Press 'W' to Fly!\n" +
                                 "Use this ability to navigate winds!\n" +
                                 "Left Click To Close Tutorial...";


            if (InternalCalls.ControllerConnected())
            {
                DisplayText = "Move your joystick up to fly!\n" +
                                 "Use this ability to navigate winds!\n" +
                                 "Left Click To Close Tutorial...";
            }

            InternalCalls.RenderLeftAlignTextFont(DisplayText, font, 0.09f, 0.545f, 16f, (1, 1, 1, 1));

            if (PlayerScript.select_trigger)
            {
                PlayerScript.hasFly = false; //boolean for showing double jump
                flyvideoloaded = false;
                InternalCalls.unloadVideos();
                InternalCalls.SetLightsToggle(true);
                popup_shown = false;
                int entt=InternalCalls.CreateEntityPrefab("TransitionToPuzle");
                InternalCalls.TransformSetPositionEntity(PlayerScript.player_pos.x, PlayerScript.player_pos.y, entt);
            }


        }



    }
}
