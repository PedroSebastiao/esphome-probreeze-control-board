#include "Message.h"

namespace esphome {
namespace probreeze {


// Constructor definition
Message::Message(RawMessage rawMessage) {
    size = rawMessage.bytes[0];
    memcpy(data, rawMessage.bytes + 1, rawMessage.totalSize - 2);
    checksum = rawMessage.bytes[rawMessage.totalSize - 1];
}

Message::Message(uint8_t *inputData, size_t inputDataSize) {
  if (inputDataSize > MAX_SERIAL_MESSAGE_SIZE - 2) {
      // Handle case where input size exceeds the maximum allowed size
      inputDataSize = MAX_SERIAL_MESSAGE_SIZE - 2;
  }
  size = inputDataSize + 2;  // Set the message size
  memcpy(data, inputData, inputDataSize);  // Copy input data into the class's data array

  checksum = size;
  for (uint8_t i = 0; i < inputDataSize; i++) {
      checksum += inputData[i];
  }
}

// rawMessage function implementation
Message::RawMessage Message::rawMessage() {
    RawMessage rawMsg;
    rawMsg.bytes[0] = size;  // First byte is the size
    memcpy(&rawMsg.bytes[1], data, size - 2);  // Copy the data
    rawMsg.bytes[size - 1] = checksum;  // Last byte is the checksum
    rawMsg.totalSize = size;  // Total size includes all bytes
    return rawMsg;
}

uint8_t Message::calculateChecksum() {
  uint8_t checksum = size;
  for (uint8_t i = 0; i < size - 2; i++) {
      checksum += data[i];
  }
  return checksum;
}



// Function to convert raw message to hex string
std::string Message::toHexString() {
    RawMessage rawMsg = rawMessage();  // Get the raw message
    std::string hexString;

    char buffer[3];  // To hold each byte as a hex string
    for (size_t i = 0; i < rawMsg.totalSize; i++) {
        sprintf(buffer, "%02X", rawMsg.bytes[i]);  // Convert byte to hex
        hexString += buffer;  // Append to the hex string
        if (i < rawMsg.totalSize - 1) {
            hexString += " ";  // Add a space between bytes
        }
    }
    return hexString;
}

}}