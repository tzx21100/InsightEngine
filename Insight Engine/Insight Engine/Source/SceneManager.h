#include "CoreEngine.h"

namespace IS {


	using SceneNumber= uint8_t;

	class SceneManager {

	public:

		std::unordered_map<SceneNumber, Entity> mSceneEntities;
		std::unordered_map<SceneNumber, IComponentArray> mSceneComponents;

		void SaveScene() {
			auto &engine= InsightEngine::Instance();
			mSceneEntities.insert({mCurrentSceneNumber,engine.EntitiesAlive()});
			mSceneComponents.insert({ mCurrentSceneNumber,SaveAllComponents(); })
		}

		IComponentArray SaveAllComponents() {
			
		
		}

		void LoadScene(SceneNumber scene) {
		
		}

	private:
		SceneNumber mCurrentSceneNumber;


	};



}