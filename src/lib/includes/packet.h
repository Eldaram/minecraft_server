#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <string>
#include <iostream>

class Packet {
private:
    uint32_t fullSize; // Full size of the packet
    uint16_t packetId; // Packet ID
    std::string content; // Content of the packet
    int contentPos = 0; // Current position in content for reading

public:
    // Constructor
    Packet(uint32_t size, uint16_t id, const std::string& data);
    Packet(int client_socket);
    Packet(uint16_t id, const std::string& data);

    // Getter for fullSize
    uint32_t getFullSize() const;

    // Getter for packetId
    uint16_t getPacketId() const;

    // Getter for content
    const std::string& getContent() const;

    // Setter for fullSize
    void setFullSize(uint32_t size);

    // Reset fullSize to match current content size
    void resetFullSize();

    // Setter for packetId
    void setPacketId(uint16_t id);

    // Setter for content
    void setContent(const std::string& data);

    // Display packet details
    void display() const;

    //reset current position in content
    void resetContentPos();

    //read functions
    //read string from content
    std::string readString(int length = 0);

    //read uuid from content
    //it return the UUID as the char decimal representation without dashes
    std::string readUUID();

    //write varint to content
    void writeVarInt(uint32_t value);

    //write string to content
    void writeString(const std::string& str, bool needLength = true);

    //write uuid to content
    //it expect the UUID as the char decimal representation without dashes
    void writeUUID(const std::string& uuid);

    //get the user properties from the api and write them to content
    bool writeUserProperties(const std::string& uuid);

    //Write prefixed array to content
    void writePrefixedArray(const std::string* arr, size_t rows, size_t cols);

    // Send packet
    ssize_t sendPacket(int socket);
};

#endif // PACKET_H