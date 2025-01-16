#pragma once

#include <string>
#include <vector>

#define MAX_SERIAL_MESSAGE_SIZE 6

namespace esphome {
namespace probreeze {

class Message {
  private:
    uint8_t size;
    uint8_t checksum;

    uint8_t calculateChecksum();
  
  public:
    std::vector<std::uint8_t> data;
    
    Message(std::vector<std::uint8_t> &data);

    // Function to return the full message as raw bytes
    std::vector<std::uint8_t> rawMessage();

    // Function to return a hex string representation of the raw message
    // std::string toHexString();
};

}}