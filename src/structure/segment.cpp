#include "segment.h"

Segment::Segment(
        const std::vector<std::vector<uint16_t>> pixels
) : size(pixels.size()) {
    this->pixels = std::vector<BitArray>(pixels.size());

    for (auto &pixel: pixels) {
        uint16_t maxIndex = 0;

        for (auto &pixelIndex: pixel) {
            maxIndex = max(maxIndex, pixelIndex);
        }

        auto pixelArray = BitArray();
        auto result = pixelArray.begin(log2(maxIndex), pixel.size());
        if (result == BA_NO_MEMORY_ERR) Serial.println("no more memory");

        uint16_t pixelArrayIndex = 0;
        for (auto &pixelIndex: pixel) {
            pixelArray.set(pixelArrayIndex++, pixelIndex);
        }

        this->pixels.push_back(pixelArray);

        Serial.print("CAPACITY:\t");
        Serial.println(pixelArray.capacity());
        Serial.print("  MEMORY:\t");
        Serial.println(pixelArray.memory());
        Serial.print("    BITS:\t");
        Serial.println(pixelArray.bits());
        Serial.print("SEGMENTS:\t");
        Serial.println(pixelArray.segments());
        Serial.println("---");
    }
}

void Segment::map(CRGB *effectArray, CRGB *outputArray) {
    for (uint16_t pixelIndex = 0; pixelIndex < size; pixelIndex++) {
        CRGB pixelColour = effectArray[pixelIndex];
        BitArray pixel = pixels.at(pixelIndex);

        for (uint16_t ledIndex = 0; ledIndex < pixel.segments(); ledIndex++) {
            outputArray[pixel.get(ledIndex)] = pixelColour;
        }
    }
}