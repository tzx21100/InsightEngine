/* Start Header **************************************************************/
/*!
 * \file Audio.cpp
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 *          Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2451
 * \date 27-09-2023
 * \brief
 * Implementation of the ISAudio class for handling audio in the game engine.
 *
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 */
 /* End Header ****************************************************************/

 // Include the precompiled header at the top of every cpp file
#include "Pch.h"
#include "Audio.h"
#include "Engine/Core/CoreEngine.h"
#include "../Window/WindowSystem.h"
#include "Engine/Systems/Asset/Asset.h"
#include "Graphics/Core/Graphics.h"
#include "Math/ISMath.h"
#include "Engine/JSON/JsonSaveLoad.h"
#include <iostream>

namespace IS {
    
    std::string ISAudio::GetName() {
        /*!
         * \brief Gets the name of the ISAudio system.
         *
         * \return A string containing the name of the ISAudio system.
         */
        return "Audio";
    }

    void ISAudio::Initialize() {
        /*!
         * \brief Initializes the ISAudio system.
         *
         * This function sets up the ISAudio system and should be called before any other ISAudio operations.
         */
        FMOD_RESULT result = FMOD::System_Create(&mSystem);
        if (result != FMOD_OK) {
            // Handle initialization error
            return;
        }

        result = mSystem->init(128, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            // Handle initialization error
            return;
        }
        LoadConfig();
    }

    // Function to calculate distance between two points in 2D
    float CalculateDistance(float x1, float y1, float x2, float y2) {
        return std::sqrtf(std::powf(x2 - x1, 2) + std::powf(y2 - y1, 2));
    }

    // Function to calculate the gain (volume level) based on distance
    float CalculateGain(float distance, float rollOffFactor = 1.0f) {
        // Make sure distance is never zero to avoid division by zero
        distance = std::max(distance, 0.0001f);
        return 1.0f / (distance/100 * rollOffFactor);
    }

    /*!
     * \brief Updates the ISAudio system.
     *
     * This function is called each frame to update the ISAudio system.
     *
     * \param deltaTime The time elapsed since the last frame in seconds.
     */
    void ISAudio::Update([[maybe_unused]] float deltaTime) {
        for (auto& i : mChannelList)
        {
            i->setMute(mAudioConfig.mBGMControl.mIsMute);
        }

        mSystem->update();
        if (InsightEngine::Instance().mRuntime == false) {
            auto sys = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
            sys->ClearAllSounds();
            mChannel->stop();

            for (auto& i : mChannelList) {
                i->stop();
            }

            return; 
        }

        if (!InsightEngine::Instance().GetSystem<WindowSystem>("Window")->IsFocused()) {

            auto sys = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
            sys->ClearAllSounds();
            //sys->ClearAllSounds();
            mChannel->setPaused(true);

            for (auto& i : mChannelList) {
                i->setPaused(true);
            }

        }
        else {
            mChannel->setPaused(false);
            for (auto& i : mChannelList) {
                i->setPaused(false);
            }
        }


        auto& engine = InsightEngine::Instance();
        auto assetsys = engine.GetSystem<AssetManager>("Asset");
        auto mEmitterEntities = engine.GetSystem<AudioEmitterSystem>("AudioEmitter")->GetEntities();
        for (auto const &entity : mEntities) {
            auto listener = engine.GetComponent<AudioListener>(entity);
            auto current_entity_transform = engine.GetComponent<Transform>(entity);
            for (auto emittingEntities : mEmitterEntities) {
                auto& emitting_transform = engine.GetComponent<Transform>(emittingEntities);
                auto &emitter = engine.GetComponent<AudioEmitter>(emittingEntities);

                if (emitter.isPlaying == false ) { continue; }

                float distance = CalculateDistance(current_entity_transform.world_position.x,
                                                    current_entity_transform.world_position.y,
                                                    emitting_transform.world_position.x,
                                                    emitting_transform.world_position.y
                                                    );
                distance = Max(distance, 100.f);

                if (distance > listener.hearing_range) {
                   emitter.Channel->stop();
                    continue;
                }
                //If they are close enough
                    
                float volume = CalculateGain(distance, emitter.falloff_factor);
                
                if (IsSoundPlaying(emitter.Channel)){
                    emitter.Channel->setVolume(emitter.volumeLevel * listener.volume * volume *mAudioConfig.mMasterControl.mVolume *2.f * mAudioConfig.mBGMControl.mVolume);
                }
                else {
                    emitter.Channel=PlaySoundCheck(assetsys->GetSound(emitter.soundName),emitter.Channel, emitter.isLoop, emitter.volumeLevel * listener.volume * volume * mAudioConfig.mMasterControl.mVolume *2.f * mAudioConfig.mBGMControl.mVolume, emitter.pitch);
                    mChannelList.emplace_back(emitter.Channel);
                }

            }

            
        
        }



        if (mFadeOutAudio) {
            mCurrentVolume -= deltaTime/mFadeTime;
            auto sys = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
            auto& channels = sys->mChannelList;
            for (auto& i : mChannelList) {
                float vol;
                i->getVolume(&vol);
                i->setVolume(vol*mCurrentVolume);
                //std::cout<< mCurrentVolume * vol << std::endl;
            }
            //asset channel list
            for (auto& i : channels) {
                float vol=0.2f;
                FMOD_RESULT result;
                result=i.second->getVolume(&vol);
                if (result != FMOD_OK) {
                    // Handle error appropriately
                    vol = 0.02f;
                }
                else {
                    i.second->setVolume(vol * mCurrentVolume);
                }

            }
            if (mCurrentVolume <= 0) {
                StopAllAudio();
                mFadeOutAudio = false;
                mCurrentVolume = 1.f;
           }
        }

        

       
    }

    void ISAudio::HandleMessage(const Message& message) {
        /*!
         * \brief Handles messages sent to the ISAudio system.
         *
         * This function handles messages sent to the ISAudio system and can be used for communication between systems.
         *
         * \param message The message to be handled.
         */
        if (message.GetType() == MessageType::DebugInfo) {
            // Handle collision logic here
            IS_CORE_INFO("Handling Debug");
        }
    }

    ISAudio::ISAudio() {
        /*!
         * \brief Constructor for the ISAudio class.
         *
         * Initializes member variables.
         */
        mSystem = nullptr;
        mSound = nullptr;
        mChannel = nullptr;
        mAudioConfig = AudioConfig();
    }

    ISAudio::~ISAudio() {
        /*!
         * \brief Destructor for the ISAudio class.
         *
         * Releases all resources associated with the ISAudio system.
         */
        ISAudioRelease();
        SaveConfig();
    }

    void ISAudio::LoadConfig()
    {
        Json::Value config;
        bool success = LoadJsonFromFile(config, CONFIG_FILEPATH);
        if (success)
        {
            auto& audio_config = config["AudioConfig"];
            mAudioConfig.mMasterControl.mIsMute = audio_config["Master"]["Mute"].asBool();
            mAudioConfig.mMasterControl.mVolume = audio_config["Master"]["Volume"].asFloat();
            mAudioConfig.mBGMControl.mIsMute = audio_config["BGM"]["Mute"].asBool();
            mAudioConfig.mBGMControl.mVolume = audio_config["BGM"]["Volume"].asFloat();
            mAudioConfig.mSFXControl.mIsMute = audio_config["SFX"]["Mute"].asBool();
            mAudioConfig.mSFXControl.mVolume = audio_config["SFX"]["Volume"].asFloat();
            IS_CORE_INFO("Loaded audio config from \"{}\"", CONFIG_FILEPATH);
        }
        else
        {
            mAudioConfig = AudioConfig();
            IS_CORE_INFO("Using default audio config");
        }
    }

    void ISAudio::SaveConfig()
    {
        Json::Value config;
        auto& audio_config = config["AudioConfig"];
        Json::Value master;
        master["Volume"] = mAudioConfig.mMasterControl.mVolume;
        master["Mute"] = mAudioConfig.mMasterControl.mIsMute;
        audio_config["Master"] = master;

        Json::Value bgm;
        bgm["Volume"] = mAudioConfig.mBGMControl.mVolume;
        bgm["Mute"] = mAudioConfig.mBGMControl.mIsMute;
        audio_config["BGM"] = bgm;

        Json::Value sfx;
        sfx["Volume"] = mAudioConfig.mSFXControl.mVolume;;
        sfx["Mute"] = mAudioConfig.mSFXControl.mIsMute;
        audio_config["SFX"] = sfx;

        bool success = SaveJsonToFile(config, CONFIG_FILEPATH);
        success ? IS_CORE_INFO("Successfully saved audio config to \"{}\"!", CONFIG_FILEPATH) :
            IS_CORE_ERROR("Failed to save audio config to \"{}\"!", CONFIG_FILEPATH);
    }

    void ISAudio::ISAudioRelease() {
        /*!
         * \brief Releases resources and shuts down the ISAudio system.
         *
         * This function releases all resources associated with the ISAudio system and should be called during cleanup.
         */
        if (mChannel) {
            mChannel->stop();
        }

        if (mSound) {
            mSound->release();
        }

        if (mSystem) {
            mSystem->close();
            mSystem->release();
        }
    }

    bool ISAudio::ISAudioIsValidAudio(FMOD::Sound* audio) {
        /*!
         * \brief Checks if a given FMOD::Sound* pointer is valid.
         *
         * \param audio A pointer to the FMOD::Sound to be checked.
         * \return True if the sound is valid; otherwise, false.
         */
        if (audio) {
            FMOD_RESULT result = audio->getOpenState(nullptr, nullptr, nullptr, nullptr);
            if (result == FMOD_OK) {
                return true; // The audio is valid and can be played
            }
        }
        return false; // The audio is not valid or an error occurred
    }

    bool ISAudio::ISAudioIsValidGroup(FMOD::ChannelGroup* group) {
        /*!
         * \brief Checks if a given FMOD::ChannelGroup* pointer is valid.
         *
         * \param group A pointer to the FMOD::ChannelGroup to be checked.
         * \return True if the group is valid; otherwise, false.
         */
        return group != nullptr;
    }

    FMOD::ChannelGroup* ISAudio::ISAudioCreateGroup() {
        /*!
         * \brief Creates a new FMOD::ChannelGroup for audio management.
         *
         * \return A pointer to the created FMOD::ChannelGroup.
         */
        FMOD::ChannelGroup* new_group = nullptr;
        FMOD_RESULT result = mSystem->createChannelGroup(nullptr, &new_group);

        if (result != FMOD_OK) {
            // Handle group creation error
            IS_CORE_ERROR("Group Creation Failed!");
            return nullptr; // Return nullptr on error
        }

        return new_group;
    }

    void ISAudio::StopAllAudio() {
        auto sys = InsightEngine::Instance().GetSystem<AssetManager>("Asset");
        sys->ClearAllSounds();
        mChannel->stop();

        for (auto& i : mChannelList) {
            i->stop();
        }
    
    }

    void ISAudio::FadeOutAudio(float time_to_fade) {

        mFadeOutAudio = true;
        mCurrentVolume = 1.f;
        mFadeTime = time_to_fade;

    }

    

#pragma warning(push)
#pragma warning(disable: 4458)
    FMOD::Channel* ISAudio::ISAudioLoadSound(const char* file_path) {
        /*!
         * \brief Loads a sound from a file and returns a FMOD::Channel* for playing it.
         *
         * \param file_path The full path to the sound file to be loaded.
         * \return A pointer to the FMOD::Channel for the loaded sound.
         */
        FMOD_MODE mode = FMOD_LOOP_OFF;
        FMOD::Sound* sound = nullptr;

        FMOD_RESULT result = mSystem->createSound(file_path, mode, nullptr, &sound);
        if (result != FMOD_OK) {
            // Handle sound loading error
            return nullptr; // Return nullptr on error
        }

        FMOD::Channel* sound_channel = nullptr;
        //result = system->playSound(sound, nullptr, false, &sound_channel);
        if (result != FMOD_OK) {
            // Handle sound playing error
            return nullptr; // Return nullptr on error
        }

        return sound_channel;
    }

    FMOD::Sound* ISAudio::ISAudioLoadSoundS(const char* file_path) {
        /*!
         * \brief Loads a sound from a file and returns a FMOD::Sound* for future use.
         *
         * \param file_path The full path to the sound file to be loaded.
         * \return A pointer to the FMOD::Sound for the loaded sound.
         */
        FMOD_MODE mode = FMOD_LOOP_OFF;
        FMOD::Sound* sound = nullptr;

        FMOD_RESULT result = mSystem->createSound(file_path, mode, nullptr, &sound);
        if (result != FMOD_OK) {
            // Handle sound loading error
            return nullptr;
        }
        return sound; // Return the loaded sound
    }

#pragma warning(pop)

    FMOD::Channel* ISAudio::ISAudioLoadMusic(const char* file_path) {
        /*!
         * \brief Loads music from a file and returns a FMOD::Channel* for playing it.
         *
         * \param file_path The full path to the music file to be loaded.
         * \return A pointer to the FMOD::Channel for the loaded music.
         */
        FMOD_MODE mode = FMOD_LOOP_OFF;
        FMOD::Sound* music = nullptr;

        FMOD_RESULT result = mSystem->createSound(file_path, mode, nullptr, &music);
        if (result != FMOD_OK) {
            // Handle sound loading error
            return nullptr; // Return nullptr on error
        }

        FMOD::Channel* music_channel = nullptr;
        result = mSystem->playSound(music, nullptr, false, &music_channel);
        if (result != FMOD_OK) {
            // Handle sound playing error
            return nullptr; // Return nullptr on error
        }

        return music_channel;
    }

    void ISAudio::ISAudioPlay(FMOD::Channel* audio, FMOD::ChannelGroup* group, float volume, float pitch, bool loop) {
        /*!
         * \brief Plays a sound on a specified audio channel and group.
         *
         * \param audio A pointer to the FMOD::Channel representing the audio.
         * \param group A pointer to the FMOD::ChannelGroup representing the group.
         * \param volume The volume at which to play the sound (0-1).
         * \param pitch The pitch at which to play the sound (0-1).
         * \param loop Specifies whether the sound should loop (default is false).
         */
        if (audio && group) {
            audio->setChannelGroup(group);
            audio->setVolume(volume);
            audio->setPitch(pitch);
            audio->setLoopCount(loop ? -1 : 0);
            audio->setPaused(false);
        }
    }

    void ISAudio::ISAudioResumeGroup(FMOD::ChannelGroup* group) {
        /*!
         * \brief Resumes playback of all sounds in a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup to be resumed.
         */
        if (group) {
            group->setPaused(false);
        }
    }

    void ISAudio::ISAudioStopGroup(FMOD::ChannelGroup* group) {
        /*!
         * \brief Stops playback of all sounds in a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup to be stopped.
         */
        if (group) {
            group->stop();
        }
    }

    void ISAudio::ISAudioPauseGroup(FMOD::ChannelGroup* group) {
        /*!
         * \brief Pauses playback of all sounds in a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup to be paused.
         */
        if (group) {
            group->setPaused(true);
        }
    }

    void ISAudio::ISAudioSetGroupVolume(FMOD::ChannelGroup* group, float volume) {
        /*!
         * \brief Sets the volume of a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup whose volume to set.
         * \param volume The volume to set (0-1).
         */
        if (group) {
            group->setVolume(volume);
        }
    }

    void ISAudio::ISAudioSetGroupPitch(FMOD::ChannelGroup* group, float pitch) {
        /*!
         * \brief Sets the pitch of a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup whose pitch to set.
         * \param pitch The pitch to set (0-1).
         */
        if (group) {
            group->setPitch(pitch);
        }
    }

    FMOD::Channel* ISAudio::PlaySound(FMOD::Sound* Ssound, bool loop, float volume, float pitch) {
        /*!
         * \brief Plays a sound on a specified audio channel.
         *
         * \param Ssound A pointer to the FMOD::Sound representing the sound to be played.
         * \param loop Specifies whether the sound should loop.
         * \param volume The volume at which to play the sound (0-1).
         * \param pitch The pitch at which to play the sound (0-1).
         * \return A pointer to the FMOD::Channel representing the played sound.
         */
        if (!Ssound) return nullptr;

        FMOD::Channel* Cchannel = nullptr;
        FMOD_RESULT result = mSystem->playSound(Ssound, nullptr, true, &Cchannel);

        if (result != FMOD_OK || !Cchannel) {
            // Handle error
            return nullptr;
        }

        Cchannel->setVolume(volume);
        Cchannel->setPitch(pitch);
        Cchannel->setLoopCount(loop ? -1 : 0); // -1 for infinite loop, 0 for no loop
        Cchannel->setPaused(false);

        return Cchannel;
    }

    FMOD::Channel* ISAudio::PlaySoundCheck(FMOD::Sound* Ssound,FMOD::Channel* channel, bool loop, float volume, float pitch) {

        if (!Ssound) return nullptr;

       channel->stop();
        FMOD_RESULT result = mSystem->playSound(Ssound, nullptr, true, &channel);

        if (result != FMOD_OK || !channel) {
            // Handle error
            return nullptr;
        }

        channel->setVolume(volume);
        channel->setPitch(pitch);
        channel->setLoopCount(loop ? -1 : 0); // -1 for infinite loop, 0 for no loop
        channel->setPaused(false);

        return channel;
    }


    bool ISAudio::IsSoundPlaying(FMOD::Channel* Cchannel) {
        /*!
         * \brief Checks if a sound is currently playing on a specified audio channel.
         *
         * \param Cchannel A pointer to the FMOD::Channel to be checked.
         * \return True if the sound is currently playing; otherwise, false.
         */
        if (!Cchannel) return false;

        bool is_playing = false;
        Cchannel->isPlaying(&is_playing);
        return is_playing;
    }

    void ISAudio::SetMasterVolume(float volume)
    {
        mAudioConfig.mMasterControl.mVolume = volume;
    }

    Json::Value AudioListener::Serialize()
    {
        Json::Value data;
        data["AudioVolume"]           = volume;
        data["AudioPitchCorrectness"] = pitch_correctness;
        data["AudioHearingRange"]     = hearing_range;

        return data;
    }

    void AudioListener::Deserialize(Json::Value data)
    {
        volume            = data["AudioVolume"].asFloat();
        pitch_correctness = data["AudioPitchCorrectness"].asFloat();
        hearing_range     = data["AudioHearingRange"].asFloat();
    }

    Json::Value AudioEmitter::Serialize()
    {
        Json::Value data;
        data["AudioIsLoop"]        = isLoop;
        data["AudioFalloffFactor"] = falloff_factor;
        data["AudioVolumeLevel"]   = volumeLevel;
        data["AudioPitch"]         = pitch;
        data["AudioSoundName"]     = soundName;

        return data;
    }

    void AudioEmitter::Deserialize(Json::Value data)
    {
        isLoop         = data["AudioIsLoop"].asBool();
        falloff_factor = data["AudioFalloffFactor"].asFloat();
        volumeLevel    = data["AudioVolumeLevel"].asFloat();
        pitch          = data["AudioPitch"].asFloat();
        soundName      = data["AudioSoundName"].asString();
    }


    void AudioEmitterSystem::Update([[maybe_unused]] float deltaTime) {
        if (InsightEngine::Instance().mRuntime == false) {
            for (auto& emittingEntities : mEntities) {
                auto& emitter = InsightEngine::Instance().GetComponent<AudioEmitter>(emittingEntities);
                if (&emitter == nullptr) { continue; }
                emitter.Channel->stop();
                
            }
            return;
        }

    }







}
