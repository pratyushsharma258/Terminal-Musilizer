#include <stdlib.h>
#include <iostream>
#include <portaudio.h>

using namespace std;

static void checkError(PaError err)
{
    if (err != paNoError)
    {
        cout << "Error: " << Pa_GetErrorText(err) << endl;
        exit(EXIT_FAILURE);
    }
}

int main()
{
    PaError err;
    err = Pa_Initialize();
    checkError(err);

    int numDevices = Pa_GetDeviceCount();
    printf("Number of devices: %d\n", numDevices);

    if (numDevices < 0)
    {
        printf("Error getting device count.\n");
        exit(EXIT_FAILURE);
    }
    else if (numDevices == 0)
    {
        printf("There are no available audio devices on this machine.\n");
        exit(EXIT_SUCCESS);
    }

    const PaDeviceInfo *deviceInfo;
    for (int i = 0; i < numDevices; i++)
    {
        deviceInfo = Pa_GetDeviceInfo(i);
        printf("Device %d:\n", i);
        printf("  name: %s\n", deviceInfo->name);
        printf("  maxInputChannels: %d\n", deviceInfo->maxInputChannels);
        printf("  maxOutputChannels: %d\n", deviceInfo->maxOutputChannels);
        printf("  defaultSampleRate: %f\n", deviceInfo->defaultSampleRate);
    }

    int device = 0;

    PaStream *stream;
    PaStreamParameters inputParameters, outputParameters;
    inputParameters.device = device;
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = paInt16;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = device;
    outputParameters.channelCount = 1;
    outputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&stream, &inputParameters, &outputParameters, 44100, 256, paClipOff, NULL, NULL);
    checkError(err);

    err = Pa_StartStream(stream);
    checkError(err);

    printf("Press Enter to stop the stream.\n");
    getchar();

    err = Pa_StopStream(stream);
    checkError(err);

    err = Pa_CloseStream(stream);
    checkError(err);

    Pa_Terminate();

    return EXIT_SUCCESS;
}