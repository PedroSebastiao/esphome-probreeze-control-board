#include "message.h"

#include <cstring>  // Include the header for memcpy

namespace esphome {
namespace probreeze {


// Constructor definition
Message::Message(std::vector<std::uint8_t> &data) {
    this->size = data.size() + 2;
    this->data = data;
    this->checksum = this->calculateChecksum();
}

uint8_t Message::calculateChecksum() {
  uint8_t checksum = this->size;
  for(auto byte: this->data) {
    checksum += byte;
  }
  return checksum;
}

// rawMessage function implementation
std::vector<std::uint8_t> Message::rawMessage() {

    // Create a new vector with enough space for all bytes
    std::vector<std::uint8_t> new_data;
    new_data.reserve(this->data.size() + 2);  // +1 for prepend, +1 for append

    // Add the prepend byte
    new_data.push_back(this->size);

    // Add the original data
    new_data.insert(new_data.end(), this->data.begin(), this->data.end());

    // Add the append byte
    new_data.push_back(this->checksum);

    return new_data;
}



// // Function to convert raw message to hex string
// std::string Message::toHexString() {
//     RawMessage rawMsg = rawMessage();  // Get the raw message
//     std::string hexString;

//     char buffer[3];  // To hold each byte as a hex string
//     for (size_t i = 0; i < rawMsg.totalSize; i++) {
//         sprintf(buffer, "%02X", rawMsg.bytes[i]);  // Convert byte to hex
//         hexString += buffer;  // Append to the hex string
//         if (i < rawMsg.totalSize - 1) {
//             hexString += " ";  // Add a space between bytes
//         }
//     }
//     return hexString;
// }

}
}