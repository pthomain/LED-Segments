//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain

/*
 * This file is part of LED Segments.
 *
 * LED Segments is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LED Segments is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LED Segments. If not, see <https://www.gnu.org/licenses/>.
 */

#include "Mirror.h"
#include <functional>
#include <memory>
#include "engine/effect/BaseEffect.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Blending.h"

template<typename C>
void applyMirror(
    const std::shared_ptr<BaseEffect<C> > &effect,
    Mirror mirror,
    C *effectArray,
    uint16_t effectArraySize
) {
    if (mirror == Mirror::NONE) return;

    const uint16_t mirrorSize = getMirrorSize(mirror, effectArraySize);
    const uint16_t offset = effectArraySize % 2 == 0 ? 1 : 0;
    const uint16_t centre = mirrorSize - 1;

    auto reverse = [&](const std::function<void(uint16_t x)> &extraStep = nullptr) {
        for (uint16_t index = 0; index < mirrorSize / 2; ++index) {
            uint16_t reverseIndex = mirrorSize - 1 - index;

            auto temp = effectArray[index];
            effectArray[index] = effectArray[reverseIndex];
            effectArray[reverseIndex] = temp;

            if (extraStep) extraStep(index);
        }
    };

    switch (mirror) {
        case Mirror::REVERSE:
            reverse();
            break;

        case Mirror::CENTRE:
            for (uint16_t index = 0; index < mirrorSize; ++index) {
                effectArray[centre + index + offset] = effectArray[centre - index];
            }
            break;

        case Mirror::EDGE: {
            for (uint16_t index = 0; index < mirrorSize; ++index) {
                effectArray[centre + index + offset] = effectArray[index];
                effectArray[centre - index] = effectArray[centre + index + offset];
            }
        }
        break;

        case Mirror::REPEAT:
            for (uint16_t index = 0; index < mirrorSize; ++index) {
                effectArray[centre + index + offset] = effectArray[index];
            }
            break;

        case Mirror::REPEAT_REVERSE: {
            auto extraStep = [&](uint16_t index) {
                effectArray[mirrorSize + index] = effectArray[index];
            };
            reverse(extraStep);
            for (uint16_t index = mirrorSize / 2; index < mirrorSize; ++index) {
                extraStep(index);
            }
        }
        break;

        default: {
            auto effectOperation = effect->effectOperation();
            bool isOverlay = effectOperation == EffectOperation::OVERLAY_SCREEN
                             || effectOperation == EffectOperation::OVERLAY_MULTIPLY
                             || effectOperation == EffectOperation::OVERLAY_INVERT;

            if (!isOverlay) return;
        }
    }

    auto overlayRepeat = [&](uint16_t x) {
        C *originalEffectArray = new C[mirrorSize];
        std::copy(effectArray, effectArray + mirrorSize, originalEffectArray);

        for (uint16_t repetition = 0; repetition < x; ++repetition) {
            int repetitionOffset = repetition * mirrorSize / x;
            for (uint16_t index = 0; index < mirrorSize; ++index) {
                int targetIndex = (repetitionOffset + index) % mirrorSize;
                effectArray[targetIndex] = mix(
                    effectArray[targetIndex],
                    originalEffectArray[index],
                    mixOperation(EffectOperation::OVERLAY_SCREEN)
                );
            }
        }

        delete[] originalEffectArray;
    };

    auto overlayRepeatReverse = [&](uint16_t x) {
        overlayRepeat(x);

        C *forwardEffectArray = new C[mirrorSize];
        std::copy(effectArray, effectArray + mirrorSize, forwardEffectArray);

        reverse();

        for (uint16_t index = 0; index < mirrorSize; ++index) {
            effectArray[index] = mix(
                effectArray[index],
                forwardEffectArray[index],
                mixOperation(EffectOperation::OVERLAY_SCREEN)
            );
        }

        delete[] forwardEffectArray;
    };

    switch (mirror) {
        case Mirror::OVERLAY_REPEAT_2:
            overlayRepeat(2);
            break;
        case Mirror::OVERLAY_REPEAT_3:
            overlayRepeat(3);
            break;
        case Mirror::OVERLAY_REPEAT_4:
            overlayRepeat(4);
            break;
        case Mirror::OVERLAY_REPEAT_5:
            overlayRepeat(5);
            break;
        case Mirror::OVERLAY_REPEAT_6:
            overlayRepeat(6);
            break;
        case Mirror::OVERLAY_REVERSE:
            overlayRepeatReverse(1);
            break;
        case Mirror::OVERLAY_REPEAT_2_REVERSE:
            overlayRepeatReverse(2);
            break;
        case Mirror::OVERLAY_REPEAT_3_REVERSE:
            overlayRepeatReverse(3);
            break;
        case Mirror::OVERLAY_REPEAT_4_REVERSE:
            overlayRepeatReverse(4);
            break;
        case Mirror::OVERLAY_REPEAT_5_REVERSE:
            overlayRepeatReverse(5);
            break;
        case Mirror::OVERLAY_REPEAT_6_REVERSE:
            overlayRepeatReverse(6);
            break;
    }
}

uint16_t getMirrorSize(
    Mirror mirror,
    uint16_t effectArraySize
) {
    switch (mirror) {
        case Mirror::CENTRE:
        case Mirror::EDGE:
        case Mirror::REPEAT:
        case Mirror::REPEAT_REVERSE:
            return (effectArraySize / 2) + (effectArraySize % 2 == 0 ? 0 : 1);

        default: return effectArraySize;
    }
}

EffectOperation mixOperation(EffectOperation operation) {
    return EffectOperation::OVERLAY_SCREEN;
}

String getMirrorName(Mirror mirror) {
    switch (mirror) {
        case Mirror::NONE:
            return "NONE";
        case Mirror::REVERSE:
            return "REVERSE";
        case Mirror::CENTRE:
            return "CENTRE";
        case Mirror::EDGE:
            return "EDGE";
        case Mirror::REPEAT:
            return "REPEAT";
        case Mirror::REPEAT_REVERSE:
            return "REPEAT_REVERSE";
        case Mirror::OVERLAY_REPEAT_2:
            return "OVERLAY_REPEAT_2";
        case Mirror::OVERLAY_REPEAT_3:
            return "OVERLAY_REPEAT_3";
        case Mirror::OVERLAY_REPEAT_4:
            return "OVERLAY_REPEAT_4";
        case Mirror::OVERLAY_REPEAT_5:
            return "OVERLAY_REPEAT_5";
        case Mirror::OVERLAY_REPEAT_6:
            return "OVERLAY_REPEAT_6";
        case Mirror::OVERLAY_REVERSE:
            return "OVERLAY_REVERSE";
        case Mirror::OVERLAY_REPEAT_2_REVERSE:
            return "OVERLAY_REPEAT_2_REVERSE";
        case Mirror::OVERLAY_REPEAT_3_REVERSE:
            return "OVERLAY_REPEAT_3_REVERSE";
        case Mirror::OVERLAY_REPEAT_4_REVERSE:
            return "OVERLAY_REPEAT_4_REVERSE";
        case Mirror::OVERLAY_REPEAT_5_REVERSE:
            return "OVERLAY_REPEAT_5_REVERSE";
        case Mirror::OVERLAY_REPEAT_6_REVERSE:
            return "OVERLAY_REPEAT_6_REVERSE";
        default:
            return "UNKNOWN";
    }
}

template
void applyMirror(
    const std::shared_ptr<BaseEffect<CRGB> > &effect,
    Mirror mirror,
    CRGB *effectArray,
    uint16_t effectArraySize
);

template
void applyMirror(
    const std::shared_ptr<BaseEffect<uint8_t> > &effect,
    Mirror mirror,
    uint8_t *effectArray,
    uint16_t effectArraySize
);
