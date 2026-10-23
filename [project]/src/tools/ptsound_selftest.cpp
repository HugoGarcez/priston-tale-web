// ptsound_selftest.cpp - checks the DirectSound emulation (ptsound.cpp) on SDL2.
// Buffers play at minimum volume, so nothing is heard.
// Build + run: tools\ptsound_selftest.bat

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include "..\ptsound\ptsound.h"

static int g_Fail = 0;
#define CHECK( cond, what ) do { BOOL ok_ = (cond) ? TRUE : FALSE; printf( "  %s %s\n", ok_ ? "ok  " : "FAIL", what ); if ( !ok_ ) g_Fail++; } while ( 0 )

int main()
{
	LPDIRECTSOUND ds = NULL;
	CHECK( PtSound_DirectSoundCreate( NULL, &ds, NULL )==DS_OK && ds, "DirectSoundCreate (SDL audio device)" );
	if ( !ds ) return 1;
	CHECK( ds->SetCooperativeLevel( GetDesktopWindow(), DSSCL_NORMAL )==DS_OK, "SetCooperativeLevel" );

	// 0.5 s of 22050 Hz mono 16-bit, like the game's .wav effects
	WAVEFORMATEX wf = { WAVE_FORMAT_PCM, 1, 22050, 44100, 2, 16, 0 };
	DSBUFFERDESC bd = { sizeof(bd), DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY, 22050, 0, &wf };
	LPDIRECTSOUNDBUFFER b = NULL;
	CHECK( ds->CreateSoundBuffer( &bd, &b, NULL )==DS_OK && b, "CreateSoundBuffer" );

	void *p1, *p2; DWORD n1, n2;
	CHECK( b->Lock( 0, 22050, &p1, &n1, &p2, &n2, 0 )==DS_OK && n1==22050 && n2==0, "Lock whole buffer" );
	for ( DWORD i=0; i<n1/2; i++ ) ((short *)p1)[i] = (short)( ( i%50 )<25 ? 8000 : -8000 );
	b->Unlock( p1, n1, p2, n2 );
	CHECK( b->Lock( 20000, 4000, &p1, &n1, &p2, &n2, 0 )==DS_OK && n1==2050 && n2==1950, "Lock wraps around the end" );
	b->Unlock( p1, n1, p2, n2 );

	b->SetVolume( DSBVOLUME_MIN );
	DWORD st = 0;
	b->GetStatus( &st );
	CHECK( !( st & DSBSTATUS_PLAYING ), "not playing before Play" );

	b->Play( 0, 0, 0 );
	b->GetStatus( &st );
	CHECK( ( st & DSBSTATUS_PLAYING ) && !( st & DSBSTATUS_LOOPING ), "playing after Play" );
	Sleep( 200 );
	DWORD play = 0, write = 0;
	b->GetCurrentPosition( &play, &write );
	CHECK( play>2000 && play<22050, "play cursor advances" );
	Sleep( 600 );
	b->GetStatus( &st );
	CHECK( !( st & DSBSTATUS_PLAYING ), "one-shot sound stops at the end" );

	b->SetCurrentPosition( 0 );
	b->Play( 0, 0, DSBPLAY_LOOPING );
	Sleep( 800 );
	b->GetStatus( &st );
	CHECK( ( st & DSBSTATUS_PLAYING ) && ( st & DSBSTATUS_LOOPING ), "looping sound keeps playing past the end" );
	b->SetFrequency( 44100 );
	DWORD before = 0, after = 0;
	b->GetCurrentPosition( &before, NULL );
	Sleep( 100 );
	b->GetCurrentPosition( &after, NULL );
	DWORD moved = ( after + 22050 - before ) % 22050;
	CHECK( moved>4000, "SetFrequency doubles playback speed" );
	b->Stop();
	b->GetStatus( &st );
	CHECK( !( st & DSBSTATUS_PLAYING ), "Stop" );

	CHECK( b->Release()==0, "buffer Release" );
	ds->Release();

	printf( g_Fail ? "\n%d check(s) FAILED\n" : "\nall checks passed\n", g_Fail );
	return g_Fail ? 1 : 0;
}
