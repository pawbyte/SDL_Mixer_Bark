/*
sdl_mixer_bark.hpp
This file is part of:
SDL_Mixer_Bark
https://www.pawbyte.com/sdl_mixer_bark
Copyright (c) 2023 Nathan Hurde, Chase Lee.

Copyright (c) 2023 PawByte.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the �Software�), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-sdl_mixer_bark <https://www.pawbyte.com/sdl_mixer_bark>


*/

#define MIX_BARK_VERSION_MAJOR 1
#define MIX_BARK_VERSION_MINOR 0
#define MIX_BARK_VERSION_PATCH 0

#define MIX_CHANNELS 8 //Number of channels used
#define PLAY_FREQUENCY 44100 //Frequency that SDL_mixer will work

#define MIX_BARK_RENDER_MAX_RECTS 999999 //Used to avoid heap crashing when rendering points as a list of rectanges
#define MIX_RENDER_CLIPPED_CHUNKS 32 //Defaulting to 32, but you can change this to your heart content to whole number of your choice. The higher the number, the more memory is required.




#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "stdbool.h"

const float MIX_Audio_Amplitude  = 32768.0f;
const int MIX_Bark_Default_Chunk_Size  = 4096;

const int Mix_Bark_Graph_Align_Top = 0;
const int Mix_Bark_Graph_Align_Center = 1;
const int Mix_Bark_Graph_Align_Bottom = 2;

extern SDL_FRect Mix_Bark_PreviewRects[MIX_BARK_RENDER_MAX_RECTS];
extern Mix_Chunk * Mix_ShortenedChunks[MIX_RENDER_CLIPPED_CHUNKS];
extern int Mix_Bark_ShortenedChunkPos;
extern bool Mix_Bark_Was_Innitted;

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

//#endif // sdl_mixer_bark_h
