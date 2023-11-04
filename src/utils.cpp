#include <utils.h>

void printNumber(char text[], int value) {
    auto string = std::string(text);
    string.append(": ");
    string.append(std::to_string(value));
    Serial.println(string.data());
}
