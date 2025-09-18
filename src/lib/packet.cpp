#include "packet.h"
#include "tools.h"

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

uint32_t Packet::getFullSize() const {
    return fullSize;
}

uint16_t Packet::getPacketId() const {
    return packetId;
}

const std::string& Packet::getContent() const {
    return content;
}

void Packet::setFullSize(uint32_t size) {
    fullSize = size;
}

void Packet::resetFullSize() {
    fullSize = static_cast<uint32_t>(content.size() + (packetId >= 0x80 ? 2 : 1));
}

void Packet::setPacketId(uint16_t id) {
    packetId = id;
}

void Packet::setContent(const std::string& data) {
    content = data;
}

void Packet::display() const {
    std::cout << "Packet ID: " << packetId << ", Full Size: " << fullSize << " bytes, Content: " << content << std::endl;
    for (size_t i = 0; i < fullSize; i++)
        std::cout << (static_cast<int>(content[i])) << " ";
    std::cout << std::endl;    
}

ssize_t Packet::sendPacket(int socket) {
    // Prepare buffer
    std::string buffer;

    resetFullSize();
    
    // Encode fullSize as VarInt
    writeVarIntTool(buffer, fullSize);
    
    // Encode packetId as VarInt
    writeVarIntTool(buffer, packetId);
    
    // Append content
    buffer += content;

    // Debug output
    for (size_t i = 0; i < buffer.length(); i++)
    {
        std::cout << (static_cast<int>(buffer[i])) << " ";
    }
    
    std::cout << "//" << std::endl;

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

void Packet::writeVarInt(uint32_t value) {
    writeVarIntTool(content, value);
}

void Packet::writeString(const std::string& str, bool needLength) {
    // Write length of the string as VarInt
    if (needLength) {
        uint32_t length = static_cast<uint32_t>(str.size());
        writeVarInt(length);
    }

    // Write the string data
    content += str;
}

void Packet::writeUUID(const std::string& uuid) {
    writeString(uuid, false);
}

bool Packet::writeUserProperties(const std::string& uuid) {
    Json::Value details = getPlayerDetailsFromMojangAPI(parseUUID(uuid));
    if (details.isNull() || !details.isObject() || !details.isMember("properties") || !details["properties"].isArray()) {
        std::cerr << "Failed to get valid user properties for UUID: " << uuid << std::endl;
        return false;
    }

    std::string name = details["properties"][0]["name"].asString();
    std::string value = details["properties"][0]["value"].asString();

    uint32_t length = static_cast<uint32_t>(name.size()) + static_cast<uint32_t>(value.size()) + 1;
    writeVarInt(length);
    writeString(name, true);
    writeString(value, true);
    content.push_back(0); // Add a byte for optional signature (false)

    return true;
}