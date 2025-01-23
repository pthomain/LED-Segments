#ifndef LED_SEGMENTS_FIBONACCIENUMS_H
#define LED_SEGMENTS_FIBONACCIENUMS_H

const uint8_t LEDS_IN_SPIRAL = 27;
const uint8_t PIXELS_IN_SPIRAL = 20;
const uint8_t NB_SPIRAL_SEGMENTS = 12;
const uint8_t NB_CONCENTRIC_SEGMENTS = 24;

const uint8_t PIXEL_UNIT_MASK = 0b00000001;
const uint8_t DIRECTION_MASK = 0b00000010;
const uint8_t ALIGNMENT_MASK = 0b00000100;
const uint8_t INFLEXION_MASK = 0b00011000;

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
    CONCENTRIC
};

enum Inflexion {
    INFLEXION_NONE,
    INFLEXION_AT_20,
    INFLEXION_DYNAMIC,
    INFLEXION_JAGGED
};

static PixelUnit getPixelUnit(uint8_t layout) {
    return (layout & PIXEL_UNIT_MASK) ? SEGMENT : PIXEL;
}

static Direction getDirection(uint8_t layout) {
    return (layout & DIRECTION_MASK) ? COUNTER_CLOCKWISE : CLOCKWISE;
}

static Alignment getAlignment(uint8_t layout) {
    return (layout & ALIGNMENT_MASK) ? CONCENTRIC : SPIRAL;
}

static Inflexion getInflexion(uint8_t layout) {
    return static_cast<Inflexion>((layout & INFLEXION_MASK) >> 3);
}

static uint8_t getLayout(
    PixelUnit pixelUnit,
    Direction direction,
    Alignment alignment,
    Inflexion inflexion
) {
    uint8_t pixelUnitValue = (pixelUnit == PIXEL ? 0 : 1);
    uint8_t directionValue = (direction == CLOCKWISE ? 0 : 1) << 1;
    uint8_t alignmentValue = (alignment == SPIRAL ? 0 : 1) << 2;
    uint8_t inflexionValue = static_cast<uint8_t>(inflexion) << 3;

    return inflexionValue | alignmentValue | directionValue | pixelUnitValue;
}

static String getLayoutName(const uint16_t layoutIndex) {
    auto pixelUnit = getPixelUnit(layoutIndex);
    auto direction = getDirection(layoutIndex);
    auto alignment = getAlignment(layoutIndex);
    auto inflexion = getInflexion(layoutIndex);

    String inflexionName;

    switch (inflexion) {
        case INFLEXION_NONE: inflexionName = "NO_INFLEXION";
        break;
        case INFLEXION_AT_20: inflexionName = "INFLEXION_AT_20";
        break;
        case INFLEXION_DYNAMIC: inflexionName = "DYNAMIC_INFLEXION";
        break;
        default: inflexionName = "JAGGED_INFLEXION";
        break;
    }

    return String(pixelUnit == PIXEL ? "PIXEL_" : "SEGMENT_")
           + (alignment == SPIRAL ? "SPIRAL_" : "CONCENTRIC_")
           + (direction == CLOCKWISE ? "CW_" : "CCW_")
           + inflexionName;
}

#endif //LED_SEGMENTS_FIBONACCIENUMS_H