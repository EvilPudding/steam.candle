#include "steam.h"

#include <string.h>
#include <systems/editmode.h>
#include <stdlib.h>
#include <stdio.h>
#include <utils/mafs.h>
#define bool _Bool
#define S_API
#include "steam_api_c.h"

void c_steam_init(c_steam_t *self)
{
	strcpy(self->persona_name, "not connected");
	int started = SteamAPI_Init();
	if(!started) return;
	self->client = SteamClient();
	HSteamPipe hsteampipe = SteamAPI_ISteamClient_CreateSteamPipe(self->client);
	HSteamUser hsteamuser = SteamAPI_ISteamClient_ConnectToGlobalUser(self->client, hsteampipe);

	ISteamUser *isteamuser = SteamAPI_ISteamClient_GetISteamUser(self->client, hsteamuser, hsteampipe, "SteamUser019");

	intptr_t isteamfriends = (intptr_t)SteamAPI_ISteamClient_GetISteamFriends(self->client, hsteamuser, hsteampipe, "SteamFriends015");

	uint64 steamid = SteamAPI_ISteamUser_GetSteamID((intptr_t)isteamuser);
	printf("SteamID: %llu\n", steamid);

	const char *persona_name = SteamAPI_ISteamFriends_GetPersonaName((intptr_t)isteamfriends);
	printf("Persona name: %s\n", self->persona_name);

	strncpy(self->persona_name, persona_name, sizeof(self->persona_name));

	printf("OVERLAY? %d\n", SteamAPI_ISteamUtils_IsOverlayEnabled(self->client));
}

c_steam_t *c_steam_new()
{
	c_steam_t *self = component_new("steam");

	return self;

}

void c_steam_destroy(c_steam_t *self)
{
	SteamAPI_Shutdown();
}

int c_steam_menu(c_steam_t *self, void *ctx)
{
	nk_layout_row_begin(ctx, NK_DYNAMIC, 30, 2);
		nk_layout_row_push(ctx, 0.35);
		nk_label(ctx, "Steam name: ", NK_TEXT_LEFT);
		nk_layout_row_push(ctx, 0.65);
		nk_label(ctx, self->persona_name, NK_TEXT_RIGHT);
	nk_layout_row_end(ctx);

	return CONTINUE;
}

REG()
{
	ct_t *ct = ct_new("steam", sizeof(c_steam_t),
			c_steam_init, c_steam_destroy, 0);

	ct_listener(ct, WORLD, sig("component_menu"), c_steam_menu);
}

