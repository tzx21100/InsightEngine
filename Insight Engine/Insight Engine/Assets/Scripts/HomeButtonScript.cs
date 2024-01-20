/*!
 * \file HomeButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the HomeButtonScript class, used for home button
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System.Runtime.CompilerServices;
namespace IS
{
    class HomeButtonScript
    {
        static public void Init()
        {
        }

        static public void Update()
        {
            if (PauseButtonScript.paused)
            {
                //hovered
                if (InternalCalls.GetButtonState() == 1)
                {
                    //hovering
                }
                if (InternalCalls.GetButtonState() == 2)
                {
                    //click
                }
            }
        }
        

        static public void CleanUp()
        {

        }
    }
}
