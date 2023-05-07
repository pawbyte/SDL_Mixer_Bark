/*
sdl_mixer_bark_base.cpp
This file is part of:
SDL_Mixer_Bark
https://www.pawbyte.com/sdl_mixer_bark
Copyright (c) 2023 Nathan Hurde, Chase Lee.

Copyright (c) 2023 PawByte.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-sdl_mixer_bark <https://www.pawbyte.com/sdl_mixer_bark>


*/

#include "sdl_mixer_bark.hpp"
#include <math.h>
#include <string.h>
#include <stdio.h>

SDL_FRect Mix_Bark_PreviewRects[MIX_BARK_RENDER_MAX_RECTS];
Mix_Chunk * Mix_ShortenedChunks[MIX_RENDER_CLIPPED_CHUNKS];
int Mix_Bark_ShortenedChunkPos = 0;
bool Mix_Bark_Was_Innitted = false;

bool Mix_Bark_Initted()
{
    return Mix_Bark_Was_Innitted;
}

bool Mix_ClearShorts()
{
    if( !Mix_Bark_Was_Innitted )
    {
        return false;
    }
    for ( int i_chunk = 0; i_chunk < MIX_RENDER_CLIPPED_CHUNKS ; i_chunk+=1)
    {
        if( Mix_ShortenedChunks[i_chunk] != NULL )
        {
            Mix_FreeChunk( Mix_ShortenedChunks[i_chunk] );
            Mix_ShortenedChunks[i_chunk] = NULL;
        }
    }
    return true;
}

Uint16 Mix_GetFormatSampleSize(Uint16 format)
{

    return (format & 0xFF) / 8;
}

int Mix_Getaudio_frequency()
{
    Uint16 audio_format;  // current audio format constant
    int audio_frequency,  // frequency rate of the current audio format
    audio_channel_count;  // number of channels of the current audio format

    /* bytes / samplesize == sample points */
    Mix_QuerySpec( &audio_frequency, &audio_format, &audio_channel_count );
    return audio_frequency;
}

float Mix_GetChunk_Duration_MS( Mix_Chunk * chunk)
{
    if( chunk == NULL )
    {
        return 0;
    }

    Uint16 audio_format;  // current audio format constant
    int audio_frequency,  // frequency rate of the current audio format
    audio_channel_count;  // number of channels of the current audio format

    /* bytes / samplesize == sample points */
    Mix_QuerySpec( &audio_frequency, &audio_format, &audio_channel_count );

    const Uint32 points = chunk->alen / Mix_GetFormatSampleSize(audio_format);

    /* sample points / channels == sample frames */
    const Uint32 frames = (points / audio_channel_count);

    /* (sample frames * 1000) / frequency == play length, in ms */
    float sound_length_ms_total =  ((frames * 1000) / audio_frequency);
    //computer_lengths();
    return sound_length_ms_total;
}

int Mix_GetChunkSampleCount(Mix_Chunk * chunk)
{
    if( chunk == NULL)
    {
        return -1;
    }
    return chunk->alen / sizeof(Uint16);
}

float Mix_GetChunkVolumeAt(Mix_Chunk * chunk, int position)
{
    return ( (((float)((Sint16*)chunk->abuf)[position] / MIX_Audio_Amplitude) + 1.f )/2.0f );
}

float Mix_GetChunkVolumeAt_Safe(Mix_Chunk * chunk, int position)
{
    if( chunk == NULL)
    {
        return -1;;
    }
    if( position >=0 && position < Mix_GetChunkSampleCount(chunk) )
    {
        return ( (((float)((Sint16*)chunk->abuf)[position] / MIX_Audio_Amplitude) + 1.f )/2.0f );
    }
    return -1;
}


int Mix_InitBark(int flags, int chunk_size )
{
    if( Mix_Bark_Was_Innitted == false)
    {
        for ( int i_chunk = 0; i_chunk < MIX_RENDER_CLIPPED_CHUNKS ; i_chunk+=1)
        {
            Mix_ShortenedChunks[i_chunk] = NULL;
        }
    }

    Mix_Init( flags );

    if( chunk_size < 0 )
    {
        chunk_size = MIX_Bark_Default_Chunk_Size;
    }

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, chunk_size);

    Mix_Bark_Was_Innitted = true;
    Mix_Bark_ShortenedChunkPos = 0;

}


int Mix_PlayChunkClipped( Mix_Chunk * chunk, float position, float duration, int preferred_channel, bool halt_channel)
{
    if( chunk == NULL )
    {
        return -3;
    }

    if( preferred_channel !=-1 && halt_channel )
    {
        Mix_HaltChannel(preferred_channel);
    }

    float chuck_time_ms =  Mix_GetChunk_Duration_MS( chunk );

    if( position > 0 && position < chuck_time_ms )
    {
        int number_of_samples = chunk->alen / sizeof(Uint16);

        if(number_of_samples == 0.f)
        {
            return -2;
        }
        Mix_Bark_ShortenedChunkPos+=1;
        if(Mix_Bark_ShortenedChunkPos >= MIX_RENDER_CLIPPED_CHUNKS )
        {
            Mix_Bark_ShortenedChunkPos = 0;
        }
        if( Mix_ShortenedChunks[Mix_Bark_ShortenedChunkPos] != NULL )
        {
            Mix_FreeChunk( Mix_ShortenedChunks[Mix_Bark_ShortenedChunkPos] );
            Mix_ShortenedChunks[Mix_Bark_ShortenedChunkPos] = NULL;
        }

        Mix_ShortenedChunks[Mix_Bark_ShortenedChunkPos] = Mix_SplitChunk( chunk, position, duration );
        Mix_ShortenedChunks[Mix_Bark_ShortenedChunkPos]->volume = chunk->volume;

        int channel = 0;
        channel = Mix_PlayChannel( preferred_channel, Mix_ShortenedChunks[Mix_Bark_ShortenedChunkPos], 0);
        //channel = Mix_PlayChannelTimed( preferred_channel, Mix_ShortenedChunks[Mix_Bark_ShortenedChunkPos], 0, (int)duration );
        return channel;
    }
    return -2;
}

int Mix_PlayChunkAt( Mix_Chunk * chunk, float position, int preferred_channel, bool halt_channel )
{
    return Mix_PlayChunkClipped(chunk, position, -1, preferred_channel, halt_channel);
}

bool Mix_RenderAudioPreview( SDL_Renderer *renderer, Mix_Chunk * chunk, float audio_graph_x, float audio_graph_y, float audio_graph_width, float audio_graph_height, short alignment )
{
    if( chunk == nullptr || chunk == NULL)
    {
        return false;
    }
    float number_of_samples = chunk->alen / sizeof(Uint16);
    if(number_of_samples == 0.f)
    {
        return false;
    }

    float x, y, x2, y2;

    float i_inc = 1;
    if( number_of_samples >= audio_graph_width )
    {
        i_inc =  number_of_samples / audio_graph_width;
    }
    else
    {
        i_inc = audio_graph_width / number_of_samples;
    }
    int point_count = floor(i_inc);
    if( point_count > MIX_BARK_RENDER_MAX_RECTS)
    {
        point_count = MIX_BARK_RENDER_MAX_RECTS;
        i_inc = number_of_samples / ( (float)MIX_BARK_RENDER_MAX_RECTS );
    }
    if(  point_count == 0 )
    {
        return false;
    }

    int point_pos = 0;
    for ( point_pos = 0; point_pos <= point_count; point_pos++)
    {
        Mix_Bark_PreviewRects[(int)point_pos].x = 0;
        Mix_Bark_PreviewRects[(int)point_pos].y = 0;
        Mix_Bark_PreviewRects[(int)point_pos].w = 1;
        Mix_Bark_PreviewRects[(int)point_pos].h = audio_graph_height;
    }
    point_pos = 0;

    float point_width = 1;
    if(  point_count <  audio_graph_width  )
    {
        point_width = audio_graph_width / (float)point_count;
    }
    float sample_volume_size = 0;

    for (float i = 0; i < number_of_samples; i += i_inc )
    {
        sample_volume_size = Mix_GetChunkVolumeAt(chunk, floor(i) )* audio_graph_height;
        if ( sample_volume_size  > audio_graph_height )
        {
            sample_volume_size = audio_graph_height;
        }
        // Calculate the x-coordinate of the current sample in the graph
        Mix_Bark_PreviewRects[point_pos].x = audio_graph_x +(float)point_pos*point_width;

        // Calculate the y-coordinate of the current sample in the graph
        Mix_Bark_PreviewRects[point_pos].w = point_width;
        if ( alignment == Mix_Bark_Graph_Align_Center )
        {
            Mix_Bark_PreviewRects[point_pos].y = audio_graph_y + (audio_graph_height - sample_volume_size)/2.f;
        }
        else if(alignment == Mix_Bark_Graph_Align_Bottom )
        {
            Mix_Bark_PreviewRects[point_pos].y = audio_graph_y + audio_graph_height - sample_volume_size;
        }
        else
        {
            //Mix_Bark_Graph_Align_Top
            Mix_Bark_PreviewRects[point_pos].y = audio_graph_y;
        }
        Mix_Bark_PreviewRects[point_pos].h = sample_volume_size;

        point_pos++;
    }
    SDL_RenderFillRectsF(renderer, Mix_Bark_PreviewRects, point_count-1 );
    return true;
}

//Pending, hopefully doable
bool Mix_SaveChunkFlac( Mix_Chunk * chunk, char * export_file_name )
{
    return false;
}

//Pending, hopefully doable
bool Mix_SaveChunkMP3( Mix_Chunk * chunk, char * export_file_name )
{
    return false;
}

//Pending, hopefully doable
bool Mix_SaveChunkOgg( Mix_Chunk * chunk, char * export_file_name )
{
    return false;
}

bool Mix_SaveChunkWav( Mix_Chunk * chunk, char * export_file_name )
{
    if( chunk == NULL )
    {
        return false;
    }

    Uint16 audio_format;  // current audio format constant
    int audio_frequency,  // frequency rate of the current audio format
    audio_channel_count;  // number of channels of the current audio format

    Mix_QuerySpec( &audio_frequency, &audio_format, &audio_channel_count );

    // Write the WAV file header
    FILE* file = fopen(export_file_name, "wb");
    if (file == NULL)
    {
        printf("ERROR:  Failed to open wav file.\n");
        return 0;
    }
    fwrite("RIFF", 1, 4, file);
    Uint32 chunk_size = chunk->alen + 36;
    fwrite(&chunk_size, 1, 4, file);
    fwrite("WAVE", 1, 4, file);
    fwrite("fmt ", 1, 4, file);
    Uint32 sub_chun_k1_size = 16;
    fwrite(&sub_chun_k1_size, 1, 4, file);
    audio_format = 1; //Setting to 1 solves the corruption issue
    fwrite(&audio_format, 1, 2, file);
    Uint16 num_channels = audio_channel_count;
    fwrite(&num_channels, 1, 2, file);
    Uint32 sample_rate = audio_frequency;
    fwrite(&sample_rate, 1, 4, file);
    Uint32 byte_rate = 176400;
    fwrite(&byte_rate, 1, 4, file);
    Uint16 block_align = 4;
    fwrite(&block_align, 1, 2, file);
    Uint16 bits_per_sample = 16;
    fwrite(&bits_per_sample, 1, 2, file);
    fwrite("data", 1, 4, file);
    fwrite(&chunk->alen, 1, 4, file);
    fwrite(chunk->abuf, 1, chunk->alen, file);
    fclose(file);
    return 0;
}

Mix_Chunk * Mix_SplitChunk( Mix_Chunk * chunk, float position, float duration   )
{
    if( chunk == NULL )
    {
        return NULL;
    }
    float chuck_time_ms = Mix_GetChunk_Duration_MS( chunk );
    if( position > 0 && position < chuck_time_ms )
    {
        // Set the start and end positions of the chunk to copy
        Uint16 audio_format;  // current audio format constant
        int audio_frequency,  // frequency rate of the current audio format
        audio_channel_count;  // number of channels of the current audio format

        /* bytes / samplesize == sample points */
        Mix_QuerySpec( &audio_frequency, &audio_format, &audio_channel_count );

        // Set the start position in seconds
        float start_time = floor(position / 1000.f); // in seconds
        Uint32 start_pos = (Uint32)(start_time * (float)audio_frequency * (float)audio_channel_count * (audio_format & 0xFF) / 8.0f);


        // Calculate the end position in bytes
        Uint32 end_pos = 0;
        if( duration < 0.f || ( position+duration >= chuck_time_ms ) )
        {
            end_pos = chunk->alen;
        }
        else
        {
            //TODO: Implement clipping to the right, currently experiencing static bug...
            float end_sec = start_time + floor(duration / 1000.f);
            end_pos = (Uint32)(end_sec * (float)audio_frequency * (float)audio_channel_count * (audio_format & 0xFF) / 8.0f);
            //end_pos+= start_time;
        }
        // Calculate the size of the buffer to hold the copied data
        Uint32 buffer_size = end_pos - start_pos;

        // Allocate memory for the new chunk data
        Uint8* new_chunk_data = (Uint8*)malloc(buffer_size);

        // Copy the desired portion of the chunk to the new data buffer
        memcpy(new_chunk_data, chunk->abuf + start_pos, buffer_size);

        // Create a new chunk from the copied data
        Mix_Chunk* new_chunk = Mix_QuickLoad_RAW(new_chunk_data, buffer_size);
        return new_chunk;
    }
    return NULL;
}

bool Mix_QuitBark()
{
    if(!Mix_Bark_Was_Innitted )
    {
        return false;
    }

    Mix_ClearShorts();
    Mix_Bark_Was_Innitted = false;
    Mix_Quit();
    return true;
}
