#ifndef LED_SEGMENTS_FIBONACCIENUMS_H
#define LED_SEGMENTS_FIBONACCIENUMS_H

const uint8_t LEDS_IN_SPIRAL = 27;
const uint8_t PIXELS_IN_SPIRAL = 20;
const uint8_t NB_SPIRAL_SEGMENTS = 12;
const uint8_t NB_CONCENTRIC_SEGMENTS = 24;

enum FibonacciLayout {
    PIXEL_IN_SPIRAL,
    SEGMENT_IN_SPIRAL,
    PIXEL_IN_SPIRAL_SYMMETRIC,
    SEGMENT_IN_SPIRAL_SYMMETRIC,
    SEGMENT_IN_CONCENTRIC,
    PIXEL_IN_CONCENTRIC,

    PIXEL_IN_SPIRAL_INFLEXION_20,
    PIXEL_IN_SPIRAL_INFLEXION_DYNAMIC,
    PIXEL_IN_SPIRAL_INFLEXION_ODD_EVEN,

    SEGMENT_IN_SPIRAL_INFLEXION_20,
    SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC,
    SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN,
};

enum Alignment {
    SPIRAL,
    SYMMETRIC,
    CONCENTRIC
};

enum PixelUnit {
    PIXEL,
    SEGMENT
};

static Alignment getAlignment(uint8_t layout){
    switch (layout) {
        case PIXEL_IN_SPIRAL:
        case SEGMENT_IN_SPIRAL:
        case PIXEL_IN_SPIRAL_INFLEXION_20:
        case PIXEL_IN_SPIRAL_INFLEXION_DYNAMIC:
        case PIXEL_IN_SPIRAL_INFLEXION_ODD_EVEN:
        case SEGMENT_IN_SPIRAL_INFLEXION_20:
        case SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC:
        case SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN:
            return SPIRAL;

        case PIXEL_IN_SPIRAL_SYMMETRIC:
        case SEGMENT_IN_SPIRAL_SYMMETRIC:
            return SYMMETRIC;

        case SEGMENT_IN_CONCENTRIC:
        case PIXEL_IN_CONCENTRIC:
            return CONCENTRIC;
    }
}

static PixelUnit getPixelUnit(uint16_t layout){
    switch (layout) {
        case PIXEL_IN_SPIRAL:
        case PIXEL_IN_SPIRAL_INFLEXION_20:
        case PIXEL_IN_SPIRAL_INFLEXION_DYNAMIC:
        case PIXEL_IN_SPIRAL_INFLEXION_ODD_EVEN:
        case PIXEL_IN_SPIRAL_SYMMETRIC:
        case PIXEL_IN_CONCENTRIC:
            return PIXEL;

        case SEGMENT_IN_SPIRAL:
        case SEGMENT_IN_SPIRAL_INFLEXION_20:
        case SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC:
        case SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN:
        case SEGMENT_IN_SPIRAL_SYMMETRIC:
        case SEGMENT_IN_CONCENTRIC:
            return SEGMENT;
    }
}

static bool hasInflexionPoint(FibonacciLayout layout){
    switch (layout) {
        case PIXEL_IN_SPIRAL_INFLEXION_20:
        case PIXEL_IN_SPIRAL_INFLEXION_DYNAMIC:
        case PIXEL_IN_SPIRAL_INFLEXION_ODD_EVEN:
        case SEGMENT_IN_SPIRAL_INFLEXION_20:
        case SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC:
        case SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN:
            return true;

        default:
            return false;
    }
}

#endif //LED_SEGMENTS_FIBONACCIENUMS_H
