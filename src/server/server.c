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
        inited = true;

        hashtable_init(&server.players, sizeof(NetPlayer), CR_MAX_PLAYERS, 0);
    }

    return &server;
}

uint16_t GetPlayerCount()
{
    Server* server = GetServer();
    return hashtable_count(&server->players);
}

NetPlayer* GetAllPlayers()
{
    return hashtable_items(&GetServer()->players);
}

NetPlayer* GetPlayer(PlayerID id)
{
    Server* server = GetServer();
    return hashtable_find(&server->players, id);
}

PlayerID GetFreeID()
{
    Server* server = GetServer();

    PlayerID id = 0;
    while (true)
    {
        if(GetPlayer(id) == NULL) return id;
        id++;
    }

    // will never happen
    // LogInfo("COULD NOT FIND A FREE ID");
    // abort();
}

NetPlayer* InitPlayer(TCPsocket tcpSocket, uint16_t udpPort)
{
    Server* server = GetServer();

    NetPlayer player;

    // IPaddress* ip = SDLNet_TCP_GetPeerAddress(tcpSocket);

    player.tcpSocket = tcpSocket;
    // player.udpSocket = SDLNet_UDP_Open(ip);
    player.udpPort = udpPort;
    player.data.id = GetFreeID();
    player.data.x = rand() % CR_MAP_WIDTH;
    player.data.y = rand() % CR_MAP_HEIGHT;
    player.data.angle = 0;
    if(player.data.id == 0) player.data.infected = true;
    else player.data.infected = false;
    player.data.infectionRadius = 50;

    return hashtable_insert(&server->players, player.data.id, &player);
}

void ServerDisconnectPlayer(NetPlayer* player)
{
    LogInfo("PLAYER %d DISCONNECTED\n", player->data.id);

    Server* server = GetServer();

    hashtable_remove(&server->players, player->data.id);
}

int GetAllPlayerData(PlayerData* dataArray)
{
    Server* server = GetServer();

    int count = hashtable_count(&server->players);
    // HASHTABLE_U64 const* keys = hashtable_keys( &server->players );
    NetPlayer* players = hashtable_items(&server->players);
    for (size_t i = 0; i < count; i++)
    {
        NetPlayer* otherply = &players[i];

        memcpy(&dataArray[i], &otherply->data, sizeof(PlayerData));
    }

    return count;
}

void ApplyMovementDataToPlayer(PlayerMovementData* data)
{
    NetPlayer* player = GetPlayer(data->id);
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
    NetPlayer* players = GetAllPlayers();
    uint16_t playercount = GetPlayerCount();

    // for every player
    for (size_t i = 0; i < playercount; i++)
    {
        // Get the player
        NetPlayer* player = &players[i];

        NetPlayerUpdate();
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
    uint16_t playercount = GetPlayerCount();
    if(playercount == CR_MAX_PLAYERS)
    {
        LogInfo("MAXIMUM PLAYERS REACHED, REFUSING CONNECTION\n");
        char* reason = "SERVER IS FULL";
        SendTCPMessage(incomingSocket, reason, strlen(reason)+1);
        SDLNet_TCP_Close(incomingSocket);
    }
    else {
        // Send ok confirmation
        char* reason = "OK";
        SendTCPMessage(incomingSocket, reason, strlen(reason)+1);

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
        int datacount = GetAllPlayerData(data);
        if(!SendTCPMessageArray(incomingSocket, data, sizeof(PlayerData), datacount))
        {
            LogInfo("COULD NOT SEND ALL PLAYER DATA");
            abort();
        }
        freea(data);

        // Register and send player
        NetPlayer* connectingplayer = InitPlayer(incomingSocket, udpPort);
        if(!SendTCPMessage(incomingSocket, &connectingplayer->data, sizeof(PlayerData)))
        {
            LogInfo("COULD NOT SEND PLAYER DATA");
            abort();
        }

        // Send connecting player event to all players
        NetPlayer* players = GetAllPlayers();
        for (size_t i = 0; i < playercount; i++)
        {
            NetPlayer* player = &players[i];

            // Skip sending event to the connecting player himself
            if(player->tcpSocket == connectingplayer->tcpSocket)
                continue;

            // Send event
            NetEventPlayerConnected ev;
            ev.data = connectingplayer->data;
            if(!NetEventPlayerConnectedSend(player->tcpSocket, &ev))
            {
                // disconnect player
            }
        }

        LogInfo("Player connected, ID: %d\n", connectingplayer->data.id);
    }
}

void ServerBroadcastPlayerData()
{
    NetPlayer* players = GetAllPlayers();
    uint16_t playercount = GetPlayerCount();
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

void ServerReadUpdates()
{
    static UDPpacket* recvpacket = NULL;

    Network* net = GetNetwork();
    if(recvpacket == NULL)
    {
        recvpacket = SDLNet_AllocPacket(1024);
    }

    // Read updates from players
    if(SDLNet_CheckSockets(net->udpSocketSet, 0))
    {
        while(SDLNet_UDP_Recv(net->udpSocket, recvpacket))
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
                LogInfo("UNHANDLE DATA ID %d", dataid);
                break;
            }
        }
    }
}
