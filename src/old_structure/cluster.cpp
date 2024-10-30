#include "cluster.h"
#include "utils/utils.h"
#include <utility>
#include <vector>

Cluster::Cluster(std::vector<Section> sections, Scope scope)
        : scope(scope),
          scopeSections(std::move(sections)),
          clusterSection(
                  Section(
                          scopeSections.at(0).start,
                          scopeSections.at(scopeSections.size() - 1).end
                  )
          ) {}
