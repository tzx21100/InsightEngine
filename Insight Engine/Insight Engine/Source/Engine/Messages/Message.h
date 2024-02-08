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
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

 /*                                                                   guard
 ----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_
#define GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_

namespace IS {

    /**
     * \enum MessageType
     * \brief Defines the types of messages that can be sent and received by systems.
     */
    enum class MessageType {
        None,       ///< Represents no message or an uninitialized message.
        Quit,       ///< Represents a message indicating the termination or quitting of a process or action.
        Collide,    ///< Represents a collision-related message.
        DebugInfo,  ///< Represents a debug information message.
        RunScene,   ///< Represents the scene running.
        StopScene,  ///< Represent that the scene isn't running.
        SpriteAdded,
        SpriteRemoved
    };

    /**
     * \class Message
     * \brief Represents a basic message that is used to convey information by systems to the engine.
     */
    class Message {
    public:
        /**
         * \brief Constructor that initializes the message with a specific type.
         * \param message The type of the message.
         */
        Message(MessageType message) : type(message), float_value{}, int_value{} {};

        /**
         * \brief Returns the type of the message.
         * \return The message type.
         */
        MessageType GetType() const { return type; }

        /**
         * \brief Returns the float value contained in the message.
         * \return The float value.
         */
        float GetFloat() const { return float_value; }

        /**
         * \brief Returns the integer value contained in the message.
         * \return The integer value.
         */
        int GetInt() const { return int_value; }

        MessageType type;  ///< Holds the type of the message.
        float float_value{}; ///< A float value that the message might carry.
        int int_value{};     ///< An integer value that the message might carry.
        int int_value2{};
    };

    /**
     * \class MessageListener
     * \brief An abstract base class that provides an interface for objects that wish to listen and handle messages.
     */
    class MessageListener {
    public:
        /**
         * \brief A virtual method to handle incoming messages.
         * \param msg The message to be handled.
         *
         * Derived classes should override this method to implement specific behavior for handling different types of messages.
         */
        virtual void HandleMessage(const Message&) {};

        /**
         * \brief Virtual destructor ensuring proper cleanup for derived classes.
         */
        virtual ~MessageListener() {}
    };
}

#endif //GAM200_INSIGHT_ENGINE_SOURCE_MESSAGE_H_