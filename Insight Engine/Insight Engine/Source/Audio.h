/* Start Header **************************************************************/
/*!
\file	Audio.h
\author Matthew

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/

#ifndef GAM200_INSIGHT_ENGINE_SOURCE_AUDIO_H
#define GAM200_INSIGHT_ENGINE_SOURCE_AUDIO_H

#include "System.h"
#include "fmod.hpp"

namespace IS {
    class ISAudio :public ParentSystem {
    public:
        //override parent sys
        void Update(float deltaTime) override;
        std::string GetName() override;
        void Initialize() override;
        void HandleMessage(const Message& message) override;

        ISAudio(); //ctor
        ~ISAudio(); //dtor
        void ISAudioRelease(); //exits audio sys
        
        bool ISAudioIsValidAudio(FMOD::Sound* audio); //check audio valid anot
        bool ISAudioIsValidGroup(FMOD::ChannelGroup* group); //check grp valid anot
        FMOD::ChannelGroup* ISAudioCreateGroup(); //create audio grp
        FMOD::Channel* ISAudioLoadSound(const char* file_path); //load sound file (pass in full path)
        FMOD::Sound* ISAudioLoadSoundS(const char* file_path);
        FMOD::Channel* ISAudioLoadMusic(const char* file_path); //load music file (pass in full path)

        void ISAudioPlay(FMOD::Channel* audio, FMOD::ChannelGroup* group, float volume, float pitch, bool loop = false); // play sound; default no looped audio
        void ISAudioResumeGroup(FMOD::ChannelGroup* group);
        void ISAudioStopGroup(FMOD::ChannelGroup* group); 
        void ISAudioPauseGroup(FMOD::ChannelGroup* group);
        void ISAudioSetGroupVolume(FMOD::ChannelGroup* group, float volume); // volume 0-1
        void ISAudioSetGroupPitch(FMOD::ChannelGroup* group, float pitch);//pitch 0-1

        FMOD::System* GetSystem() { return system; }

        //specific functions made for rubrics
        // Check if a sound is playing
        bool IsSoundPlaying(FMOD::Channel* channel);
        FMOD::Channel* PlaySound(FMOD::Sound* sound, bool loop, float volume, float pitch);

    private:
        FMOD::System* system;
        FMOD::Sound* sound;
        FMOD::Channel* channel;
    };
}
#endif // GAM200_INSIGHT_ENGINE_SOURCE_AUDIO_H  