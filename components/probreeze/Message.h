#pragma once

#include <string>

#define MAX_SERIAL_MESSAGE_SIZE 6

namespace esphome {
namespace probreeze {

class Message {
  private:
    uint8_t size;
    uint8_t data[MAX_SERIAL_MESSAGE_SIZE - 2];
    uint8_t checksum;

    uint8_t calculateChecksum();
  
  public:
    // Structure to hold the raw message and its size
    struct RawMessage {
        uint8_t bytes[MAX_SERIAL_MESSAGE_SIZE];
        size_t totalSize;
    };

    Message(RawMessage rawMessage);
    Message(uint8_t *inputData, size_t inputDataSize);

    // Function to return the full message as raw bytes
    RawMessage rawMessage();

    // Function to return a hex string representation of the raw message
    std::string toHexString();
};

}}