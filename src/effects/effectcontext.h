#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include "effects/mirror/mirror.h"

class EffectContext {

protected:
    const Mirror _mirror;
    const uint16_t _arraySize{};
    const uint16_t _position{};
    const uint16_t _iteration{};

public:
    EffectContext(
            const Mirror mirror,
            const uint16_t arraySize,
            const uint16_t position,
            const uint16_t iteration
    ) : _mirror(mirror),
        _arraySize(arraySize),
        _position(position),
        _iteration(iteration) {}

    const Mirror mirror() { return _mirror; }

    const uint16_t arraySize() { return _arraySize; }

    const uint16_t position() { return _position; }

    const uint16_t iteration() { return _iteration; }
};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
