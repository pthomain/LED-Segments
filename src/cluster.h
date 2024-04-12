#pragma once

#ifndef CLUSTER_H
#define CLUSTER_H

#include <utility>
#include <vector>
#include <functional> // Include for std::function
#include <FastLED.h>
#include "effects/effect.h"
#include <memory>
#include "section.h"

const std::vector<std::pair<Scope, PixelUnit>> variations = {
        {SCOPE_WHOLE,  UNIT_WORD},
        {SCOPE_WHOLE,  UNIT_LETTER},
        {SCOPE_WHOLE,  UNIT_PIXEL},
        {SCOPE_WORD,   UNIT_LETTER},
        {SCOPE_WORD,   UNIT_PIXEL},
        {SCOPE_LETTER, UNIT_PIXEL}
};

const std::vector<Mirror> mirrors = {
        MIRROR_NONE,
        MIRROR_CENTRE,
        MIRROR_EDGE
};

class Cluster {
private:
    const Scope scope;

    const std::vector<Section> scopeSections;
    const std::vector<Section> emptySections = std::vector<Section>();

    std::vector<std::pair<Effect *, std::vector<Section>>> effectPerSectionPixels =
            std::vector<std::pair<Effect *, std::vector<Section>>>();

    static std::vector<Section> intersectAllPixelsWithClusterScope(
            const Section &clusterSection,
            const std::vector<Section> &pixelSections
    );

public:

    const Section clusterSection;

    Cluster(std::vector<Section> sections, Scope scope);

    void changeEffect(
            const std::function<Effect *(const Modifier *)> &effectFactory,
            const std::function<Modifier *(const Section &, const Mirror)> &modifierFactory,
            const Cluster *pixelUnits,
            const Mirror mirror
    );

    void render(CRGB *targetArray, CRGB *bufferArray);
};

#endif //CLUSTER_H