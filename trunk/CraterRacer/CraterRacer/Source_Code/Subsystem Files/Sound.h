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
#include "Constants.h"
#include "ApplicationState.h"


//-----------------------------------------------------------------------------
// Forward declaration
//-----------------------------------------------------------------------------
HRESULT PrepareXACT( LPCWSTR bgWavebankFile, LPCWSTR seWavebankFile, LPCWSTR bgSettingsFile, LPCWSTR bgSoundbankFile );
void UpdateAudio(ApplicationState appState);
VOID CleanupXACT();
void WINAPI XACTNotificationCallback( const XACT_NOTIFICATION* pNotification );
void playVictory();
void playStartMenu();
void playPauseMenu();
void pauseMusic();
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );

extern enum ApplicationState;
//-----------------------------------------------------------------------------
// Struct to hold audio game state
//-----------------------------------------------------------------------------
struct AUDIO_STATE
{
    XACTINDEX iApplicationStart;
    XACTINDEX iGameStart;
	XACTINDEX iStartMenu;
	XACTINDEX iPauseMenu;
	XACTINDEX iVictoryMusic;
	XACTINDEX iEngine;
	XACTINDEX iImpact;
	XACTINDEX iCollision;
	XACTINDEX iBoost;

	XACTCATEGORY iMusicCategory;
    XACTCATEGORY iEffectCategory;
    XACTVARIABLEINDEX iRPMVariable;
    XACTVARIABLEVALUE nRPM;

	float fMusicVolume;
	float fEffectVolume;

	int acceleration;
	bool reverse;

	ApplicationState gameState;

    IXACT3Engine* pEngine;
    IXACT3SoundBank* pSoundBank;
    IXACT3WaveBank* pBGMusicWaveBank;//pStreamingWaveBank;
	IXACT3WaveBank* pSoundEffectWaveBank;

    IXACT3Cue* pZeroLatencyRevCue;

    // Handles to audio files to be closed upon cleanup
    HANDLE hBGMusicWaveBankFile; // Handle to wave bank data.  Its memory mapped so call UnmapViewOfFile() upon cleanup to release file
    VOID* pbSoundBank; // Pointer to sound bank data.  Call delete on it when the sound bank is destroyed
	VOID* pbGameWaveBank;
	VOID* pbSoundEffectWaveBank;

    CRITICAL_SECTION cs;
    bool bHandleStreamingWaveBankPrepared;
    bool bHandleZeroLatencyCueStop;
    bool bHandleSongStopped;
};

extern AUDIO_STATE g_audioState;

#endif SOUND_H
