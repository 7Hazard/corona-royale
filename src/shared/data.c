#include "data.h"

#include "network.h"
#include "log.h"
#include "memory.h"

DataID GetDataID_UDP(UDPpacket* packet)
{
    return packet->data[0];
}

#ifdef CR_CLIENT
void SendMovementData_UDP(PlayerMovementData* data)
#else
void SendMovementData_UDP(IPaddress* dest, PlayerMovementData* data)
#endif
{
    static const size_t datasize = sizeof(PlayerMovementData);
    static const size_t packetsize = 1 + sizeof(PlayerMovementData);
    static const uint8_t dataid = CR_DATA_MOVEMENT;

    UDPpacket packet;

    // 1 byte for data id, rest for size of struct
    uint8_t* buffer = alloca(packetsize);
    buffer[0] = dataid;
    memcpy(&buffer[1], data, datasize);

    packet.data = buffer;
    packet.len = packetsize;
    packet.maxlen = packetsize;

    Network* net = GetNetwork();

#ifdef CR_SERVER
    packet.address = *dest;
#else
    packet.address = net->address;
#endif

    if(!SendUDPPacket(&packet))
    {
        LogInfo("PACKET WAS NOT SENT | ERROR: %s\n", SDLNet_GetError());
    }
}

PlayerMovementData* GetMovementData_UDP(UDPpacket* packet)
{
    static const uint8_t dataid = CR_DATA_MOVEMENT;

    uint8_t packetDataID = packet->data[0];
    if(packetDataID != dataid)
    {
        LogInfo("DATA ID MISMATCH, PACKET ID WAS %d, ATTEMPTED TO READ AS %d\n", packetDataID, dataid);
        abort();
        return NULL;
    }
    else
    {
        // Actual data is 1 byte forward
        return packet->data + 1;
    }
}
