#include "Message.h"
#include <unordered_map>
#include <vector>
/*this class is to listen to messages and allow for specific systems to subscribe to specific messages
It stores who the subscribers are for the specific message and when the engine broadcasts the message,
all subscribed Systems will recieved the message.
*/
namespace IS {
    class EventManager {
    public:
        void Subscribe(MessageType type, MessageListener* listener);
        void Broadcast(const Message& message);

    private:
        //i create an unorderedmap with MessageType as key and it should store an array of a pointer to message listeners (systems)
        std::unordered_map<MessageType, std::vector<MessageListener*>> handlers;
    };
}