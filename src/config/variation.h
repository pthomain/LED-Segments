#ifndef LED_MATRIX_DLH_VARIATION_H
#define LED_MATRIX_DLH_VARIATION_H

#include <functional> // Include for std::function
#include <utility>
#include "effects/effect.h"
#include "modifiers/modifier.h"
#include "structure/cluster.h"

class Variation {
public:
    std::vector<std::pair<Scope, PixelUnit>> scopeAndUnits;
    std::vector<Mirror> mirrors;
    std::vector<std::function<Effect *(const EffectContext &effectContext)> *> modifierFactories;

    explicit Variation(
            std::vector<std::pair<Scope, PixelUnit>> variations,
            std::vector<Mirror> mirror,
            std::vector<std::function<Effect *(const EffectContext &effectContext)> *> modifierFactories
    ) : scopeAndUnits(std::move(std::move(variations))),
        mirrors(std::move(mirror)),
        modifierFactories(std::move(modifierFactories)) {};
};

const std::vector<std::pair<Scope, PixelUnit>> ALL_SCOPES = {
        std::make_pair(SCOPE_WHOLE, UNIT_WORD),
        std::make_pair(SCOPE_WHOLE, UNIT_LETTER),
        std::make_pair(SCOPE_WHOLE, UNIT_PIXEL),
        std::make_pair(SCOPE_WORD, UNIT_LETTER),
        std::make_pair(SCOPE_WORD, UNIT_PIXEL),
        std::make_pair(SCOPE_LETTER, UNIT_PIXEL),
};

const std::vector<std::pair<Scope, PixelUnit>> ALL_SCOPES_WEIGHTED = {
        std::make_pair(SCOPE_WHOLE, UNIT_WORD),
        std::make_pair(SCOPE_WHOLE, UNIT_LETTER),
        std::make_pair(SCOPE_WHOLE, UNIT_LETTER),
        std::make_pair(SCOPE_WHOLE, UNIT_LETTER),
        std::make_pair(SCOPE_WHOLE, UNIT_LETTER),
        std::make_pair(SCOPE_WHOLE, UNIT_LETTER),
        std::make_pair(SCOPE_WORD, UNIT_LETTER),
        std::make_pair(SCOPE_WORD, UNIT_LETTER),
        std::make_pair(SCOPE_WORD, UNIT_LETTER),
        std::make_pair(SCOPE_LETTER, UNIT_PIXEL),
};

const std::vector<Mirror> ALL_MIRRORS = {
        MIRROR_NONE,
        MIRROR_CENTRE,
        MIRROR_EDGE,
};

const std::vector<Mirror> ALL_MIRRORS_WEIGHTED = {
        MIRROR_NONE,
        MIRROR_CENTRE,
        MIRROR_EDGE,
};

#endif //LED_MATRIX_DLH_VARIATION_H
