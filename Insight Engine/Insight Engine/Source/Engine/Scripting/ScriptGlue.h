/* Start Header **************************************************************/
/*!
\file    ScriptGlue.h
\author  Matthew Ng, matthewdeen.ng@digipen.edu
\par     Course: CSD2401
\date    27-09-2023
\brief   This header file declares the implementation of the ScriptGlue class,
         which is responsible for registering C++ functions to be accessible from
         C# scripts.

\copyright
All content (C) 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*____________________________________________________________________________*/
/* End Header ****************************************************************/
 /* Include Guard */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTGLUE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTGLUE_H

namespace IS {

    // Global static variables for sharing
    static bool RecievedScriptCondition = false;

    // This class is solely to register C++ functions so that C# knows of their existence, for now.
    class ScriptGlue {
    public:
        static void RegisterFunctions();
    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTGLUE_H
