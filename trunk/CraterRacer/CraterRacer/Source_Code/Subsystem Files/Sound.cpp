#include "Sound.h"

AUDIO_STATE g_audioState;
//-----------------------------------------------------------------------------------------
// This function does the following steps:
//
//      1. Initialize XACT by calling pEngine->Initialize 
//      2. Register for the XACT notification desired
//      3. Create the in memory XACT wave bank(s) you want to use
//      4. Create the streaming XACT wave bank(s) you want to use
//      5. Create the XACT sound bank(s) you want to use
//      6. Store indices to the XACT cue(s) your game uses
//-----------------------------------------------------------------------------------------
HRESULT PrepareXACT( LPCWSTR bgWavebankFile, LPCWSTR bgSoundbankFile )
{
    HRESULT hr;
    WCHAR str[MAX_PATH];
    HANDLE hFile;
    DWORD dwFileSize;
    DWORD dwBytesRead;
    HANDLE hMapFile;

    // Clear struct
    ZeroMemory( &g_audioState, sizeof( AUDIO_STATE ) );
	InitializeCriticalSection( &g_audioState.cs );

    hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );  // COINIT_APARTMENTTHREADED will work too
    if( SUCCEEDED( hr ) )
    {
        // Switch to auditioning mode based on command line.  Change if desired
        bool bAuditionMode = DoesCommandLineContainAuditionSwitch();
        bool bDebugMode = false;

        DWORD dwCreationFlags = 0;
        if( bAuditionMode ) dwCreationFlags |= XACT_FLAG_API_AUDITION_MODE;
        if( bDebugMode ) dwCreationFlags |= XACT_FLAG_API_DEBUG_MODE;

        hr = XACT3CreateEngine( dwCreationFlags, &g_audioState.pEngine );
    }
    if( FAILED( hr ) || g_audioState.pEngine == NULL )
        return E_FAIL;

    // Initialize & create the XACT runtime 
    XACT_RUNTIME_PARAMETERS xrParams = {0};
    xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	xrParams.fnNotificationCallback = XACTNotificationCallback;
    hr = g_audioState.pEngine->Initialize( &xrParams );
    if( FAILED( hr ) )
        return hr;

	//-----------------------------------------------------------------------------------------
    // Register for XACT notifications
    //-----------------------------------------------------------------------------------------

    // The "wave bank prepared" notification will let the app know when it is save to use
    // play cues that reference streaming wave data.
    XACT_NOTIFICATION_DESCRIPTION desc = {0};
    desc.flags = XACT_FLAG_NOTIFICATION_PERSIST;
    desc.type = XACTNOTIFICATIONTYPE_WAVEBANKPREPARED;
    g_audioState.pEngine->RegisterNotification( &desc );

    // The "sound bank destroyed" notification will let the app know when it is save to use
    // play cues that reference streaming wave data.
    desc.flags = XACT_FLAG_NOTIFICATION_PERSIST;
    desc.type = XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED;
    g_audioState.pEngine->RegisterNotification( &desc );

    // The "cue stop" notification will let the app know when it a song stops so a new one 
    // can be played
    desc.flags = XACT_FLAG_NOTIFICATION_PERSIST;
    desc.type = XACTNOTIFICATIONTYPE_CUESTOP;
    desc.cueIndex = XACTINDEX_INVALID;
    g_audioState.pEngine->RegisterNotification( &desc );

    // The "cue prepared" notification will let the app know when it a a cue that uses 
    // streaming data has been prepared so it is ready to be used for zero latency streaming
    desc.flags = XACT_FLAG_NOTIFICATION_PERSIST;
    desc.type = XACTNOTIFICATIONTYPE_CUEPREPARED;
    desc.cueIndex = XACTINDEX_INVALID;
    g_audioState.pEngine->RegisterNotification( &desc );

    
    //-----------------------------------------------------------------------------------------
    // Create a streaming XACT wave bank file.  
    // Take note of the following:
    // 1) This wave bank in the XACT project file must marked as a streaming wave bank 
    //    This is set inside the XACT authoring tool)
    // 2) Use FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING flags when opening the file 
    // 3) To use cues that reference this streaming wave bank, you must wait for the 
    //    wave bank to prepared first or the playing the cue will fail
    //-----------------------------------------------------------------------------------------
    if( FAILED( hr = FindMediaFileCch( str, MAX_PATH, bgWavebankFile ) ) )
        return hr;
    hr = E_FAIL; // assume failure
    g_audioState.hBGMusicWaveBankFile = CreateFile( str,
                                                      GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                                      FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL );
    if( g_audioState.hBGMusicWaveBankFile != INVALID_HANDLE_VALUE )
    {
        XACT_WAVEBANK_STREAMING_PARAMETERS wsParams;
        ZeroMemory( &wsParams, sizeof( XACT_WAVEBANK_STREAMING_PARAMETERS ) );
        wsParams.file = g_audioState.hBGMusicWaveBankFile;
        wsParams.offset = 0;

        // 64 means to allocate a 64 * 2k buffer for streaming.  
        // This is a good size for DVD streaming and takes good advantage of the read ahead cache
        wsParams.packetSize = 64;

        hr = g_audioState.pEngine->CreateStreamingWaveBank( &wsParams, &g_audioState.pBGMusicWaveBank );
    }

    // Read and register the sound bank file with XACT.  Do not use memory mapped file IO because the 
    // memory needs to be read/write and the working set of sound banks are small.
    if( FAILED( hr = FindMediaFileCch( str, MAX_PATH, bgSoundbankFile ) ) )
        return hr;
    hr = E_FAIL; // assume failure
    hFile = CreateFile( str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            // Allocate the data here and free the data when recieving the sound bank destroyed notification
            g_audioState.pbSoundBank = new BYTE[dwFileSize];
            if( g_audioState.pbSoundBank )
            {
                if( 0 != ReadFile( hFile, g_audioState.pbSoundBank, dwFileSize, &dwBytesRead, NULL ) )
                {
                    hr = g_audioState.pEngine->CreateSoundBank( g_audioState.pbSoundBank, dwFileSize, 0,
                                                                0, &g_audioState.pSoundBank );
                }
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( hr ) )
        return E_FAIL; // CleanupXACT() will cleanup state before exiting

    // Get the sound cue index from the sound bank
    //
    // Note that if the cue does not exist in the sound bank, the index will be XACTINDEX_INVALID
    // however this is ok especially during development.  The Play or Prepare call will just fail.
	g_audioState.iGameStart = g_audioState.pSoundBank->GetCueIndex( "GameStart" );

    return S_OK;
}

//-----------------------------------------------------------------------------------------
// This is the callback for handling XACT notifications.  This callback can be executed on a 
// different thread than the app thread so shared data must be thread safe.  The game 
// also needs to minimize the amount of time spent in this callbacks to avoid glitching, 
// and a limited subset of XACT API can be called from inside the callback so 
// it is sometimes necessary to handle the notification outside of this callback.
//-----------------------------------------------------------------------------------------
void WINAPI XACTNotificationCallback( const XACT_NOTIFICATION* pNotification )
{
    // Use the critical section properly to make shared data thread safe while avoiding deadlocks.  
    //
    // To do this follow this advice:
    // 1) Use a specific CS only to protect the specific shared data structures between the callback and the app thread.
    // 2) Don’t make any API calls while holding the CS. Use it to access the shared data, make a local copy of the data, release the CS and then make the API call.
    // 3) Spend minimal amount of time in the CS (to prevent the callback thread from waiting too long causing a glitch).   
    // 
    // Instead of using a CS, you can also use a non-blocking queues to keep track of notifications meaning 
    // callback will push never pop only push and the app thread will only pop never push

    // In this simple tutorial, we will respond to a cue stop notification for the song 
    // cues by simply playing another song but its ultimately it's up the application 
    // and sound designer to decide what to do when a notification is received. 
    if( pNotification->type == XACTNOTIFICATIONTYPE_CUESTOP )
    {
        // The previous background song ended, so pick and new song to play it
        EnterCriticalSection( &g_audioState.cs );
        g_audioState.bHandleSongStopped = true;
        LeaveCriticalSection( &g_audioState.cs );
    }

    if( pNotification->type == XACTNOTIFICATIONTYPE_WAVEBANKPREPARED &&
        pNotification->waveBank.pWaveBank == g_audioState.pBGMusicWaveBank )
    {
        // Respond to this notification outside of this callback so Prepare() can be called
        EnterCriticalSection( &g_audioState.cs );
        g_audioState.bHandleStreamingWaveBankPrepared = true;
        LeaveCriticalSection( &g_audioState.cs );
    }

    if( pNotification->type == XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED &&
        pNotification->soundBank.pSoundBank == g_audioState.pSoundBank )
    {
        // Cleanup sound bank memory
        if( g_audioState.pbSoundBank )
        {
            delete[] g_audioState.pbSoundBank;
            g_audioState.pbSoundBank = NULL;
        }
    }
}

//-----------------------------------------------------------------------------------------
// Handle these notifications outside of the callback and call pEngine->DoWork()
//-----------------------------------------------------------------------------------------
void UpdateAudio()
{

	EnterCriticalSection( &g_audioState.cs );
    bool bHandleStreamingWaveBankPrepared = g_audioState.bHandleStreamingWaveBankPrepared;
	LeaveCriticalSection( &g_audioState.cs );

	if( bHandleStreamingWaveBankPrepared )
    {
        EnterCriticalSection( &g_audioState.cs );
        g_audioState.bHandleStreamingWaveBankPrepared = false;
        LeaveCriticalSection( &g_audioState.cs );

        // Starting playing background music after the streaming wave bank 
        // has been prepared but no sooner.  The background music does not need to be 
        // zero-latency so the cues do not need to be prepared first 
        g_audioState.pSoundBank->Play(g_audioState.iGameStart, 0, 0, NULL);
    }

    if( g_audioState.bHandleSongStopped )
    {
        EnterCriticalSection( &g_audioState.cs );
        g_audioState.bHandleSongStopped = false;
        LeaveCriticalSection( &g_audioState.cs );

        g_audioState.pSoundBank->Play(g_audioState.iGameStart, 0, 0, NULL);
    }


    // It is important to allow XACT to do periodic work by calling pEngine->DoWork().  
    // However this must function be call often enough.  If you call it too infrequently, 
    // streaming will suffer and resources will not be managed promptly.  On the other hand 
    // if you call it too frequently, it will negatively affect performance. Calling it once 
    // per frame is usually a good balance.
    if( g_audioState.pEngine )
        g_audioState.pEngine->DoWork();
}

//-----------------------------------------------------------------------------
// Releases all previously initialized XACT objects
//-----------------------------------------------------------------------------
VOID CleanupXACT()
{
    // Shutdown XACT
    //
    // Note that pEngine->ShutDown is synchronous and will take some time to complete 
    // if there are still playing cues.  Also pEngine->ShutDown() is generally only 
    // called when a game exits and is not the preferred method of changing audio 
    // resources. To know when it is safe to free wave/sound bank data without 
    // shutting down the XACT engine, use the XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED 
    // or XACTNOTIFICATIONTYPE_WAVEBANKDESTROYED notifications 
    if( g_audioState.pEngine )
    {
        g_audioState.pEngine->ShutDown();
        g_audioState.pEngine->Release();
    }

    if( g_audioState.pbSoundBank )
        delete[] g_audioState.pbSoundBank;
    g_audioState.pbSoundBank = NULL;

    // After pEngine->ShutDown() returns it is safe to release memory mapped files
    if( g_audioState.pbGameWaveBank )
        UnmapViewOfFile( g_audioState.pbGameWaveBank );
    g_audioState.pbGameWaveBank = NULL;

    CoUninitialize();
}


//--------------------------------------------------------------------------------------
// Helper function to try to find the location of a media file
//--------------------------------------------------------------------------------------
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename )
{
    bool bFound = false;

    if( NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10 )
        return E_INVALIDARG;

    // Get the exe name, and exe path
    WCHAR strExePath[MAX_PATH] = {0};
    WCHAR strExeName[MAX_PATH] = {0};
    WCHAR* strLastSlash = NULL;
    GetModuleFileName( NULL, strExePath, MAX_PATH );
    strExePath[MAX_PATH - 1] = 0;
    strLastSlash = wcsrchr( strExePath, TEXT( '\\' ) );
    if( strLastSlash )
    {
        StringCchCopy( strExeName, MAX_PATH, &strLastSlash[1] );

        // Chop the exe name from the exe path
        *strLastSlash = 0;

        // Chop the .exe from the exe name
        strLastSlash = wcsrchr( strExeName, TEXT( '.' ) );
        if( strLastSlash )
            *strLastSlash = 0;
    }

    StringCchCopy( strDestPath, cchDest, strFilename );
    if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
        return S_OK;

    // Search all parent directories starting at .\ and using strFilename as the leaf name
    WCHAR strLeafName[MAX_PATH] = {0};
    StringCchCopy( strLeafName, MAX_PATH, strFilename );

    WCHAR strFullPath[MAX_PATH] = {0};
    WCHAR strFullFileName[MAX_PATH] = {0};
    WCHAR strSearch[MAX_PATH] = {0};
    WCHAR* strFilePart = NULL;

    GetFullPathName( L".", MAX_PATH, strFullPath, &strFilePart );
    if( strFilePart == NULL )
        return E_FAIL;

    while( strFilePart != NULL && *strFilePart != '\0' )
    {
        StringCchPrintf( strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            StringCchCopy( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        StringCchPrintf( strFullFileName, MAX_PATH, L"%s\\Tutorials\\%s\\%s", strFullPath, strExeName, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            StringCchCopy( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        StringCchPrintf( strSearch, MAX_PATH, L"%s\\..", strFullPath );
        GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
    }
    if( bFound )
        return S_OK;

    // On failure, return the file as the path but also return an error code
    StringCchCopy( strDestPath, cchDest, strFilename );

    return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
}

//--------------------------------------------------------------------------------------
bool DoesCommandLineContainAuditionSwitch()
{
    const WCHAR* strAuditioning = L"-audition"; size_t nArgLen; int nNumArgs;
    LPWSTR* pstrArgList = CommandLineToArgvW( GetCommandLine(), &nNumArgs );
    for( int iArg = 1; iArg < nNumArgs; iArg++ )
    {
        StringCchLength( pstrArgList[iArg], 256, &nArgLen );
        if( _wcsnicmp( pstrArgList[iArg], strAuditioning, nArgLen ) == 0 && nArgLen == 9 )
            return true;
    }
    LocalFree( pstrArgList );
    return false;
}