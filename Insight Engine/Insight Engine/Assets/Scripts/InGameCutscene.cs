using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class InGameCutscene
    {
        static string text = "I'm glad to see that there's someone still alive.";
        static string text2 = "I see that you have already been collecting these floating fragments around us.";
        static string text3 = "Please continue collecting them. I have a plan to fix this world!";
        static string text4 = "Wait... what's that?!";


        static bool textshow = true;
        static public void Init(){
            textshow = true;
        }

        static public void Update(){

            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1 && textshow) 
            {
                textshow = false;
                TextBox.CreateTextBox(text);
                TextBox.AddTextLines(text2);
                TextBox.AddTextLines(text3);
                TextBox.AddTextLines(text4);
            
            }

/*            if (InternalCalls.KeyPressed((int)KeyCodes.P))
            {
                TextBox.CreateTextBox("NIGGER NIGNEIG EAIGAONEAOIGNEGNEAOINGIN NIGGER");
            }*/

        }
        
        static public void CleanUp(){

        }

    }
}
