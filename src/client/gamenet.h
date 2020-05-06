#pragma once

#include <stdbool.h>

bool GameNetIsConnected();
void GameNetSetConnected(bool value);
void GameNetStartThread();
void GameNetDisconnect();
