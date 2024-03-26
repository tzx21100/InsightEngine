/*!
 * \file SpikeScript.cs
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This source file is meant for the player movement and camera.
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
    class SpikeScript
    {
        static public void Init()
        {

        }

        static public void Update()
        {

            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if(array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                PlayerScript.isDead = true;
                PlayerScript.screen_flash_timer += 1.5f;
            }

            foreach (var key in Enemy.enemies.Keys)
            {
                if (array.FindIndex(key) != -1)
                {
                    Enemy.enemies[key].health= 0;
                }
            }

            foreach (var key in HandEnemy.enemies.Keys)
            {
                if (array.FindIndex(key) != -1)
                {
                    HandEnemy.enemies[key].health = 0;
                }
            }


        }

        static public void CleanUp()
        {

        }

    }
}
