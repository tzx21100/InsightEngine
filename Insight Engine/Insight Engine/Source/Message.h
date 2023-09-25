/*!
 * \file Message.h
 * \author Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-09-2023
 * \brief
 * This source file declares the Message, MessageListener and MessageType
 * classes, these classes are mainly used for the systems and engine to 
 * communicate in situations where components are not enough. These can be
 * direct events such as quitting or debugging system to system.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
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
			Message(MessageType message) : type(message), float_value{}, int_value{} {};
			MessageType GetType() const { return type; }
			float GetFloat() const { return float_value; }
			int GetInt() const { return int_value; }
	    private:
			MessageType type;
			//these can be additional stuff messages hold more can be added in the future for whichever uses we need
			float float_value;
			int int_value;
    };

	//create a class to listen to messages
    class MessageListener {
    public:
	    virtual void HandleMessage(const Message&) {};
	    virtual ~MessageListener() {}

    };

}

#endif //GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_