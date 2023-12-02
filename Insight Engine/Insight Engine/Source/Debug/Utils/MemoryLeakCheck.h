/*!
 * \file MemoryLeakCheck.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file defines new as DEBUG_NEW which tracks the normal block,
 * file and line of where it was used, as well as setting debug flags.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_DEBUG_MEMORY_LEAK_CHECK_H
#define GAM200_INSIGHT_ENGINE_DEBUG_MEMORY_LEAK_CHECK_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define new DEBUG_NEW
    #define ENABLE_MEMORY_CHECK() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#else
    #define ENABLE_MEMORY_CHECK __noop
#endif


#endif // !GAM200_INSIGHT_ENGINE_DEBUG_MEMORY_LEAK_CHECK_H
