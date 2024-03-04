using System.Runtime.CompilerServices;

namespace IS
{
    class TextBox
    {
        static public bool isVisible = false;
        static public float textSpeed = 0.05f; // Time in seconds per character
        static public float fontSize = 24f;
        static public string font = "Semplicita_Light";
        static public string text = "";

        static public float textbox_width_ratio = 0.8f;
        static public float textbox_height_ratio = 0.4f;
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
        static private float text_height = 0.255f;


        static public void Init()
        {

            isVisible = true;
            text = "This is a test to see if the newline thing works or not :) as u can tell its not very accurate";

            textbox_width = InternalCalls.GetWindowWidth() * textbox_width_ratio;
            textbox_height = InternalCalls.GetWindowHeight() * textbox_height_ratio;
            position_of_textbox = new SimpleVector2D(CameraScript.camera_pos.x,
                                                     CameraScript.camera_pos.y + textbox_height / 2f);

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
            SimpleImage image = InternalCalls.GetSpriteImage("textbox.png");
            InternalCalls.DrawImageAt(position_of_textbox, 0, image_scale, image, 1, InternalCalls.GetTopLayer());

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
            }

            // Assuming there's a method to draw text. Adjust parameters as needed.
            InternalCalls.RenderTextFont(current_text_shown,font,0.5f,text_height, fontSize, (1f,1f,1f,1f) );
        }

        static public void CleanUp()
        {
            // Clean-up code if necessary
        }

        static private void ProcessTextForLineBreaks()
        {
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

            return (int)(textbox_width / fontSize); // Will change later when proper text draw comes out
        }

    }
}
