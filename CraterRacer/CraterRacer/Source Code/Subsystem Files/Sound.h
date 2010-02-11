#ifndef SOUND_H
#define SOUND_H
#define _WIN32_DCOM
#include <windows.h>
#include <commdlg.h>
#include <xact3.h>
#include <shellapi.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

//-----------------------------------------------------------------------------
// Forward declaration
//-----------------------------------------------------------------------------
HRESULT PrepareXACT();
void UpdateAudio();
VOID CleanupXACT();
void WINAPI XACTNotificationCallback( const XACT_NOTIFICATION* pNotification );
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );
bool DoesCommandLineContainAuditionSwitch();

//-----------------------------------------------------------------------------
// Struct to hold audio game state
//-----------------------------------------------------------------------------
struct AUDIO_STATE
{
    XACTINDEX iApplicationStart;
    XACTINDEX iGameStart;

    IXACT3Engine* pEngine;
    IXACT3SoundBank* pSoundBank;
	IXACT3WaveBank* pWaveBank;
    IXACT3WaveBank* pMenuWaveBank;//pInMemoryWaveBank;
    IXACT3WaveBank* pGameBGMusicWaveBank;//pStreamingWaveBank;

    IXACT3Cue* pZeroLatencyRevCue;

    // Handles to audio files to be closed upon cleanup
    HANDLE hMenuWaveBankFile;
    HANDLE hGameMusicWaveBankFile; // Handle to wave bank data.  Its memory mapped so call UnmapViewOfFile() upon cleanup to release file
    VOID* pbSoundBank; // Pointer to sound bank data.  Call delete on it when the sound bank is destroyed
	VOID* pbWaveBank;
	VOID* pbGameWaveBank;

    CRITICAL_SECTION cs;
    bool bHandleStreamingWaveBankPrepared;
    bool bHandleZeroLatencyCueStop;
    bool bHandleSongStopped;
};

extern AUDIO_STATE g_audioState;

#endif SOUND_H
