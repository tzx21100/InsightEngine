using System.Runtime.CompilerServices;
namespace IS
{
    class TextBox
    {

        static public bool isVisible = false;
        static public float textSpeed = 1f;
        static public float fontSize = 24f;
        static public string font = "";
        static public string text = "";


        //setting text window size
        static public float textbox_width_ratio = 0.8f;
        static public float textbox_height_ratio = 0.4f;
        static private float textbox_width = InternalCalls.GetWindowWidth();
        static private float textbox_height = InternalCalls.GetWindowHeight();

        static private SimpleVector2D position_of_textbox = new SimpleVector2D();
        static private SimpleVector2D image_scale = new SimpleVector2D(1f,1f);

        //getting the place to start writing
        static public float left_margin = 0f;
        static public float top_margin = 0f;
        static public float line_padding = 2f; //space inbetween lines


        // calculations
        static private string text_to_draw="";
        static private string current_text_shown = "";
        static private int current_text_id;

        static public void Init(){
            textbox_width = InternalCalls.GetWindowWidth() * textbox_width_ratio;
            textbox_height= InternalCalls.GetWindowHeight() * textbox_height_ratio;
            position_of_textbox= new SimpleVector2D(CameraScript.camera_pos.x,
                                                                   CameraScript.camera_pos.y+textbox_height/2f);

            text_to_draw =text;
            //add \n to end of lines
            int length = text.Length;
            float max_length_perline = (textbox_width - left_margin * 2) / fontSize;


            current_text_shown = "";
            current_text_id = 0;
            

        }

        static public void Update(){


            //draw textbox 
            SimpleImage image= InternalCalls.GetSpriteImage("textbox.png");
            InternalCalls.DrawImageAt(position_of_textbox, 0, image_scale, image , 1, InternalCalls.GetTopLayer());

            //draw the words

            

        }
        
        static public void CleanUp(){

        }

    }
}
