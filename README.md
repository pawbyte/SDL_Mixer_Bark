# SDL_Mixer_Bark

![](https://github.com/pawbyte/SDL_Mixer_Bark/blob/main/sdl_mixer_bark_logo_transparent.png)
# A little collection of features to enhance the SDL_Mixer experience

## Simply include SDL_Mixer.h and SDL.h plus SDL_Mixer_Bark.hpp and enjoy these addons:

bool Mix_Bark_Initted(); // check if mix_bark is innited, seperate from checking if sdl_mixer itself is iniited

bool Mix_ClearShorts();

int Mix_GetAudioFrequency();

float Mix_GetChunk_Duration_MS( Mix_Chunk * chunk);

int Mix_GetChunkSampleCount(Mix_Chunk * chunk);

float Mix_GetChunkVolumeAt(Mix_Chunk * chunk, int position);

float Mix_GetChunkVolumeAt_Safe(Mix_Chunk * chunk, int position);

Uint16 Mix_GetFormatSampleSize(Uint16 format);

//Inits SDL_Mixer with the given flags as well as SDL_Mixer_Bark with the given chunk_size

int Mix_InitBark(int flags, int chunk_size );

int Mix_PlayChunkClipped( Mix_Chunk * chunk, float position, float duration, int preferred_channel, bool halt_channel);

int Mix_PlayChunkAt( Mix_Chunk * chunk, float position, int preferred_channel, bool halt_channel);

bool Mix_RenderAudioPreview( SDL_Renderer *renderer, Mix_Chunk * chunk, float audio_graph_x, float audio_graph_y, float audio_graph_width, float audio_graph_height, short alignment );

//Pending, hopefully doable

bool Mix_SaveChunkFlac( Mix_Chunk * chunk, char * export_file_name );

//Pending, hopefully doable

bool Mix_SaveChunkMP3( Mix_Chunk * chunk, char * export_file_name );

//Pending, hopefully doable

bool Mix_SaveChunkOgg( Mix_Chunk * chunk, char * export_file_name );

bool Mix_SaveChunkWav( Mix_Chunk * chunk, char * export_file_name );

Mix_Chunk * Mix_SplitChunk( Mix_Chunk * chunk, float position, float duration  );

bool Mix_QuitBark(); // Quits Mix_Bark and SDL_Mixer

Donate to make SDL_Mixer_Bark greater
=========================================
Work on this library is not possible without funding. The amount of time it takes to update the codebases, create examples, tutorials and more is quite expensive. Please consider either a one-time or monthly donation to keep this engine going and more frequently updated. 

We have a [Patreon Page](https://www.patreon.com/pawbyte?ty=h) that you can help fund future developments of Pencil Engine and to help us kickstart new projects!

There is also a [PayPal](http://www.pawbyte.com/donate/) one time donation method as well. 

I appreciate your generousity and moving this project forward. 

