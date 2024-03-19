using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace IS
{
    class TextBox
    {

        static public bool isVisible = false;
        static public float textSpeed = 0.05f; // Time in seconds per character
        static public float fontSize = 14f;
        static public string font = "Semplicita_Light";
        static public string text = "";

        static public float textbox_width_ratio = 0.7f;
        static public float textbox_height_ratio = 0.26f;
        static private float textbox_width = InternalCalls.GetWindowWidth();
        static private float textbox_height = InternalCalls.GetWindowHeight();

        static private SimpleVector2D position_of_textbox = new SimpleVector2D();
        static private SimpleVector2D image_scale = new SimpleVector2D(1f, 1f);

        static public float left_margin = 0f;
        static public float top_margin = 0f;
        static public float line_padding = 2f;

        static private string text_to_draw = "";
        static private string current_text_shown = "";
        static private int current_text_id = 0;
        static private float text_timer = 0f; // New timer variable
        static private float text_height = 0.141f;

        // keep track of which page of text the textbox is on
        static public int PAGE_NUMBER = 0;


        static public Queue<string> mTexts = new Queue<string>();


        static public void Init()
        {
            PAGE_NUMBER = 0;
            //isVisible = true;
            text = "This is a test to see if the newline thing works or not :) as u can tell its not very accurate";

            textbox_width = InternalCalls.GetWindowWidth() * textbox_width_ratio / CameraScript.camera_zoom;
            textbox_height = InternalCalls.GetWindowHeight() * textbox_height_ratio / CameraScript.camera_zoom / 2f;
            image_scale = new SimpleVector2D(textbox_width, textbox_height);

            position_of_textbox = new SimpleVector2D(CameraScript.camera_pos.x,
            CameraScript.camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 2.8f);

            


            // Initialize text processing
            text_to_draw = text; // Assuming 'text' contains the entire text to be displayed
            current_text_shown = "";
            current_text_id = 0;
            text_timer = 0f;

            // Process text to add line breaks
            ProcessTextForLineBreaks();
        }

        static public void Update()
        {
            if (!isVisible) return;

            // Draw textbox
            Vector2D camera_pos =new Vector2D(0,0)  ;
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;


            position_of_textbox = new SimpleVector2D(camera_pos.x,
            camera_pos.y - InternalCalls.GetWindowHeight() / CameraScript.camera_zoom / 2.8f);
            textbox_width = InternalCalls.GetWindowWidth() * textbox_width_ratio / CameraScript.camera_zoom;
            textbox_height = InternalCalls.GetWindowHeight() * textbox_height_ratio / CameraScript.camera_zoom;
            image_scale = new SimpleVector2D(textbox_width, textbox_height);
            SimpleImage image = InternalCalls.GetSpriteImage("textbox.png");
            InternalCalls.DrawImageAt(position_of_textbox, 0, image_scale, image, 1, InternalCalls.GetTopLayer());
            // End of draw textbox

           // text_height =  ( (position_of_textbox.y) / (position_of_textbox.y+ InternalCalls.GetWindowHeight()));
            //Console.WriteLine(text_height);

            // Update and draw the text letter by letter
            if (current_text_id < text_to_draw.Length)
            {
                text_timer += InternalCalls.GetDeltaTime();
                if (text_timer >= textSpeed)
                {
                    current_text_shown += text_to_draw[current_text_id];
                    current_text_id++;
                    text_timer = 0f; // Reset timer after adding a character
                }

                if (InternalCalls.KeyPressed((int)KeyCodes.Enter) || InternalCalls.MousePressed(0) || InternalCalls.ControllerKeyPressed((int)KeyCodes.Button_B) )
                {

                    current_text_shown = text_to_draw;
                    current_text_id = text_to_draw.Length;
                }

            }
            else
            {
                if (InternalCalls.KeyPressed((int)KeyCodes.Enter) || InternalCalls.MousePressed(0) || InternalCalls.ControllerKeyPressed((int)KeyCodes.Button_B) )
                {
                    if (mTexts.Count == 0)
                    {
                        isVisible = false;
                    }

                    if (current_text_id == text_to_draw.Length)
                    {
                        if (mTexts.Count > 0)
                        {
                            text = mTexts.Dequeue();
                            PAGE_NUMBER++;
                            ProcessTextForLineBreaks();
                        }
                    }

                }
            }

            // Assuming there's a method to draw text. Adjust parameters as needed.
            InternalCalls.RenderLeftAlignTextFont(current_text_shown,font,0.24f,text_height, fontSize, (1f,1f,1f,1f) );
        }

        static public void CleanUp()
        {
            // Clean-up code if necessary
        }

        static private void ProcessTextForLineBreaks()
        {

            // Initialize text processing
            text_to_draw = text; // Assuming 'text' contains the entire text to be displayed
            current_text_shown = "";
            current_text_id = 0;
            text_timer = 0f;


            string processedText = "";
            int lineLength = 0;
            int maxLineLength = CalculateMaxLineLength(); // Implement this method based on your textbox size and font size

            foreach (char c in text_to_draw)
            {
                // Add the character to the processed text
                processedText += c;
                lineLength++;

                // Check if a space or newline character, consider for line break logic
                if (c == ' ' || c == '\n')
                {
                    if (lineLength >= maxLineLength)
                    {
                        processedText += '\n'; // Add a line break
                        lineLength = 0; // Reset line length counter
                    }
                }
            }

            text_to_draw = processedText;
        }


        static private int CalculateMaxLineLength()
        {

            return (int)(InternalCalls.GetWindowWidth() / ((fontSize+2)*2.15)) ; // Will change later when proper text draw comes out
        }

        static public void CreateTextBox(string text2)
        {
            isVisible = true;
            text = text2;
            ProcessTextForLineBreaks();
            mTexts.Clear();
            PAGE_NUMBER = 0;
        }
        static public void AddTextLines(string text2)
        {
            isVisible = true;
            mTexts.Enqueue(text2);
        }

    }
}
