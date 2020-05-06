#include <SDL.h>
#include <assert.h>

#include "server.h"
#include "netplayer.h"
#include "shared/memory.h"
#include "shared/log.h"
#include "shared/netevent.h"

Server* GetServer()
{
    static Server server;
    static bool inited = false;
    if(!inited)
    {
        Network* net = GetNetwork();

        inited = true;

        if(SDL_Init(0)!=0) 
        {
            SDL_Log("SDL_Init: %s\n", SDL_GetError());
            abort();
            exit(0);
        }

        hashtable_init(&server.playersByID, sizeof(NetPlayer), CR_MAX_PLAYERS, 0);
        // hashtable_init(&server.playersBySocket, sizeof(NetPlayer), CR_MAX_PLAYERS, 0);

        // network.udpChannel = SDLNet_UDP_Bind(network.udpSocket, 1, &udpaddr);
        // if(network.udpChannel == -1)
        // {
        //     printf("Could not bind UDP socket");
        //     abort();
        // }

        server.tcpSocketSet = SDLNet_AllocSocketSet(CR_MAX_PLAYERS);
        if(server.tcpSocketSet == NULL)
        {
            printf("Couldn't create socket set: %s\n", SDLNet_GetError());
            abort();
        }

        server.udpSocketSet = SDLNet_AllocSocketSet(2);
        if(server.udpSocketSet == NULL)
        {
            printf("Couldn't create socket set: %s\n", SDLNet_GetError());
            abort();
        }

        int numused = SDLNet_UDP_AddSocket(server.udpSocketSet, net->udpSocket);
        if(numused == -1) {
            printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
            abort();
        }
    }

    return &server;
}

void ServerDispose()
{
    Server* server = GetServer();

    NetPlayer* players = ServerGetAllPlayers();
    for (size_t i = 0; i < ServerGetPlayerCount(); i++)
    {
        ServerDisposePlayer(&players[i]);
    }

    SDLNet_FreeSocketSet(server->tcpSocketSet);
    // SDLNet_FreeSocketSet(server->udpSocketSet);
}

uint16_t ServerGetPlayerCount()
{
    Server* server = GetServer();

    // assert(hashtable_count(&server->playersByID) == hashtable_count(&server->playersBySocket));

    return hashtable_count(&server->playersByID);
}

NetPlayer* ServerGetAllPlayers()
{
    return hashtable_items(&GetServer()->playersByID);
}

NetPlayer* ServerGetPlayerByID(PlayerID id)
{
    Server* server = GetServer();
    return hashtable_find(&server->playersByID, id);
}

NetPlayer* ServerGetPlayerBySocket(TCPsocket socket)
{
    Server* server = GetServer();
    // NetPlayer* ptr = (NetPlayer*)hashtable_find(&server->playersBySocket, socket);
    // return ptr;

    NetPlayer* players = ServerGetAllPlayers();
    for (size_t i = 0; i < ServerGetPlayerCount(); i++)
    {
        NetPlayer* player = &players[i];

        if(player->tcpSocket == socket)
        {
            return player;
        }
    }
    
    return NULL;
}

PlayerID GetFreeID()
{
    Server* server = GetServer();

    PlayerID id = 0;
    while (true)
    {
        if(ServerGetPlayerByID(id) == NULL) return id;
        id++;
    }

    // will never happen
    // LogInfo("COULD NOT FIND A FREE ID");
    // abort();
}

NetPlayer* ServerInitPlayer(TCPsocket tcpSocket, uint16_t udpPort)
{
    Server* server = GetServer();

    NetPlayer player;

    // IPaddress* ip = SDLNet_TCP_GetPeerAddress(tcpSocket);

    player.tcpSocket = tcpSocket;
    player.udpPort = udpPort;
    player.data.id = GetFreeID();
    player.data.x = rand() % CR_MAP_WIDTH;
    player.data.y = rand() % CR_MAP_HEIGHT;
    player.data.angle = 0;
    if(player.data.id == 0) player.data.infected = true;
    else player.data.infected = false;
    player.data.infectionRadius = 50;
    player.connected = true;

    SDLNet_TCP_AddSocket(server->tcpSocketSet, tcpSocket);

    NetPlayer* res = hashtable_insert(&server->playersByID, player.data.id, &player);
    // hashtable_insert(&server->playersBySocket, tcpSocket, &res);
    return res;
}

void ServerDisposePlayer(NetPlayer* player)
{
    Server* server = GetServer();

    SDLNet_TCP_DelSocket(server->tcpSocketSet, player->tcpSocket);

    // hashtable_remove(&server->playersBySocket, player->tcpSocket);
    hashtable_remove(&server->playersByID, player->data.id);
}

void ServerDisconnectPlayer(NetPlayer* player, bool skipself)
{
    LogInfo("DISCONNECTING PLAYER %d\n", player->data.id);

    Server* server = GetServer();

    PlayerID id = player->data.id;
    ServerDisposePlayer(player);

    NetPlayer* players = ServerGetAllPlayers();
    for (size_t i = 0; i < ServerGetPlayerCount(); i++)
    {
        NetPlayer* ply = &players[i];
        if(skipself && ply == player) continue; // skip himself

        NetEventPlayerDisconnected data = { id };
        NetEventPlayerDisconnectedSend(ply->tcpSocket, &data);
    }
}

int ServerGetAllPlayerData(PlayerData* dataArray)
{
    Server* server = GetServer();

    int count = hashtable_count(&server->playersByID);
    // HASHTABLE_U64 const* keys = hashtable_keys( &server->players );
    NetPlayer* players = hashtable_items(&server->playersByID);
    for (size_t i = 0; i < count; i++)
    {
        NetPlayer* otherply = &players[i];

        memcpy(&dataArray[i], &otherply->data, sizeof(PlayerData));
    }

    return count;
}

void ApplyMovementDataToPlayer(PlayerMovementData* data)
{
    NetPlayer* player = ServerGetPlayerByID(data->id);
    if(player == NULL)
    {
        // player is sending data when it's not in this session
        return;
    }

    player->data.angle = data->angle;
    player->data.x = data->x;
    player->data.y = data->y;
}

void ServerUpdate()
{
    NetPlayer* players = ServerGetAllPlayers();
    uint16_t playercount = ServerGetPlayerCount();

    // for every player
    for (size_t i = 0; i < playercount; i++)
    {
        // Get the player
        NetPlayer* player = &players[i];

        // if(!NetPlayerConnected(player)) continue;

        NetPlayerUpdate(player);
    }
}

void ServerAcceptConnection()
{
    Network* net = GetNetwork();

    TCPsocket incomingSocket = SDLNet_TCP_Accept(net->tcpSocket);
    if(incomingSocket == NULL) return;

    // Get client UDP port to send packets to
    if(GetTCPMessageLength(incomingSocket) == 0)
    {
        printf("Could not get client UDP port, connection invalid!\n");
        return;
    }
    uint16_t udpPort = 0;
    if(!ReadTCPMessage(incomingSocket, &udpPort, sizeof(uint16_t)))
    {
        printf("COULD NOT READ CLIENT PORT\n");
        return;
    }
    
    // Player count before connecting
    uint16_t playercount = ServerGetPlayerCount();
    if(playercount == CR_MAX_PLAYERS)
    {
        LogInfo("MAXIMUM PLAYERS REACHED, REFUSING CONNECTION\n");
        char* reason = "SERVER IS FULL";
        if(!SendTCPMessage(incomingSocket, reason, strlen(reason)+1))
        {
            LogInfo("TCP CONNECTION LOST\n");
            return;
        }
        SDLNet_TCP_Close(incomingSocket);
    }
    else {
        // Send ok confirmation
        char* reason = "OK";
        if(!SendTCPMessage(incomingSocket, reason, strlen(reason)+1))
        {
            LogInfo("TCP CONNECTION LOST\n");
            return;
        }

        // Send max players
        {
            uint16_t maxplayers = CR_MAX_PLAYERS;
            if(!SendTCPMessageNoCopy(incomingSocket, &maxplayers, sizeof(uint16_t)))
            {
                LogInfo("TCP CONNECTION LOST\n");
                return;
            }
        }

        // Send all player data to player
        PlayerData* data = malloca(sizeof(PlayerData)*playercount);
        int datacount = ServerGetAllPlayerData(data);
        if(!SendTCPMessageArray(incomingSocket, data, sizeof(PlayerData), datacount))
        {
            LogInfo("COULD NOT SEND ALL PLAYER DATA\n");
            return;
        }
        freea(data);

        // Register and send player
        NetPlayer* connectingplayer = ServerInitPlayer(incomingSocket, udpPort);
        if(!SendTCPMessage(incomingSocket, &connectingplayer->data, sizeof(PlayerData)))
        {
            LogInfo("COULD NOT SEND PLAYER DATA\n");
            abort();
        }

        // Send connecting player event to all players
        NetPlayer* players = ServerGetAllPlayers();
        for (size_t i = 0; i < playercount; i++)
        {
            NetPlayer* player = &players[i];

            // Skip sending event to the connecting player himself
            if(player->tcpSocket == connectingplayer->tcpSocket)
                continue;

            // Send event
            NetEventPlayerConnected ev;
            ev.data = connectingplayer->data;
            NetEventPlayerConnectedSend(player->tcpSocket, &ev);
        }

        LogInfo("Player connected, ID: %d, Socket: %x\n", connectingplayer->data.id, incomingSocket);
    }
}

void ServerBroadcastPlayerData()
{
    NetPlayer* players = ServerGetAllPlayers();
    uint16_t playercount = ServerGetPlayerCount();
    // PlayerData* data = alloca(sizeof(PlayerData)+playercount);
    // int datacount = GetAllPlayerData(&data);

    // for every player
    for (size_t i = 0; i < playercount; i++)
    {
        // Get the player
        NetPlayer* player = &players[i];

        // send all others players data to the player
        for (size_t i = 0; i < playercount; i++)
        {
            // get the other player
            NetPlayer* otherPlayer = &players[i];

            // don't send player data to himself
            if(otherPlayer == player) continue;

            // get other players data
            PlayerMovementData data;
            NetPlayerGetMovementData(otherPlayer, &data);
            // get recipient udp ip
            IPaddress udpaddr;
            udpaddr.host = NetPlayerGetTCPAddress(player)->host;
            udpaddr.port = player->udpPort;
            // send other player's data to the recipient
            SendMovementData_UDP(&udpaddr, &data);
        }
    }
}

void ServerReadEvents()
{
    // Network* net = GetNetwork();
    Server* server = GetServer();

    int res = SDLNet_CheckSockets(server->tcpSocketSet, 0);
    if(!res == -1)
    {
        // error with sockets
        abort();
        return;
    }

    NetPlayer* players = ServerGetAllPlayers();
    for (size_t i = 0; i < ServerGetPlayerCount(); i++)
    {
        NetPlayer* player = &players[i];

        if(SDLNet_SocketReady(player->tcpSocket))
        {
            NetEvent event = NetEventGet(player->tcpSocket);
            switch (event)
            {
            case CR_NETEVENT_PlayerDisconnected:
            {
                NetEventPlayerDisconnected e;
                NetEventPlayerDisconnectedRead(player->tcpSocket, &e);
                if(e.id != player->data.id)
                {
                    LogInfo("PLAYER TRIED TO DISCONNECT ANOTHER PLAYER???\n");
                    return;
                }
                ServerDisconnectPlayer(player, true);
                break;
            }
            case CR_NETEVENT_Disconnected:
            {
                // LogInfo("CR_NETEVENT_Disconnected event recieved!\n");
                break;
            }
            
            default:
            {
                LogInfo("UNHANDLED EVENT %d\n", event);
                abort();
                break;
            }
            }
        }
    }
}

void ServerReadUpdates()
{
    static UDPpacket* recvpacket = NULL;

    Network* net = GetNetwork();
    Server* server = GetServer();

    if(recvpacket == NULL)
    {
        recvpacket = SDLNet_AllocPacket(1024);
    }

    assert(SDLNet_CheckSockets(server->udpSocketSet, 0) != -1);

    // Read updates from players
    int res = 0;
    while(res = SDLNet_UDP_Recv(net->udpSocket, recvpacket))
    {
        DataID dataid = GetDataID_UDP(recvpacket);
        switch (dataid)
        {
        case CR_DATA_MOVEMENT:
        {
            PlayerMovementData* data = GetMovementData_UDP(recvpacket);
            // printf("RECIEVED POSITION {x: %d, y: %d}\n", data->x, data->y);
            ApplyMovementDataToPlayer(data);
            break;
        }
        
        default:
            LogInfo("UNHANDLED DATA ID %d\n", dataid);
            break;
        }
    }

    if(res == -1) abort();
}
