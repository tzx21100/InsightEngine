#ifndef GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_

namespace IS {

	enum class MessageType {
	    None,
	    Quit,
	    Collide,
	    DebugInfo
	};

	//Basic message is the way information is sent by systems to engine
    class Message {
	    public:
		    Message(MessageType message) : type(message) {};
			MessageType GetType() const { return type; }
	    private:
			MessageType type;
    };

	//create a class to listen to messages
    class MessageListener {
    public:
	    virtual void HandleMessage(const Message& message) = 0;
	    virtual ~MessageListener() {}

    };

}

#endif //GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_