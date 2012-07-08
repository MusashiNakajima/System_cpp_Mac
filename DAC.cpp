//
//  DAC.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "DAC.h"

int DAC::outputCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData)
{
    DAC *dac = (DAC *)userData;
    short *out = (short *)outputBuffer;
    unsigned int i;
    (void) timeInfo; // Prevent unused variable warnings. 
    (void) statusFlags;
    (void) userData;
    
    int d = dac->wp - dac->rp;
    if (d < 0)  d = d + MAX_PACKET;
    
    if (dac->loop++==1000) {
        printf("%d\n",d);
        dac->loop = 0;
    }
    
    for(i = 0; i< framesPerBuffer; i++){
        
        if (dac->isPlaying) {
            if (d-- > 0) {
                dac->vol = 0.005*dac->vTable[dac->vVal] + 0.995*dac->vol;
                *out++ = dac->buf[dac->rp]*dac->vol;
                dac->buf[dac->rp] = 0;
                dac->rp = (dac->rp != MAX_PACKET-1 ? dac->rp+1 : 0);
            } else {
                dac->isPlaying = false;
                printf("stop\n");
                *out++ = 0;


            }
            
        } else {
            *out++ = 0;
        }
    }

	return paContinue;
}


int DAC::stream(const char   *path, 
                const char   *types, 
                lo_arg       **argv, 
                int          argc,
                void         *data, 
                void         *user_data)
{
    DAC *dac = (DAC *)user_data;
    
    lo_blob b = (lo_blob)argv[0];
    short *dp = (short *)lo_blob_dataptr(b);
    int size = lo_blob_datasize(b)/sizeof(short);
    int d = dac->wp - dac->rp;
    if (d < 0)  d = d + MAX_PACKET;
    
    dac->sendAudio(dp, lo_blob_datasize(b));
    
    if (dac->isPlaying) {
        for(int i=0; i<size; i++){
            
            if (d++ <= dac->bs+size) {
                dac->buf[dac->wp] = *dp++;
                dac->wp = (dac->wp != MAX_PACKET-1 ? dac->wp+1 : 0);
            } else {
                printf("//\n");
                break;
            }
        }
        
    } else {
        
        for(int i=0; i<size; i++){
            dac->buf[dac->wp] = *dp++;
            dac->wp = (dac->wp != MAX_PACKET-1 ? dac->wp+1 : 0);
        }
        if (d+size > dac->bs && !dac->isPlaying) {
            dac->start();
            printf("start\n");
        }
    }
    return 0;
}

int DAC::data1(const char   *path, 
               const char   *types, 
               lo_arg       **argv, 
               int          argc,
               void         *data, 
               void         *user_data)
{
    DAC *dac = (DAC *)user_data;
    dac->vVal = argv[0]->i;
    return 0;
}

DAC::DAC(lo_server_thread s, const char *osc) : Module(s, osc)
{
    addMethodToServer("/Stream", "b", DAC::stream, this);
    addMethodToServer("/Data", "ii", DAC::data1, this);
    sendSetMdtkn();
    sendSetMdtkn();

    for (int i=0; i<128; i++) {
        vTable[i] = 1.0 - logf((float)(127-i))/logf(127.0);
    }
    
    vTable[127] = 1.0;
    vVal        = 127;
    vol         = 1.0;
    isPlaying   = false;
    sampleRate  = 44100.0;
    rp          = 0;
    wp          = 0;
    bs          = 128*8;
    buf         = (short *)malloc(sizeof(short)*MAX_PACKET);
    memset(buf, 0, sizeof(short)*MAX_PACKET);
    if(preparePa()) printf("err_preparePa\n");
}

int DAC::preparePa()
{
    err = Pa_Initialize();
    if( err != paNoError ) {
        Pa_Terminate();
        return 1;
    }
    
    outputParameters.device = Pa_GetDefaultOutputDevice(); // デフォルトアウトプットデバイス
    if (outputParameters.device == paNoDevice) {
        printf("Error: No default output device.\n");
        Pa_Terminate();
        return 1;
    }
    
    outputParameters.channelCount = 1;       // モノラルアウトプット
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    err = Pa_OpenStream(&paStream,
                        NULL,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        0, // paClipOff,    we won't output out of range samples so don't bother clipping them 
                        outputCallback,
                        this);
    
    if( err != paNoError ) {
        Pa_Terminate();
        printf("err_open\n");
    }
    start();
    return 0;
}

int DAC::start()
{
    err = Pa_StartStream( paStream );
    isPlaying = true;
    return 0;
}

int DAC::stop()
{
    err = Pa_StopStream( paStream );
    isPlaying = false;
    return 0;
}

DAC::~DAC()
{
	free(buf);
    stop();
    Pa_CloseStream( paStream );
    Pa_Terminate();
}