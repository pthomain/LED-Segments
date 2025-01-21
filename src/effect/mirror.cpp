#include "effect/mirror.h"
#include <string>

std::string mirrorToString(Mirror mirror) {
    switch (mirror) {
        case MIRROR_NONE:
            return "MIRROR_NONE";
        case MIRROR_CENTRE:
            return "MIRROR_CENTRE";
        case MIRROR_EDGE:
            return "MIRROR_EDGE";
        default:
            return "UNKNOWN_MIRROR";
    }
}
