#include "Audio.h"

Audio g_audio;

void LoadAudio()
{
	g_audio.fire_rifle = LoadSound("./assets/audio/fire_rifle.wav");
	g_audio.fire_shotgun = LoadSound("./assets/audio/fire_shotgun.wav");
	g_audio.fire_grenade = LoadSound("./assets/audio/fire_grenade.wav");
	g_audio.fire_missile = LoadSound("./assets/audio/fire_missile.wav");
	g_audio.fire_dasher = LoadSound("./assets/audio/fire_dasher.wav");

	g_audio.hit = LoadSound("./assets/audio/hit.wav");
	g_audio.hit_mech = LoadSound("./assets/audio/hit_mech.wav");

	// Make audio quiet for testing ;)
	SetMasterVolume(0.25f);
}

void UnloadAudio()
{
	UnloadSound(g_audio.hit_mech);
	UnloadSound(g_audio.hit);

	UnloadSound(g_audio.fire_dasher);
	UnloadSound(g_audio.fire_missile);
	UnloadSound(g_audio.fire_grenade);
	UnloadSound(g_audio.fire_shotgun);
	UnloadSound(g_audio.fire_rifle);
}
