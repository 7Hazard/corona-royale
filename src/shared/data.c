#include "data.h"
#include "network.h"
#include "log.h"
#include "memory.h"

DataID GetDataID_UDP(UDPpacket* packet)
{
    return packet->data[0];
}

void SendPositionData_UDP(PositionData* data)
{
    static const size_t size = sizeof(PositionData);
    static const uint8_t dataid = CR_DATA_POSITION;

    UDPpacket packet;

    // 1 byte for data id, rest for size of struct
    uint8_t* buffer = alloca(1 + size);
    buffer[0] = dataid;
    memcpy(&buffer[1], data, size);

    packet.data = buffer;
    packet.len = size;

    SendUDPPacket(&packet);
}

PositionData* GetPositionData_UDP(UDPpacket* packet)
{
    static const uint8_t dataid = CR_DATA_POSITION;

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
