/*!
 * \file TransitionToPuzzle.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2451
 * \date 15-03-2024
 * \brief
 * This header file contains the implementation for the transition to puzzle level.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

using System.Runtime.CompilerServices;
namespace IS
{
    class CheatCodeEntity
    {
        static public void Init()
        {
        }

        static public void Update()
        {
            // cheat key to boss level
            if (InternalCalls.KeyPressed((int)KeyCodes.K))
            {
                // load to selection scene
                InternalCalls.LoadScene("Assets/Scenes/FinalSelection.insight");
                return;
            }
        }
        
        static public void CleanUp()
        {

        }

    }
}
