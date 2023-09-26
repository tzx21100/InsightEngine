/* Start Header **************************************************************/
/*!
\file	ScriptGlue.h
\author Matthew

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/

#ifndef GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTGLUE_H
#define GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTGLUE_H

namespace IS {
    //this class is solely to register cpp functions so that c# knows of it existence, for now
    class ScriptGlue {
    public:
        static void RegisterFunctions();
    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_SCRIPTGLUE_H  