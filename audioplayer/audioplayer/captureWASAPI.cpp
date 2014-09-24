#include "stdafx.h"
#include "captureWASAPI.h"
#include <strsafe.h>
//
//  WAV file writer.
//
//  This is a VERY simple .WAV file writer.
//

//
//  A wave file consists of:
//
//  RIFF header:    8 bytes consisting of the signature "RIFF" followed by a 4 byte file length.
//  WAVE header:    4 bytes consisting of the signature "WAVE".
//  fmt header:     4 bytes consisting of the signature "fmt " followed by a WAVEFORMATEX 
//  WAVEFORMAT:     <n> bytes containing a waveformat structure.
//  DATA header:    8 bytes consisting of the signature "data" followed by a 4 byte file length.
//  wave data:      <m> bytes containing wave data.
//
//
//  Header for a WAV file - we define a structure describing the first few fields in the header for convenience.
//
struct WAVEHEADER
{
    DWORD   dwRiff;                     // "RIFF"
    DWORD   dwSize;                     // Size
    DWORD   dwWave;                     // "WAVE"
    DWORD   dwFmt;                      // "fmt "
    DWORD   dwFmtSize;                  // Wave Format Size
};

//  Static RIFF header, we'll append the format to it.
const BYTE WaveHeader[] = 
{
    'R',   'I',   'F',   'F',  0x00,  0x00,  0x00,  0x00, 'W',   'A',   'V',   'E',   'f',   'm',   't',   ' ', 0x00, 0x00, 0x00, 0x00
};

//  Static wave DATA tag.
const BYTE WaveData[] = { 'd', 'a', 't', 'a'};

//
//  Write the contents of a WAV file.  We take as input the data to write and the format of that data.
//
bool WriteWaveFile(HANDLE FileHandle, const BYTE *Buffer, const size_t BufferSize, const WAVEFORMATEX *WaveFormat)
{
    DWORD waveFileSize = sizeof(WAVEHEADER) + sizeof(WAVEFORMATEX) + WaveFormat->cbSize + sizeof(WaveData) + sizeof(DWORD) + static_cast<DWORD>(BufferSize);
    BYTE *waveFileData = new (std::nothrow) BYTE[waveFileSize];
    BYTE *waveFilePointer = waveFileData;
    WAVEHEADER *waveHeader = reinterpret_cast<WAVEHEADER *>(waveFileData);

    if (waveFileData == NULL)
    {
        printf("Unable to allocate %d bytes to hold output wave data\n", waveFileSize);
        return false;
    }

    //
    //  Copy in the wave header - we'll fix up the lengths later.
    //
    CopyMemory(waveFilePointer, WaveHeader, sizeof(WaveHeader));
    waveFilePointer += sizeof(WaveHeader);

    //
    //  Update the sizes in the header.
    //
    waveHeader->dwSize = waveFileSize - (2 * sizeof(DWORD));
    waveHeader->dwFmtSize = sizeof(WAVEFORMATEX) + WaveFormat->cbSize;

    //
    //  Next copy in the WaveFormatex structure.
    //
    CopyMemory(waveFilePointer, WaveFormat, sizeof(WAVEFORMATEX) + WaveFormat->cbSize);
    waveFilePointer += sizeof(WAVEFORMATEX) + WaveFormat->cbSize;


    //
    //  Then the data header.
    //
    CopyMemory(waveFilePointer, WaveData, sizeof(WaveData));
    waveFilePointer += sizeof(WaveData);
    *(reinterpret_cast<DWORD *>(waveFilePointer)) = static_cast<DWORD>(BufferSize);
    waveFilePointer += sizeof(DWORD);

    //
    //  And finally copy in the audio data.
    //
    CopyMemory(waveFilePointer, Buffer, BufferSize);

    //
    //  Last but not least, write the data to the file.
    //
    DWORD bytesWritten;
    if (!WriteFile(FileHandle, waveFileData, waveFileSize, &bytesWritten, NULL))
    {
        printf("Unable to write wave file: %d\n", GetLastError());
        delete []waveFileData;
        return false;
    }

    if (bytesWritten != waveFileSize)
    {
        printf("Failed to write entire wave file\n");
        delete []waveFileData;
        return false;
    }
    delete []waveFileData;
    return true;
}

//
//  Write the captured wave data to an output file so that it can be examined later.
//
void SaveWaveData(BYTE *CaptureBuffer, size_t BufferSize, const WAVEFORMATEX *WaveFormat)
{
    wchar_t waveFileName[MAX_PATH];
    HRESULT hr = StringCbCopy(waveFileName, sizeof(waveFileName), L"../../wavsource/WASAPICapture-");
    if (SUCCEEDED(hr))
    {
        GUID testGuid;
        if (SUCCEEDED(CoCreateGuid(&testGuid)))
        {
            wchar_t *guidString;
            if (SUCCEEDED(StringFromCLSID(testGuid, &guidString)))
            {
                hr = StringCbCat(waveFileName, sizeof(waveFileName), guidString);
                if (SUCCEEDED(hr))
                {
                    hr = StringCbCat(waveFileName, sizeof(waveFileName), L".WAV");
                    if (SUCCEEDED(hr))
                    {
                        HANDLE waveHandle = CreateFile(waveFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 
                            NULL);
                        if (waveHandle != INVALID_HANDLE_VALUE)
                        {
                            if (WriteWaveFile(waveHandle, CaptureBuffer, BufferSize, WaveFormat))
                            {
                                printf("Successfully wrote WAVE data to %S\n", waveFileName);
                            }
                            else
                            {
                                printf("Unable to write wave file\n");
                            }

                            CloseHandle(waveHandle);
                        }
                        else
                        {
                            printf("Unable to open output WAV file %S: %d\n", waveFileName, GetLastError());
                        }
                    }
                }
                CoTaskMemFree(guidString);
            }
        }
    }
}




BOOL bDone = FALSE;
DWORD  WINAPI CoreAudioCapture(LPVOID pM)
{
    HRESULT hr;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    REFERENCE_TIME hnsActualDuration;
    UINT32 bufferFrameCount;
    UINT32 numFramesAvailable;
    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDevice *pDevice = NULL;
    IAudioClient *pAudioClient = NULL;
    IAudioCaptureClient *pCaptureClient = NULL;
    WAVEFORMATEX *pwfx = NULL;
    UINT32 packetLength = 0;
    BYTE *pData;
    DWORD flags = 0;
    FILE *Precord = NULL;
    string ftemp = "F:\\github\\audioplayer\\wavsource\\temp.pcm";
    Precord = fopen(ftemp.c_str(),"wb");
    bDone = FALSE;
    CoInitialize(NULL);  //to tell system creat COM by single thread!!!!
    hr = CoCreateInstance(
             CLSID_MMDeviceEnumerator, NULL,
             CLSCTX_ALL, IID_IMMDeviceEnumerator,
             (void **)&pEnumerator);

    hr = pEnumerator->GetDefaultAudioEndpoint(
             eCapture, eConsole, &pDevice);

    hr = pDevice->Activate(
             IID_IAudioClient, CLSCTX_ALL,
             NULL, (void **)&pAudioClient);

    hr = pAudioClient->GetMixFormat(&pwfx);

    hr = pAudioClient->Initialize(
             AUDCLNT_SHAREMODE_SHARED,
             0,
             hnsRequestedDuration,
             0,
             pwfx,
             NULL);

    // Get the size of the allocated buffer.
    hr = pAudioClient->GetBufferSize(&bufferFrameCount);

    hr = pAudioClient->GetService(
             IID_IAudioCaptureClient,
             (void **)&pCaptureClient);

    // Notify the audio sink which format to use.
    //hr = pMySink->SetFormat(pwfx);

    // Calculate the actual duration of the allocated buffer.
    hnsActualDuration = (double)REFTIMES_PER_SEC *
                        bufferFrameCount / pwfx->nSamplesPerSec;
        
    hr = pAudioClient->Start();  // Start recording.

    // Each loop fills about half of the shared buffer.
    while (bDone == FALSE)
    {
        // Sleep for half the buffer duration.
        Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);

        hr = pCaptureClient->GetNextPacketSize(&packetLength);

        while (packetLength != 0)
        {
            // Get the available data in the shared buffer.
            hr = pCaptureClient->GetBuffer(
                     &pData,
                     &numFramesAvailable,
                     &flags, NULL, NULL);

            if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
            {
                pData = NULL;  // Tell CopyData to write silence.
            }
            fwrite(pData,sizeof(BYTE),numFramesAvailable*8,Precord);
            // Copy the available capture data to the audio sink.
            //            hr = pMySink->CopyData(
            //                              pData, numFramesAvailable, &bDone);

            hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);

            hr = pCaptureClient->GetNextPacketSize(&packetLength);
        }
    }

    hr = pAudioClient->Stop();  // Stop recording.

Exit:
    SaveWaveData(pData,0,pwfx);
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pDevice)
    SAFE_RELEASE(pAudioClient)
    SAFE_RELEASE(pCaptureClient)
    fclose(Precord);
    return 0;
}