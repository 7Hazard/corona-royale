#include "gamenet.h"

#include "game.h"
#include "textures.h"

#include "shared/network.h"
#include "shared/data.h"
#include "shared/log.h"
#include "shared/netevent.h"

int NetEventThread(void *ptr)
{
    Game* game = GetGame();
    Network* net = GetNetwork();
    Textures* textures = GetTextures();

    while (GameIsPlaying())
    {
        NetEvent event = NetEventGet(net->tcpSocket);
        switch(event)
        {
        case CR_NETEVENT_Disconnected:
        {
            LogInfo("(EVENT) DISCONNECTED");
            break;
        }
        case CR_NETEVENT_PlayerDisconnected:
        {
            NetEventPlayerDisconnected e;
            if(!NetEventPlayerDisconnectedRead(net->tcpSocket, &e)) break;
            LogInfo("PLAYER %d DISCONNECTED\n", e.id);
            GameDisposeNetPlayer(GameGetNetPlayer(e.id));

            break;
        }
        case CR_NETEVENT_PlayerConnected:
        {
            NetEventPlayerConnected e;
            if(!NetEventPlayerConnectedRead(net->tcpSocket, &e)) break;
            GameInitNetPlayer(&e.data);

            break;
        }
        case CR_NETEVENT_PlayerInfected:
        {
            NetEventPlayerInfected e;
            if(!NetEventPlayerInfectedRead(net->tcpSocket, &e)) break;
            
            if(e.id == game->player.id)
            {
                game->player.infected = true;
                game->player.texture = textures->infectedPlayer;
            }
            else {
                NetPlayer* player = GameGetNetPlayer(e.id);
                player->data.infected = true;
                player->texture = textures->infectedPlayer;
                LogInfo("PLAYER %d GOT INFECTED\n", e.id);
            }

            break;
        }
        case CR_NETEVENT_VirusWin:
        {
            GameSetState(CR_STATE_VIRUSWIN);
            break;
        }
        
        default:
            LogInfo("Unhandled event %d", event);
            abort();
            break;
        }
    }

    return 0;
}

int NetworkThread(void *ptr)
{
    Game* game = GetGame();
    Network* net = GetNetwork();

    // Start event thread
    SDL_Thread* eventThread = SDL_CreateThread(NetEventThread, "NetEventThread", (void *)NULL);

    UDPpacket* recvpacket = SDLNet_AllocPacket(1024);

    // Network loop
    while (GameIsPlaying())
    {
        time_t tickstart = NetworkStartTick();
        ///////// START OF NET TICK

        while(SDLNet_UDP_Recv(net->udpSocket, recvpacket))
        {
            DataID dataid = GetDataID_UDP(recvpacket);
            switch (dataid)
            {
            case CR_DATA_MOVEMENT:
            {
                PlayerMovementData* data = GetMovementData_UDP(recvpacket);
                // printf("RECIEVED POSITION {x: %d, y: %d, angle: %d}\n", data->x, data->y, data->angle);
                ApplyMovementDataToPlayer(data);
                break;
            }
            
            default:
                LogInfo("UNHANDLED DATA ID %d", dataid);
                break;
            }
        }

        // Send 
        PlayerMovementData data;
        GetPlayerMovementData(&game->player, &data);
        SendMovementData_UDP(&data);

        ///////// END OF NET TICK
        NetworkEndTick(tickstart);
    }

    // free session stuff
    GameDisposeNetPlayers();
    SDLNet_FreePacket(recvpacket);

    return 0;
}

void GameNetConnect()
{
    Game* game = GetGame();
    Network* net = GetNetwork();

    if(!Connect(game->menu.textInTextBox))
    {
        SDL_Log("COULD NOT CONNECT TO GAME SERVER");
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "Connection error",
            "Could not connect to server",
            NULL
        );
        
        return;
    }

    { // Get confirmation
        uint16_t len = GetTCPMessageLength(net->tcpSocket);
        if(len == 0)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "TCP read failed",
                "Could not get confirmation from server",
                NULL
            );
            abort();
        }
        
        char* confirmation = alloca(len);
        if(!ReadTCPMessage(net->tcpSocket, confirmation, len))
        {
            SDL_Log("COULD NOT READ CONFIRMATION");
            abort();
        }

        if(strcmp(confirmation, "OK") != 0)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Connection failed",
                confirmation,
                NULL
            );
            LogInfo("CONNECTION FAILED: %s", confirmation);

            return;
        }
        else
        {
            LogInfo("CONNECTED TO SERVER: %s", confirmation);
        }
    }

    // GetMaxPlayers
    uint16_t maxPlayers = 0;
    if(!ReadTCPMessage(net->tcpSocket, &maxPlayers, sizeof(uint16_t)))
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "Connection failed",
            "Could not get max players",
            NULL
        );
        LogInfo("COULD NOT GET MAX PLAYERS");
        abort();
    }
    LogInfo("MAX PLAYERS: %d", maxPlayers);
    GameInitNetPlayersTable(maxPlayers);
    
    { // Get other players
        uint16_t datasize = GetTCPMessageLength(net->tcpSocket);
        uint16_t count = GetTCPMessageLength(net->tcpSocket);

        for (size_t i = 0; i < count; i++)
        {
            PlayerData data;
            if(!ReadTCPMessage(net->tcpSocket, &data, sizeof(PlayerData)))
            {
                abort();
            }

            GameInitNetPlayer(&data);
        }
    }
    
    { // Get player data
        uint16_t len = GetTCPMessageLength(net->tcpSocket);
        if(len == 0)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "TCP read failed",
                "Could not get length of player data",
                NULL
            );
            abort();
        }
        
        PlayerData data;
        if(!ReadTCPMessage(net->tcpSocket, &data, len))
        {
            SDL_Log("COULD NOT READ PLAYER DATA");
            abort();
        }

        ApplyPlayerData(&game->player, &data);
    }

    GameSetState(CR_STATE_CONNECTED);

    // Start network thread
    SDL_Thread* networkThread = SDL_CreateThread(NetworkThread, "NetworkThread", (void *)NULL);
    SDL_DetachThread(networkThread);
}

void GameNetDisconnect()
{
    Game* game = GetGame();
    Network* net = GetNetwork();

    if(!GameGetState()) return;
    
    NetEventPlayerDisconnected e = { game->player.id };
    NetEventPlayerDisconnectedSend(net->tcpSocket, &e);

    GameSetState(CR_STATE_MENU);
}
