#pragma once

#ifndef CLUSTER_H
#define CLUSTER_H

#include <utility>
#include <vector>
#include "effects/effect.h"
#include <memory>
#include "section.h"

class EffectConfig;

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
public:
    const Scope scope;
    const std::vector<Section> scopeSections;
    const Section clusterSection;

    Cluster(std::vector<Section> sections, Scope scope);
};

#endif //CLUSTER_H