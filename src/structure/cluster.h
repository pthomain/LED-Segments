#pragma once

#ifndef CLUSTER_H
#define CLUSTER_H

#include <utility>
#include <vector>
#include "effects/effect.h"
#include <memory>
#include "section.h"

class EffectConfig;

class Cluster {
public:
    const Scope scope;
    const std::vector<Section> scopeSections;
    const Section clusterSection;

    Cluster(std::vector<Section> sections, Scope scope);
};

#endif //CLUSTER_H