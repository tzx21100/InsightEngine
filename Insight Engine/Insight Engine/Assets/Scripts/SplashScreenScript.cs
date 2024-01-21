/*!
 * \file SplashScreenScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This header file contains the SplashScreenScript class, used for the splash screen
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Numerics;
namespace IS
{
    class SplashScreenScript
    {
        static private float timer;
        static private float dt;

        static public void Init()
        {
            timer = 3f;
            dt = InternalCalls.GetFixedDeltaTime();
        }

        static public void Update()
        {
            timer -= dt;
            if (timer <= 0) 
            { 
                InternalCalls.LoadScene("Assets/Scenes/Main Menu.insight"); 
            }
        }


        static public void CleanUp()
        {

        }

    }
}