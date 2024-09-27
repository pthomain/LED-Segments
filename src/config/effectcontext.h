#ifndef EFFECTCONTEXT_H
#define EFFECTCONTEXT_H

class EffectContext {
public:
    const Section &section;
    const Scope scope;
    PixelUnit pixelUnit;
    const Mirror mirror;
    const uint8_t scopeMax;
    const uint8_t pixelUnitMax;
    const uint8_t scopeIndex;
    const uint16_t effectIteration;
    const int seed;

    EffectContext(
            const Section &section,
            const Scope scope,
            const Scope pixelUnitScope,
            const Mirror mirror,
            const uint8_t scopeMax,
            const uint8_t pixelUnitMax,
            const uint8_t scopeIndex,
            const uint16_t effectIteration,
            const int seed
    ) : section(section),
        scope(scope),
        mirror(mirror),
        scopeMax(scopeMax),
        pixelUnitMax(pixelUnitMax),
        scopeIndex(scopeIndex),
        effectIteration(effectIteration),
        seed(seed) {

        switch (pixelUnitScope) {
            case SCOPE_WORD:
                this->pixelUnit = UNIT_WORD;
                break;
            case SCOPE_LETTER:
                this->pixelUnit = UNIT_LETTER;
                break;
            default:
                this->pixelUnit = UNIT_PIXEL;
                break;
        }
    }
};

#endif //EFFECTCONTEXT_H