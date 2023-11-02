using System.Runtime.CompilerServices;
namespace IS
{
    class AudioScript
    {
        static MyRandom random = new MyRandom(1234);
        static public void Init(){

        }

        static public void Update(){
            
            if (InternalCalls.KeyPressed((int)KeyCodes.Z)) {

                

                // Generate a random integer between 0 and 100
                uint randomNumber = random.Next(0, 101);

                if (randomNumber < 25)
                {
                    InternalCalls.AudioPlaySound("Assets/Sounds/sound.MP3");
                    return;
                }
                if(randomNumber < 50)
                {
                    InternalCalls.AudioPlaySound("Assets/Sounds/Footsteps-Grass-Far-Small_1.wav");
                    return;
                }
                if (randomNumber < 75)
                {
                    InternalCalls.AudioPlaySound("Assets/Sounds/Footsteps-Grass-Far-Small_2.wav");
                    return;
                }
                if (randomNumber < 100)
                {
                    InternalCalls.AudioPlaySound("Assets/Sounds/Footsteps-Grass-Far-Small_3.wav");
                    return;
                }

            }
            if (InternalCalls.KeyPressed((int)KeyCodes.X)) { InternalCalls.AudioPlayMusic("Assets/Sounds/JOHN_Company History_Loop_120bpm.wav"); }
        }
        
        static public void CleanUp(){

        }

    }
}
