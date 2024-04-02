/* Start Header **************************************************************/
/*!
 * \file Audio.h
 * \author Matthew Ng, matthewdeen.ng@digipen.edu
 *          Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2451
 * \date 27-09-2023
 * \brief
 * This header file declares the ISAudio class for handling audio in the game engine.
 * The ISAudio class provides functions for initializing, playing, and managing audio and sound groups.
 *
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 */
 /* End Header ****************************************************************/

/* include guards */
#ifndef GAM200_INSIGHT_ENGINE_SOURCE_AUDIO_H
#define GAM200_INSIGHT_ENGINE_SOURCE_AUDIO_H

/*includes */
#include "Engine/ECS/System.h"
#include "Engine/ECS/Component.h"
#include <fmod.hpp>

namespace IS {




    class AudioListener : public IComponent {
    public:
        float volume=1;
        float pitch_correctness{};
        float hearing_range{};

        static std::string GetType() {
            return "AudioListener";
        }

        Json::Value Serialize() override;
        void Deserialize(Json::Value data) override;
    };

    class AudioEmitter : public IComponent {
    public:
        bool isLoop=false;
        bool isPlaying=true;
        FMOD::Channel* Channel=nullptr;
        float falloff_factor=1;
        float volumeLevel=1;
        float pitch = 1;
        std::string soundName="";
        //we are going to use the formula
        //gain=1/distance+fall-off factor higher the falloff factor faster the sound disappears


        ~AudioEmitter() {
            Channel->stop();
        }

        static std::string GetType() {
            return "AudioEmitter";
        }

        Json::Value Serialize() override;
        void Deserialize(Json::Value data) override;
    };


    /*!
     * \brief The ISAudio class handles audio in the game engine.
     *
     * This class provides functions for initializing, playing, and managing audio and sound groups.
     */
    class ISAudio :public ParentSystem {
    public:

        struct AudioConfig
        {
            struct VolumeControl
            {
                bool mIsMute;
                float mVolume;
            };

            VolumeControl mMasterControl;
            VolumeControl mSFXControl;
            VolumeControl mBGMControl;

            AudioConfig()
            {
                mMasterControl = mBGMControl = mSFXControl = { false, 1.f };
            };
        };

        AudioConfig mAudioConfig;

         //float MasterAudioLevel = 1.f;
         //float SFXAudioLevel = 1.f;
         //float BGMAudioLevel = 1.f;

        //override parent sys
        void Update(float deltaTime) override;
        std::string GetName() override;
        void Initialize() override;
        void HandleMessage(const Message& message) override;

        ISAudio(); //ctor
        ~ISAudio(); //dtor

        /*!
         * \brief Releases resources and shuts down the ISAudio system.
         *
         * This function releases all resources associated with the ISAudio system and should be called during cleanup.
         */
        void ISAudioRelease();

        /*!
         * \brief Checks if a given FMOD::Sound* pointer is valid.
         *
         * \param audio A pointer to the FMOD::Sound to be checked.
         * \return True if the sound is valid; otherwise, false.
         */
        bool ISAudioIsValidAudio(FMOD::Sound* audio);

        /*!
         * \brief Checks if a given FMOD::ChannelGroup* pointer is valid.
         *
         * \param group A pointer to the FMOD::ChannelGroup to be checked.
         * \return True if the group is valid; otherwise, false.
         */
        bool ISAudioIsValidGroup(FMOD::ChannelGroup* group);

        /*!
         * \brief Creates a new FMOD::ChannelGroup for audio management.
         *
         * \return A pointer to the created FMOD::ChannelGroup.
         */
        FMOD::ChannelGroup* ISAudioCreateGroup();

        /*!
         * \brief Loads a sound from a file and returns a FMOD::Channel* for playing it.
         *
         * \param file_path The full path to the sound file to be loaded.
         * \return A pointer to the FMOD::Channel for the loaded sound.
         */
        FMOD::Channel* ISAudioLoadSound(const char* file_path);

        /*!
         * \brief Loads a sound from a file and returns a FMOD::Sound* for future use.
         *
         * \param file_path The full path to the sound file to be loaded.
         * \return A pointer to the FMOD::Sound for the loaded sound.
         */
        FMOD::Sound* ISAudioLoadSoundS(const char* file_path);

        /*!
         * \brief Loads music from a file and returns a FMOD::Channel* for playing it.
         *
         * \param file_path The full path to the music file to be loaded.
         * \return A pointer to the FMOD::Channel for the loaded music.
         */
        FMOD::Channel* ISAudioLoadMusic(const char* file_path);

        /*!
         * \brief Plays a sound on a specified audio channel and group.
         *
         * \param audio A pointer to the FMOD::Channel representing the audio.
         * \param group A pointer to the FMOD::ChannelGroup representing the group.
         * \param volume The volume at which to play the sound (0-1).
         * \param pitch The pitch at which to play the sound (0-1).
         * \param loop Specifies whether the sound should loop (default is false).
         */
        void ISAudioPlay(FMOD::Channel* audio, FMOD::ChannelGroup* group, float volume, float pitch, bool loop = false);

        /*!
         * \brief Resumes playback of all sounds in a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup to be resumed.
         */
        void ISAudioResumeGroup(FMOD::ChannelGroup* group);

        /*!
         * \brief Stops playback of all sounds in a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup to be stopped.
         */
        void ISAudioStopGroup(FMOD::ChannelGroup* group);

        /*!
         * \brief Pauses playback of all sounds in a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup to be paused.
         */
        void ISAudioPauseGroup(FMOD::ChannelGroup* group);

        /*!
         * \brief Sets the volume of a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup whose volume to set.
         * \param volume The volume to set (0-1).
         */
        void ISAudioSetGroupVolume(FMOD::ChannelGroup* group, float volume);

        /*!
         * \brief Sets the pitch of a specified audio group.
         *
         * \param group A pointer to the FMOD::ChannelGroup whose pitch to set.
         * \param pitch The pitch to set (0-1).
         */
        void ISAudioSetGroupPitch(FMOD::ChannelGroup* group, float pitch);

        /*!
         * \brief Checks if a sound is currently playing on a specified audio channel.
         *
         * \param channel A pointer to the FMOD::Channel to be checked.
         * \return True if the sound is currently playing; otherwise, false.
         */
        bool IsSoundPlaying(FMOD::Channel* channel);

        /*!
         * \brief Plays a sound on a specified audio channel.
         *
         * \param sound A pointer to the FMOD::Sound representing the sound to be played.
         * \param loop Specifies whether the sound should loop.
         * \param volume The volume at which to play the sound (0-1).
         * \param pitch The pitch at which to play the sound (0-1).
         * \return A pointer to the FMOD::Channel representing the played sound.
         */
        FMOD::Channel* PlaySound(FMOD::Sound* sound, bool loop, float volume, float pitch);
        FMOD::Channel* PlaySoundCheck(FMOD::Sound* Ssound, FMOD::Channel* channel, bool loop, float volume, float pitch);

        void FadeOutAudio( float time_to_fade );

        void StopAllAudio();

        void SetMasterVolume(float volume);

    private:
        FMOD::System* mSystem;
        FMOD::Sound* mSound;
        FMOD::Channel* mChannel;

        std::vector<FMOD::Channel*> mChannelList;

        //for fade
        bool mFadeOutAudio = false;
        float mCurrentVolume = 0.f;
        float mFadeTime = 0.f;

        const std::string CONFIG_FILEPATH = "AudioConfig.json";

        void LoadConfig();
        void SaveConfig();


    };



    class AudioEmitterSystem :public ParentSystem {
    public:
        //override parent sys
        void Update([[maybe_unused]] float deltaTime) override;
        std::string GetName() override { return "AudioEmitter"; }
        void Initialize() override {}
        void HandleMessage(const Message& message) override {
            if (message.GetType() == MessageType::DebugInfo) {
                IS_CORE_INFO("Handling Debug");
            }
        }
        std::set<Entity> GetEntities() { return mEntities; }
    };


}
#endif // GAM200_INSIGHT_ENGINE_SOURCE_AUDIO_H  