#ifndef GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_

namespace Zx_Framework {

	enum class aMessageIdType {
	    None,
	    Quit,
	    Collide,
	    DebugInfo
	};

	//Basic message is the way information is sent by systems to engine
    class Message {
	    public:
		    Message(aMessageIdType id) : MessageId(id) {};
		    aMessageIdType MessageId =aMessageIdType::None;
		    virtual ~Message();
	};
}

#endif //GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_