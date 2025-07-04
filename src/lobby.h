#pragma once

#include <stddef.h>

#include "caulk.h"

void lobbyInit();

bool getLobbyId(size_t, CSteamID*);
size_t lobbyCount();
void leaveLobby();

void refreshLobbiesList();
void requestCreateLobby(), requestJoinLobby();
