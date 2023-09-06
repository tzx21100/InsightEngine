#ifndef GAM200_INSIGHT_ENGINE_SOURCE_AUDIO_H
#define GAM200_INSIGHT_ENGINE_SOURCE_AUDIO_H

#include "System.h"
#include "fmod.hpp"

namespace IS {
    class ISAudio :public ParentSystem {
    public:
        //override parent
        void Update(float deltaTime) override;
        std::string getName() override;
        void Initialize() override;
        void HandleMessage(const Message& message) override;

        ISAudio();
        ~ISAudio();
        void ISAudioRelease(); //exits audio sys
        
        bool ISAudioIsValidAudio(FMOD::Sound* audio);
        bool ISAudioIsValidGroup(FMOD::ChannelGroup* group); 
        FMOD::ChannelGroup* ISAudioCreateGroup();
        FMOD::Channel* ISAudioLoadSound(const char* filePath); 
        FMOD::Channel* ISAudioLoadMusic(const char* filePath);

        void ISAudioPlay(FMOD::Channel* audio, FMOD::ChannelGroup* group, float volume, float pitch, bool loop = false); // Fixed the parameters
        void ISAudioResumeGroup(FMOD::ChannelGroup* group);
        void ISAudioStopGroup(FMOD::ChannelGroup* group); 
        void ISAudioPauseGroup(FMOD::ChannelGroup* group);
        void ISAudioSetGroupVolume(FMOD::ChannelGroup* group, float volume);
        void ISAudioSetGroupPitch(FMOD::ChannelGroup* group, float pitch);

    private:
        FMOD::System* system;
        FMOD::Sound* sound;
        FMOD::Channel* channel;
    };
}

#endif // GAM200_INSIGHT_ENGINE_SOURCE_AUDIO_H  