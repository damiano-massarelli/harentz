#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <SDL_mixer.h>
#include <SDL.h>
#include <memory>
#include <map>

/** \brief simple audio lib that simplifies sdl mixer. */
class AudioManager
{
    private:
        /** \brief unique ptr deleter for chunks */
        struct ChunkDeleter {
            void operator()(Mix_Chunk* chunk) {
                Mix_FreeChunk(chunk);
            }
        };
        using chunkUniquePtr = std::unique_ptr<Mix_Chunk, ChunkDeleter>;

        struct MusicDeleter {
            void operator()(Mix_Music* music) {
                Mix_FreeMusic(music);
            }
        };
        using musicUniquePtr = std::unique_ptr<Mix_Music, MusicDeleter>;

    private:
        static std::unique_ptr<AudioManager> instance;

        std::map<int, chunkUniquePtr> m_channel2sound;
        std::map<std::string, chunkUniquePtr> m_cachedSounds;
        musicUniquePtr m_currentMusic;
        float m_volume = 1.0f;

        void attachChannel(int channel, Mix_Chunk* chunk);

    public:
        static AudioManager* getInstance();

        AudioManager();

        /** \brief loads a sound so that it is cached
          * when a sound is played through playSound the file is loaded in memory and deleted
          * once it is over. To avoid this overhead, loaded sounds are kept in memory */
        void loadSound(const std::string& fileName);

        /** \brief plays a sound.
          * Loaded sounds are not read from disk
          * \return the channel on which the sound is being played */
        int playSound(const std::string& fileName);

        /** \brief streams a sound
          * Music is different from sounds in that it is always streamed. Hence,
          * there is no such thing as loadMusic */
        void playMusic(const std::string& fileName, int loops = 0);

        /** \brief mutes all the sounds */
        void muteAll();

        void pauseMusic();

        void resumeMusic(float fadeInMs = 0);

        bool isPausedMusic() const;

        bool isStoppedMusic() const;

        /** \brief sets the volume of the music.
          * \param volume the percentage of volume. between 0 and 1 */
        void setVolumeMusic(float volume);

        void fadeOutMusic();

        void detachChannel(int channel);

        virtual ~AudioManager();
};

#endif // AUDIOMANAGER_H
