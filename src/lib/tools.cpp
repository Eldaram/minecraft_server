#include "tools.h"

// Function to receive all data from the client
ssize_t recvAll(int socket, char* buffer, size_t length) {
    ssize_t totalBytesReceived = 0;
    while (totalBytesReceived < length) {
        ssize_t bytesReceived = recv(socket, buffer + totalBytesReceived, length - totalBytesReceived, 0);
        if (bytesReceived > 0) {
            totalBytesReceived += bytesReceived;
        }
    }
    return totalBytesReceived;
}

int readVarInt(int socket) {
    int numRead = 0;
    int result = 0;
    uint8_t read;
    do {
        ssize_t bytesReceived = recv(socket, &read, 1, 0);
        if (bytesReceived <= 0) {
            // Handle error or disconnection
            return -1; // Indicate an error
        }
        int value = (read & 0b01111111);
        result |= (value << (7 * numRead));

        numRead++;
        if (numRead > 5) {
            // VarInt is too big
            return -1; // Indicate an error
        }
    } while ((read & 0b10000000) != 0);

    return result;
}

char getHeaderBufferSize(std::string data)
{
    return static_cast<char>(data.size());
}

std::string parseUUID(const std::string& uuid) {
    std::stringstream stream;
    for (char c : uuid) {
        stream << (c < 16 ? "0" : "") << std::hex << (static_cast<int>(c));
    }
    return stream.str();
}

Json::Value getPlayerDetailsFromMojangAPI(const std::string& uuid) {
    try {
        curlpp::Cleanup cleanup;
        curlpp::Easy request;

        std::string url = "https://sessionserver.mojang.com/session/minecraft/profile/" + uuid;
        request.setOpt(new curlpp::options::Url(url));
        request.setOpt(new curlpp::options::FollowLocation(true));

        std::ostringstream responseStream;
        request.setOpt(new curlpp::options::WriteStream(&responseStream));

        request.perform();

        std::string response = responseStream.str();
        Json::CharReaderBuilder readerBuilder;
        Json::Value jsonData;
        std::string errs;

        std::istringstream responseData(response);
        if (Json::parseFromStream(readerBuilder, responseData, &jsonData, &errs)) {
            return jsonData;
        } else {
            std::cerr << "Failed to parse JSON: " << errs << std::endl;
            return Json::Value(); // Return an empty Json::Value on failure
        }
    } catch (const curlpp::RuntimeError& e) {
        std::cerr << "cURLpp Runtime Error: " << e.what() << std::endl;
    } catch (const curlpp::LogicError& e) {
        std::cerr << "cURLpp Logic Error: " << e.what() << std::endl;
    }
    return Json::Value(); // Return an empty Json::Value on exception
}