using System.Runtime.CompilerServices;
namespace IS
{
    class StartButtonScript
    {

        static public bool first_hovering = false;
        static public bool start_button_click = false;
        static public float timer = 0f;
        static SimpleVector2D dark_circle = new SimpleVector2D(10000f, 10000f);
        static SimpleImage scene_circle;

        //static int button_entity;

        static public void Init() {
            scene_circle = InternalCalls.GetSpriteImage("dark_circle.png");
            //button_entity = InternalCalls.GetCurrentEntityID();
        }

        static public void Update(){
            // default scale size
            //InternalCalls.SetButtonSizeScale(button_entity, InternalCalls.GetButtonIdleScale(button_entity));
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                
                //hovering
                if (!first_hovering) {
                    //InternalCalls.AudioPlaySound("Hover.wav");
                    first_hovering = true;
                }
                //InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_1.wav");
                //InternalCalls.SetButtonSizeScale(button_entity, InternalCalls.GetButtonHoverScale(button_entity));
            }
            else
            {
                first_hovering = false;
            }
            if (InternalCalls.GetButtonState() == 2) 
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                start_button_click = true;
                //InternalCalls.SetButtonSizeScale(button_entity, InternalCalls.GetButtonIdleScale(button_entity));
            }

            if (start_button_click)
            {
                //SimpleImage cirlce = scene_circle;
                SimpleVector2D circle_scale = new SimpleVector2D(dark_circle.x * timer, dark_circle.y * timer);
                //InternalCalls.DrawImageAt(new SimpleVector2D(0,0), 0f, circle_scale, scene_circle, 1f, 4);
                timer -= InternalCalls.GetDeltaTime();
                if (timer <= 0f)
                {
                    InternalCalls.LoadScene("Assets/Scenes/GameLevel.insight");
                }
            }

            // draw text
            //InternalCalls.ButtonRenderText(button_entity, 0.4f, 0.54f, 35f, (1f, 1f, 1f));

        }
        
        static public void CleanUp(){

        }

    }
}
