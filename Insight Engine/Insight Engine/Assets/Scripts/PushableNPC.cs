using System.Runtime.CompilerServices;
namespace IS
{
    class PushableNPC
    {
        static public void Init(){

        }

        static public void Update()
        {

            if (InternalCalls.GetTransformPosition().x < 740)
            {
                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());

                TextBox.CreateTextBox("Looks like you made the same choice after all");
            }
                   


        }

        static public void CleanUp(){

        }

    }
}
