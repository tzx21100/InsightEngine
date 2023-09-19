//pch has to go to the top of every cpp
#include "Pch.h"
#include "Audio.h"
namespace IS {

    std::string ISAudio::getName() {
        return "Audio";
    }

    void ISAudio::Initialize() {//call once
        //constuctor init alr
        FMOD_RESULT result = FMOD::System_Create(&system);
        if (result != FMOD_OK) {
            // Handle initialization error
            return;
        }

        result = system->init(32, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            // Handle initialization error
            return;
        }


    }

    void ISAudio::Update([[maybe_unused]] float deltaTime) {//every frame
        //update the audio system
        system->update();

        //these commented codes are an example of how the sound system work
        
        //// Load a sound effect
        //FMOD::Channel* soundChannel = ISAudioLoadSound("sound.mp3", true);

        //// Load music
        //FMOD::Channel* musicChannel = ISAudioLoadMusic("music.MP3", true);

        //FMOD::ChannelGroup* soundGroup = ISAudioCreateGroup();
        //FMOD::ChannelGroup* musicGroup = ISAudioCreateGroup();

        //// Check if loading was successful
        //if (!soundChannel || !musicChannel) {
        //    // Handle loading errors
        //    return;
        //}
        //// Play the sound effect
        //ISAudioPlay(soundChannel, soundGroup, 1.0f, 1.0f, false);

        //// Play the background music
        //ISAudioPlay(musicChannel, musicGroup, 1.0f, 1.0f, true);
    }

    void ISAudio::HandleMessage(const Message& message) {
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            std::cout << "Handling Debug" << std::endl;
        }
    }

    // Constructor
    ISAudio::ISAudio() {
        system = nullptr;
        sound = nullptr;
        channel = nullptr;
    }

    // Destructor
    ISAudio::~ISAudio() {
        ISAudioRelease();
    }

    // Release all resources
    void ISAudio::ISAudioRelease() {
        if (channel) {
            channel->stop();
        }

        if (sound) {
            sound->release();
        }

        if (system) {
            system->close();
            system->release();
        }
    }

    bool ISAudio::ISAudioIsValidAudio(FMOD::Sound* audio) {
        if (audio) {
            FMOD_RESULT result = audio->getOpenState(nullptr, nullptr, nullptr, nullptr);
            if (result == FMOD_OK) {
                return true; // The audio is valid and can be played
            }
        }
        return false; // The audio is not valid or an error occurred
    }

    bool ISAudio::ISAudioIsValidGroup(FMOD::ChannelGroup* group) {
        return group != nullptr;
    }

    FMOD::ChannelGroup* ISAudio::ISAudioCreateGroup() { 
        FMOD::ChannelGroup* new_group = nullptr;
        FMOD_RESULT result = system->createChannelGroup(nullptr, &new_group);

        if (result != FMOD_OK) {
            // Handle group creation error
            std::cout << "group creation error";
            return nullptr; // Return nullptr on error
        }

        return new_group;
    }

#pragma warning(push)
#pragma warning(disable: 4458)
    FMOD::Channel* ISAudio::ISAudioLoadSound(const char* filePath) {
        FMOD_MODE mode = FMOD_LOOP_OFF;
        FMOD::Sound* sound = nullptr;

        FMOD_RESULT result = system->createSound(filePath, mode, nullptr, &sound);
        if (result != FMOD_OK) {
            // Handle sound loading error
            return nullptr; // Return nullptr on error
        }

        FMOD::Channel* sound_channel = nullptr;
        result = system->playSound(sound, nullptr, false, &sound_channel);
        if (result != FMOD_OK) {
            // Handle sound playing error
            return nullptr; // Return nullptr on error
        }

        return sound_channel;
    }
#pragma warning(pop)

    FMOD::Channel* IS::ISAudio::ISAudioLoadMusic(const char* filePath) {
        FMOD_MODE mode = FMOD_LOOP_OFF;
        FMOD::Sound* music = nullptr;

        FMOD_RESULT result = system->createSound(filePath, mode, nullptr, &music);
        if (result != FMOD_OK) {
            // Handle sound loading error
            return nullptr; // Return nullptr on error
        }

        FMOD::Channel* music_channel = nullptr;
        result = system->playSound(music, nullptr, false, &music_channel);
        if (result != FMOD_OK) {
            // Handle sound playing error
            return nullptr; // Return nullptr on error
        }

        return music_channel;
    }

    void ISAudio::ISAudioPlay(FMOD::Channel* audio, FMOD::ChannelGroup* group, float volume, float pitch, bool loop) {
        if (audio && group) {
            audio->setChannelGroup(group);
            audio->setVolume(volume);
            audio->setPitch(pitch);
            audio->setLoopCount(loop ? -1 : 0);
            audio->setPaused(false);
        }
    }

    void ISAudio::ISAudioResumeGroup(FMOD::ChannelGroup* group) {
        if (group) {
            group->setPaused(false);
        }
    }
    
    void ISAudio::ISAudioStopGroup(FMOD::ChannelGroup* group) {
        if (group) {
            group->stop();
        }
    }
    
    void ISAudio::ISAudioPauseGroup(FMOD::ChannelGroup* group) {
        if (group) {
            group->setPaused(true);
        }
    }

    void ISAudio::ISAudioSetGroupVolume(FMOD::ChannelGroup* group, float volume) {
        if (group) {
            group->setVolume(volume);
        }
    }

    void ISAudio::ISAudioSetGroupPitch(FMOD::ChannelGroup* group, float pitch) {
        if (group) {
            group->setPitch(pitch);
        }
    }
}
