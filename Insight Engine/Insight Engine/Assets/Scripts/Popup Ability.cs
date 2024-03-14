using System.Runtime.CompilerServices;
namespace IS
{
    // this is a class for other classes to call no need to be added to script engine.
    class Popup_Ability
    {
        static public string doublejumpVideo = "Assets/Videos/Double_Jump_Tutorial.mp4";
        static public string vineVideo = "Assets/Videos/Vine_Climb_Tutorial.mp4";
        static public string dashVideo = "Assets/Videos/Dash_Tutorial.mp4";
        static public bool doublejumpvideoloaded=false;
        static public bool vinevideoloaded=false;
        static public bool dashvideoloaded=false;
        static public string font = "Semplicita_Light";
        static public float BG_ALPHA = 0.6f;
        static private float VIDEO_Y = -0.1f;

        static public void Init(){

        }

        static public void Update(){
            
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
            }

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 10000, 10000, 0, 0, 0, BG_ALPHA, InternalCalls.GetTopLayer()+2);
            string Header_text = "DOUBLE JUMP UNLOCKED";
            InternalCalls.RenderTextFont(Header_text,font, 0.5f, 0.8f, 24f, (1, 1, 1, 1));
            string DisplayText = "You can now double jump!\n" +
                                "Use this ability wisely to cross distances.\n" +
                                "Left Click To Close Tutorial...";
            InternalCalls.RenderLeftAlignTextFont(DisplayText, font, 0.1f, 0.5f, 16f, (1, 1, 1, 1));

            if (InternalCalls.MousePressed(0))
            {
                PlayerScript.hasDoubleJump = false; //boolean for showing double jump
                doublejumpvideoloaded = false;
                InternalCalls.unloadVideos();
                InternalCalls.SetLightsToggle(true);
            }

            
        }


        static public void DisplayWallGuide()
        {
            InternalCalls.SetLightsToggle(false);
            if (vinevideoloaded == false)
            {
                InternalCalls.loadVideo(vineVideo, 0.4f, 0.4f, 0.5f, VIDEO_Y, true);
                vinevideoloaded = true;
            }

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 10000, 10000, 0, 0, 0, BG_ALPHA, InternalCalls.GetTopLayer() + 2);
            string Header_text = "VINE MECHANICS UNLOCKED";
            InternalCalls.RenderTextFont(Header_text, font, 0.5f, 0.8f, 24f, (1, 1, 1, 1));
            string DisplayText = "You can now Climb Vines!\n" +
                                "Use this ability to go up sloped walls \nand reset abilities.\n" +
                                "If it's too steep, you might fall!\n" +
                                "Left Click To Close Tutorial...";
            InternalCalls.RenderLeftAlignTextFont(DisplayText, font, 0.05f, 0.55f, 16f, (1, 1, 1, 1));

            if (InternalCalls.MousePressed(0))
            {
                PlayerScript.hasWallClimb = false; //boolean for showing double jump
                vinevideoloaded = false;
                InternalCalls.unloadVideos();
                InternalCalls.SetLightsToggle(true);
            }


        }

        static public void DisplayDashGuide()
        {
            InternalCalls.SetLightsToggle(false);
            if (dashvideoloaded == false)
            {
                InternalCalls.loadVideo(dashVideo, 0.4f, 0.4f, 0.5f, VIDEO_Y, true);
                dashvideoloaded = true;
            }

            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 10000, 10000, 0, 0, 0, BG_ALPHA, InternalCalls.GetTopLayer() + 2);
            string Header_text = "DASH UNLOCKED";
            InternalCalls.RenderTextFont(Header_text, font, 0.5f, 0.8f, 24f, (1, 1, 1, 1));
            string DisplayText = "Press 'Left Shift' to Dash!\n" +
                                 "Use this ability to move around and break walls!\n" +
                                 "Climbing on vines will reset this ability.\n" +
                                 "Left Click To Close Tutorial...";
            InternalCalls.RenderLeftAlignTextFont(DisplayText, font, 0.05f, 0.5f, 16f, (1, 1, 1, 1));

            if (InternalCalls.MousePressed(0))
            {
                PlayerScript.hasDash = false; //boolean for showing double jump
                dashvideoloaded = false;
                InternalCalls.unloadVideos();
                InternalCalls.SetLightsToggle(true);
            }


        }

    }
}
