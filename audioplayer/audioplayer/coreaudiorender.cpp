#include "stdafx.h"
#include "Avrt.h"
#include "coreaudiorender.h"
#pragma comment(lib,"Avrt.lib")
volatile DWORD flags = 0;
DWORD  WINAPI CoreAudioRender(LPVOID pM)
{
    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDevice *pDevice = NULL;
    IAudioClient *pAudioClient = NULL;
    IAudioRenderClient *pRenderClient = NULL;
    WAVEFORMATEX *pwfx = NULL;
    HRESULT hr;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC * 1;
    REFERENCE_TIME hnsActualDuration;
    HANDLE hTask = NULL;
    HANDLE hEvent = NULL;
    UINT32 bufferFrameCount;
    UINT32 numFramesAvailable;
    UINT32 numFramesPadding;
    UINT32 readnum = 0;
    BYTE *pData;
    flags = 0x00;
    CoInitialize(NULL);  //to tell system creat COM by single thread!!!!
    hr = CoCreateInstance(
             CLSID_MMDeviceEnumerator, NULL,
             CLSCTX_ALL, IID_IMMDeviceEnumerator,
             (void **)&pEnumerator);

    hr = pEnumerator->GetDefaultAudioEndpoint(
             eRender, eConsole, &pDevice);


    // LPWSTR pstr;
    // hr = pDevice->GetId(&pstr);
    renderStatus = 1;
    hr = pDevice->Activate(
             IID_IAudioClient, CLSCTX_ALL,
             NULL, (void **)&pAudioClient);

    hr = pAudioClient->GetMixFormat(&pwfx);


    // hr = pAudioClient->Initialize(
    //          AUDCLNT_SHAREMODE_SHARED,
    //          0,
    //          hnsRequestedDuration,
    //          0,
    //          pwfx,
    //          NULL);
      hr = pAudioClient->Initialize(
                         AUDCLNT_SHAREMODE_SHARED,
                         AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
                         hnsRequestedDuration,
                         hnsRequestedDuration,
                         pwfx,
                         NULL);


    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hEvent == NULL)
    {
        hr = E_FAIL;
    }

    hr = pAudioClient->SetEventHandle(hEvent);






    hr = pAudioClient->GetBufferSize(&bufferFrameCount);

    hr = pAudioClient->GetService(
             IID_IAudioRenderClient,
             (void **)&pRenderClient);

    // Grab the entire buffer for the initial fill operation.
    hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);

    fldta.seek = (int)( (fldta.scdu / 100.0) * fldta.datelen);
    fseek(fldta.frp, DATA + fldta.seek, SEEK_SET);

    fread(pData, sizeof(BYTE), bufferFrameCount * 8, fldta.frp);


    hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
    fldta.seek += bufferFrameCount * 8 ;
    fldta.scdu = (int)( (fldta.seek  / fldta.datelen) * 100);
    schedule.SetPos( fldta.scdu);
    // Calculate the actual duration of the allocated buffer.
    hnsActualDuration = (double)REFTIMES_PER_SEC *
                        bufferFrameCount / pwfx->nSamplesPerSec;


    DWORD taskIndex = 0;
    LPCWSTR lpStr;
    CString str1 = L"pro Audio";
    lpStr = str1;
    hTask = AvSetMmThreadCharacteristics( lpStr, &taskIndex);


    hr = pAudioClient->Start();  // Start playing.

    CMMNotificationClient *pNotify = NULL ;
    pNotify = new(CMMNotificationClient);
    hr = pEnumerator->RegisterEndpointNotificationCallback (pNotify);
    // Each loop fills about half of the shared buffer.
    while (flags != AUDCLNT_BUFFERFLAGS_SILENT)
    {
        if (pNotify->DefauleChanege == 1)  // if defaule device change
        {
            hr = CoCreateInstance(
                     CLSID_MMDeviceEnumerator, NULL,
                     CLSCTX_ALL, IID_IMMDeviceEnumerator,
                     (void **)&pEnumerator);

            hr = pEnumerator->GetDefaultAudioEndpoint(
                     eRender, eConsole, &pDevice);
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
            hr = pAudioClient->GetBufferSize(&bufferFrameCount);

            hr = pAudioClient->GetService(
                     IID_IAudioRenderClient,
                     (void **)&pRenderClient);
            hr = pAudioClient->Start();  // Start playing.
            pNotify->DefauleChanege = 0 ;
        }
        else
        {
            // Sleep for half the buffer duration.
            // Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));
            //
            //
            DWORD retval = WaitForSingleObject(hEvent, INFINITE);
            // if (retval != WAIT_OBJECT_0)
            // {
            //     // Event handle timed out after a 2-second wait.
            //     pAudioClient->Stop();
            //     hr = ERROR_TIMEOUT;
            // }

            //
            //  fseek(fldta.frp, DATA + fldta.seek, SEEK_SET);
            // See how much buffer space is available.
            hr = pAudioClient->GetCurrentPadding(&numFramesPadding);

            numFramesAvailable = bufferFrameCount - numFramesPadding;

            // Grab all the available space in the shared buffer.
            hr = pRenderClient->GetBuffer(numFramesAvailable, &pData);

            // hr = pMySource->LoadData(numFramesAvailable, pData, &flags);
            //GetWaveData( pData ,fldta.frp,numFramesAvailable ,bitperSample);


            // readnum = fread(pData, sizeof(BYTE), numFramesAvailable * 8, fldta.frp);
            readnum = fread(pData, sizeof(BYTE), numFramesAvailable * 8, fldta.frp);
            hr = pRenderClient->ReleaseBuffer(numFramesAvailable, flags);
            fldta.seek += numFramesAvailable * 8 ;
            // fseek(fldta.frp, DATA + fldta.seek, SEEK_SET);
            fldta.scdu = (int)((fldta.seek / fldta.datelen) * 100);
            schedule.SetPos( fldta.scdu);
            if (readnum == 0)
                flags |= AUDCLNT_BUFFERFLAGS_SILENT;

        }
    }

    // Wait for last data in buffer to play before stopping.
    Sleep((DWORD)(hnsActualDuration / REFTIMES_PER_MILLISEC / 2));

    hr = pAudioClient->Stop();  // Stop playing.
    pEnumerator->UnregisterEndpointNotificationCallback(pNotify);
    delete(pNotify);
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pDevice)
    SAFE_RELEASE(pAudioClient)
    SAFE_RELEASE(pRenderClient)
    CoUninitialize();
    if (hEvent != NULL)
    {
        CloseHandle(hEvent);
    }
    if (hTask != NULL)
    {
        AvRevertMmThreadCharacteristics(hTask);
    }
    fldta.scdu = 0;
    fldta.seek = 0;
    flags = 0;
    schedule.SetPos( 0);
    fclose(fldta.frp);
    fldta.frp = NULL;
    renderStatus = 0 ;
    return 0;
}



// unsigned int WINAPI Syc(void *)
// {

// }