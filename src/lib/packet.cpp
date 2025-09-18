#include "packet.h"
#include "tools.h"

// Constructor
Packet::Packet(uint32_t size, uint16_t id, const std::string& data)
    : fullSize(size), packetId(id), content(data) {}

Packet::Packet(int client_socket) {
    // Read packet length
    fullSize = readVarInt(client_socket);
    // Read packet ID
    packetId = readVarInt(client_socket);
    // Set a default content (could be extended to read actual content)
    char buf[256] = {0};
    ssize_t bytesReceived = recvAll(client_socket, buf, fullSize - (packetId >= 0x80 ? 2 : 1));
    if (bytesReceived > 0) {
        content = std::string(buf, bytesReceived);
    } else {    
        content = "";
    }

    display();
}

Packet::Packet(uint16_t id, const std::string& data)
    : packetId(id), content(data) {
    fullSize = static_cast<uint32_t>(data.size() + (id >= 0x80 ? 2 : 1));
}

// Getter for fullSize
uint32_t Packet::getFullSize() const {
    return fullSize;
}

// Getter for packetId
uint16_t Packet::getPacketId() const {
    return packetId;
}

// Getter for content
const std::string& Packet::getContent() const {
    return content;
}

// Setter for fullSize
void Packet::setFullSize(uint32_t size) {
    fullSize = size;
}

// Setter for packetId
void Packet::setPacketId(uint16_t id) {
    packetId = id;
}

// Setter for content
void Packet::setContent(const std::string& data) {
    content = data;
}

// Display packet details
void Packet::display() const {
    std::cout << "Packet ID: " << packetId << ", Full Size: " << fullSize << " bytes, Content: " << content << std::endl;
    for (size_t i = 0; i < fullSize; i++)
        std::cout << (static_cast<int>(content[i])) << " ";
    std::cout << std::endl;    
}

ssize_t Packet::sendPacket(int socket) {
    // Prepare buffer
    std::string buffer;
    
    // Encode fullSize as VarInt
    uint32_t size = fullSize;
    do {
        uint8_t temp = size & 0b01111111;
        size >>= 7;
        if (size != 0) {
            temp |= 0b10000000;
        }
        buffer.push_back(static_cast<char>(temp));
    } while (size != 0);
    
    // Encode packetId as VarInt
    size = packetId;
    do {
        uint8_t temp = size & 0b01111111;
        size >>= 7;
        if (size != 0) {
            temp |= 0b10000000;
        }
        buffer.push_back(static_cast<char>(temp));
    } while (size != 0);
    
    // Append content
    buffer += content;

    // Debug output
    // for (size_t i = 0; i < buffer.length(); i++)
    // {
    //     std::cout << (static_cast<int>(buffer[i])) << " ";
    // }
    
    // std::cout << "//" << std::endl;

    std::cout << "Sending packet ID: " << packetId << ", Full Size: " << fullSize << " bytes" << std::endl;

    // Send the packet
    return send(socket, buffer.data(), buffer.size(), 0);
}

void Packet::resetContentPos() {
    contentPos = 0;
}

std::string Packet::readString(int length) {
    size_t originalPos = contentPos;

    if (length == 0) {
        if (contentPos >= content.size()) {
            return ""; // No more data to read
        }

        // Read length of the string (VarInt)
        length = content[contentPos++] & 0b01111111;
        uint8_t read;
    }

    // Read the string data
    if (contentPos + length > content.size()) {
        contentPos = originalPos; // Reset position on error
        return ""; // Error: not enough data for the string
    }

    std::string result = content.substr(contentPos, length);
    contentPos += length; // Move position forward

    return result;
}

std::string Packet::readUUID() {
    return readString(16);
}