/*!
 * \file SceneManager.h
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
            Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-10-2023
 * \brief
 * This header file declares the interface for class SceneManager,
 * which encapsulates the functionalities of a scene manager, which manages
 * the scenes in a game.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_SCENE_SCENEMANAGER_H
#define GAM200_INSIGHT_ENGINE_EDITOR_SCENE_SCENEMANAGER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <functional>

namespace IS {

	using SceneID = uint8_t; ///< Type alias representing scene id

    /*!
     * \brief The SceneManager class manages the scenes in the game.
     */
	class SceneManager
	{
	public:
		/*!
		 * \brief Type alias for map used.
		 * \tparam KeyType The key type.
		 * \tparam Type The mapped type.
		 */
		template <typename KeyType, typename Type>
		using Map = std::unordered_map<KeyType, Type>;

		using ECSMap = Map<std::string, std::shared_ptr<IComponentArray>>; ///< Type alias for Map used for ECS.

		/*!
		 * \brief Type alias for Map containing scene id as key.
		 * \tparam Type The mapped type.
		 */
		template <typename Type>
		using SceneMap = Map<SceneID, Type>; ///< Type alias for Map containing scene id as key

		/*!
		 * \brief Get the singleton instance of the SceneManager.
		 * \return The singleton instance.
		 */
		static SceneManager& Instance();
        /*!
         * \brief Create a new scene with the given filename and an optional function.
         * \param scene_filename The name of the scene file.
         * \param SceneFunc An optional function to be executed for the scene.
         */
        void CreateScene(std::string const& scene_filename, std::function<void(void)> SceneFunc = nullptr);

        /*!
         * \brief Load a scene from a given filename.
         * \param scene_filename The name of the scene file to load.
         */
        void LoadScene(std::string const& scene_filename);

        /*!
         * \brief Save the current scene to the default filename.
         */
        void SaveScene();

        /*!
         * \brief Save the current scene to a specified filename.
         * \param scene_filename The name of the scene file to save.
         */
        void SaveSceneAs(std::string const& scene_filename);

        /*!
         * \brief Save the current scene to file.
         */
        void SaveSceneToFile();

        /*!
         * \brief Switch to the scene with the specified SceneID.
         * \param scene The SceneID of the scene to switch to.
         */
        void SwitchScene(SceneID scene);

        /*!
         * \brief Run a function for the specified scene.
         * \param SceneFunc The function to run.
         */
        void RunSceneFunction(std::function<void(SceneID)> SceneFunc);

        /*!
         * \brief Get the name of the scene with the given SceneID.
         * \param scene_id The SceneID of the scene.
         * \return The name of the scene.
         */
        std::string GetSceneName(SceneID scene_id) const;

        /*!
         * \brief Get the SceneID of the active scene.
         * \return The SceneID of the active scene.
         */
        SceneID GetActiveScene() const;

        /*!
         * \brief Get the name of the active scene.
         * \return The name of the active scene.
         */
        std::string GetActiveSceneName() const;

        /*!
         * \brief Get the number of scenes managed by the SceneManager.
         * \return The number of scenes.
         */
        SceneID GetSceneCount() const;

	private:
		SceneID mActiveSceneID	= 0; ///< Unique identifier for active scene
		SceneID mSceneCount		= 0; ///< Keep track of number of scenes

		// Maps
		SceneMap<std::string> mSceneNames;
		SceneMap<Entity> mSceneEntities;
		SceneMap<Map<Entity, Signature>> mSceneEntitySignatures;
		SceneMap<Map<std::string, Entity>> mSceneEntityNames;
		SceneMap<Map<Entity, std::string>> mSceneEntityIds;
		SceneMap<ECSMap> mSceneComponents; // some data structure its an unordered tree :)

        /*!
         * \brief Private constructor for the SceneManager (singleton).
         */
        SceneManager() = default;

        /*!
         * \brief Destructor for the SceneManager.
         */
        ~SceneManager() = default;
	};

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_SCENE_SCENEMANAGER_H
