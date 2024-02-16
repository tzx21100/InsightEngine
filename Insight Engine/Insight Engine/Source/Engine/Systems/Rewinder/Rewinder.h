#include "../../ECS/Component.h"
#include "../../ECS/System.h"
#include "../../../Physics/System/Physics.h"

namespace IS {

	class RewinderComponent : IComponent {
	public:
		bool mIsRecording = false;
		bool mRecordPhysics = false;
		bool mRecordAnimations = false;
		float time_to_save = 1.f;



	};

	class Rewinder : public ParentSystem {
	public:
        std::string GetName() {
            return "Rewinder";
        }



        void Initialize(){}


        void Update([[maybe_unused]] float delta_time);


		//save a copy of the component
		std::unordered_map<Entity, RigidBody> mRigidBodies;
		std::unordered_map<Entity, Transform> mTransforms;

	};




}