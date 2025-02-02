#ifndef LED_SEGMENTS_FIBONACCIENUMS_H
#define LED_SEGMENTS_FIBONACCIENUMS_H

#include "vector"

const uint16_t TOTAL_FIBONACCI_LEDS = 324;
const uint8_t NB_LEDS_IN_SPIRAL = 27;
const uint8_t NB_SPIRAL_PIXELS = 20;
const uint8_t NB_RADIAL_PIXELS = 10;
const uint8_t NB_SPIRAL_SEGMENTS = 12;
const uint8_t NB_RADIAL_SEGMENTS = 24;

const uint8_t PIXEL_UNIT_MASK = 0b00000001;
const uint8_t DIRECTION_MASK = 0b00000010;
const uint8_t ALIGNMENT_MASK = 0b00000100;
const uint8_t INFLEXION_MASK = 0b00011000;
const uint8_t SEGMENT_MODULO_MASK = 0b11100000;

enum PixelUnit {
    PIXEL,
    SEGMENT
};

enum Direction {
    CLOCKWISE,
    COUNTER_CLOCKWISE
};

enum Alignment {
    SPIRAL,
    RADIAL
};

enum Inflexion {
    INFLEXION_NONE,
    INFLEXION_AT_20,
};

enum SegmentModulo {
    MODULO_0,
    MODULO_2,
    MODULO_3,
    MODULO_4,
    MODULO_6,
    MODULO_12
};

static PixelUnit getPixelUnit(uint8_t layout) {
    return (layout & PIXEL_UNIT_MASK) ? SEGMENT : PIXEL;
}

static Direction getDirection(uint8_t layout) {
    return (layout & DIRECTION_MASK) ? COUNTER_CLOCKWISE : CLOCKWISE;
}

static Alignment getAlignment(uint8_t layout) {
    return (layout & ALIGNMENT_MASK) ? RADIAL : SPIRAL;
}

static Inflexion getInflexion(uint8_t layout) {
    return static_cast<Inflexion>((layout & INFLEXION_MASK) >> 3);
}

static SegmentModulo getSegmentModulo(uint8_t layout) {
    return static_cast<SegmentModulo>((layout & SEGMENT_MODULO_MASK) >> 5);
}

static uint8_t getLayout(
        PixelUnit pixelUnit,
        Direction direction,
        Alignment alignment,
        Inflexion inflexion,
        SegmentModulo modulo
) {
    uint8_t pixelUnitValue = (pixelUnit == PIXEL ? 0 : 1);
    uint8_t directionValue = (direction == CLOCKWISE ? 0 : 1) << 1;
    uint8_t alignmentValue = (alignment == SPIRAL ? 0 : 1) << 2;
    uint8_t inflexionValue = static_cast<uint8_t>(inflexion) << 3;
    uint8_t moduloValue = static_cast<uint8_t>(modulo) << 5;

    return inflexionValue | alignmentValue | directionValue | pixelUnitValue | moduloValue;
}

static String getLayoutName(const uint16_t variation) {
    auto pixelUnit = getPixelUnit(variation);
    auto direction = getDirection(variation);
    auto alignment = getAlignment(variation);
    auto inflexion = getInflexion(variation);
    auto modulo = getSegmentModulo(variation);

    String inflexionName = inflexion == INFLEXION_AT_20 ? "INFLEXION_AT_20" : "";

    String segmentModulo;

    switch (modulo) {
        case MODULO_0:
        default:
            segmentModulo = "MODULO_0";
            break;
        case MODULO_2:
            segmentModulo = "MODULO_2";
            break;
        case MODULO_3:
            segmentModulo = "MODULO_3";
            break;
        case MODULO_4:
            segmentModulo = "MODULO_4";
            break;
        case MODULO_6:
            segmentModulo = "MODULO_6";
            break;
        case MODULO_12:
            segmentModulo = "MODULO_12";
            break;
    }

    return String(pixelUnit == PIXEL ? "PIXEL" : "SEGMENT")
           + (alignment == SPIRAL ? (direction == CLOCKWISE ? "_CW_" : "_CCW_") + inflexionName : "_RADIAL")
           + (pixelUnit == PIXEL ? "" : "_" + segmentModulo);
}

static std::vector<uint8_t> computeVariations() {
    auto variations = std::vector<uint8_t>();

    auto addVariation = [&](
            uint8_t pixelUnit,
            uint8_t direction,
            uint8_t alignment,
            uint8_t inflexion,
            uint8_t modulo
    ) {
        variations.push_back(getLayout(
                static_cast<PixelUnit>(pixelUnit),
                static_cast<Direction>(direction),
                static_cast<Alignment>(alignment),
                static_cast<Inflexion>(inflexion),
                static_cast<SegmentModulo>(modulo)
        ));
        Serial.println(String(variations.size()) + ": " + getLayoutName(variations.at(variations.size() - 1)));
    };

    auto addVariationsWithModulo = [&](uint8_t pixelUnit, uint8_t modulo) {
        for (uint8_t alignment = 0; alignment < 2; alignment++) {
            if (alignment == RADIAL) {
                //direction and inflexion don't apply to radial
                addVariation(pixelUnit, CLOCKWISE, alignment, INFLEXION_NONE, modulo);
            } else {
                for (uint8_t inflexion = 0; inflexion < 2; inflexion++) {
                    for (uint8_t direction = 0; direction < 2; direction++) {
                        addVariation(pixelUnit, direction, alignment, inflexion, modulo);
                    }
                }
            }
        }
    };

    for (uint8_t pixelUnit = 0; pixelUnit < 2; pixelUnit++) {
        if (pixelUnit == PIXEL) {
            addVariationsWithModulo(pixelUnit, MODULO_0);
        } else {
            for (uint8_t modulo = 0; modulo < 6; modulo++) {
                addVariationsWithModulo(pixelUnit, modulo);
            }
        }
    }

    return variations;
}

#endif //LED_SEGMENTS_FIBONACCIENUMS_H