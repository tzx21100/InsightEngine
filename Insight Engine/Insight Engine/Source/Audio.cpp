/* Start Header **************************************************************/
/*!
\file	Audio.cpp
\author Matthew

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
*/
/* End Header ****************************************************************/

//pch has to go to the top of every cpp
#include "Pch.h"
#include "Audio.h"
namespace IS {

    std::string ISAudio::getName() { //for debug purposes
        return "Audio";
    }

    void ISAudio::Initialize() {//call once
        FMOD_RESULT result = FMOD::System_Create(&system); //create fmod system
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
        
        //// Load a sound effect (put full path - not working directory path)
        //FMOD::Channel* soundChannel = ISAudioLoadSound("sound.mp3", true);

        //// Load music (ditto as abv)
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

    void ISAudio::HandleMessage(const Message& message) { //for messaging system
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            std::cout << "Handling Debug" << std::endl;
        }
    }

    // Constructor
    ISAudio::ISAudio() {
        //init member var
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

    bool ISAudio::ISAudioIsValidAudio(FMOD::Sound* audio) { //check if audio valid file
        if (audio) {
            FMOD_RESULT result = audio->getOpenState(nullptr, nullptr, nullptr, nullptr);
            if (result == FMOD_OK) {
                return true; // The audio is valid and can be played
            }
        }
        return false; // The audio is not valid or an error occurred
    }

    bool ISAudio::ISAudioIsValidGroup(FMOD::ChannelGroup* group) { //check group
        return group != nullptr;
    }

    FMOD::ChannelGroup* ISAudio::ISAudioCreateGroup() { 
        // create channel group
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
    FMOD::Channel* ISAudio::ISAudioLoadSound(const char* filePath) { //load sound w/o looping
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

    FMOD::Channel* IS::ISAudio::ISAudioLoadMusic(const char* filePath) { //load music w/o looping
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
        //play audio
        if (audio && group) {
            audio->setChannelGroup(group);
            audio->setVolume(volume);
            audio->setPitch(pitch);
            audio->setLoopCount(loop ? -1 : 0); //if loop variable is false; it'll set to -1 means play audio once only
            audio->setPaused(false);
        }
    }

    void ISAudio::ISAudioResumeGroup(FMOD::ChannelGroup* group) { //resume audio in grp
        if (group) {
            group->setPaused(false);
        }
    }
    
    void ISAudio::ISAudioStopGroup(FMOD::ChannelGroup* group) { //stop audio in grp
        if (group) {
            group->stop();
        }
    }
    
    void ISAudio::ISAudioPauseGroup(FMOD::ChannelGroup* group) { //pause audio in grp
        if (group) {
            group->setPaused(true);
        }
    }

    void ISAudio::ISAudioSetGroupVolume(FMOD::ChannelGroup* group, float volume) {//set vol of grp; 0-1
        if (group) {
            group->setVolume(volume);
        }
    }

    void ISAudio::ISAudioSetGroupPitch(FMOD::ChannelGroup* group, float pitch) { //set pitch of grp; 0-1
        if (group) {
            group->setPitch(pitch);
        }
    }
}
