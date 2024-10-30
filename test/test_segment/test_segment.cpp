#include "unity.h"    // PlatformIO's test framework
#include "Segment.h"
#include "Pixel.h"

void givenSegmentHasNoChildren_thenThrowException() {
    // Single segment with no children, should throw an exception
    auto pixels = std::vector<Pixel *>();
    bool exceptionThrown = false;
    try {
        Segment segment = Segment(pixels);
    } catch (std::invalid_argument &e) {
        exceptionThrown = true;
        TEST_ASSERT_EQUAL_STRING("Segment must contain at least one Pixel", e.what());
    }
    TEST_ASSERT_TRUE(exceptionThrown);
}

void givenSegmentHasOnePixel_thenDepthIs1() {
    // Segment with one Pixel, depth should be 1
    auto pixels = std::vector < Pixel * > {new Pixel(0, 1)};
    Segment segment = Segment(pixels);
    TEST_ASSERT_EQUAL(1, segment.depth());
}

void givenSegmentHasThreePixels_thenDepthIs1() {
    // Segment with 3 Pixel, depth should be 1
    auto pixels = std::vector < Pixel * > {
            new Pixel(0, 1),
            new Pixel(2, 4),
            new Pixel(5, 8),
    };
    Segment segment = Segment(pixels);
    TEST_ASSERT_EQUAL(1, segment.depth());
}

void givenSegmentHasOneSubSegment_thenDepthIs2() {
    // Segment with 1 sub-Segment, depth should be 2
    Segment segment = Segment( //depth 0
            std::vector < Pixel * > {
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Pixel(0, 1) //depth 2
                            }
                    )
            }
    );

    TEST_ASSERT_EQUAL(2, segment.depth());
}

void givenSegmentHasTwoSubSegments_thenDepthIs3() {
    // Segment with 2 sub-Segments, depth should be 3
    Segment segment = Segment( //depth 0
            std::vector < Pixel * > {
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Segment(//depth 2
                                            std::vector < Pixel * > {
                                                    new Pixel(0, 1) //depth 3
                                            }
                                    )
                            }
                    )
            }
    );

    TEST_ASSERT_EQUAL(3, segment.depth());
}

void givenSegmentHasSeveralSubSegments_thenDepthIsShortestBranch() {
    // Segment with 2 branches (depth 4 and 3), depth should be 3 (shortest branch)
    Segment segment = Segment( //depth 0
            std::vector < Pixel * > {
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Segment(//depth 2
                                            std::vector < Pixel * > {
                                                    new Segment(//depth 3
                                                            std::vector < Pixel * > {
                                                                    new Pixel(0, 1) //depth 4
                                                            }
                                                    )
                                            }
                                    )
                            }
                    ),
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Segment(//depth 2
                                            std::vector < Pixel * > {
                                                    new Pixel(0, 1) //depth 3
                                            }
                                    )
                            }
                    )
            }
    );

    TEST_ASSERT_EQUAL(3, segment.depth());
}

void givenSegmentHasSeveralSubSegmentsInAlternateOrder_thenDepthIsShortestBranch() {
    // Segment with 2 branches (depth 4 and 3), depth should be 3 (shortest branch)
    Segment segment = Segment( //depth 0
            std::vector < Pixel * > {
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Segment(//depth 2
                                            std::vector < Pixel * > {
                                                    new Pixel(0, 1) //depth 3
                                            }
                                    )
                            }
                    ),
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Segment(//depth 2
                                            std::vector < Pixel * > {
                                                    new Segment(//depth 3
                                                            std::vector < Pixel * > {
                                                                    new Pixel(0, 1) //depth 4
                                                            }
                                                    )
                                            }
                                    )
                            }
                    )
            }
    );

    TEST_ASSERT_EQUAL(3, segment.depth());
}

void givenSegmentHasSeveralBranchesAndOneBranchHasAPixel_thenDepthStopsAtFirstPixel() {
    // Segment with 2 branches (depth 4 and 3) but branch with depth 4 has a Pixel at depth 2,
    // depth should be 2 (first encountered Pixel marks the bottom).
    Segment segment = Segment( //depth 0
            std::vector < Pixel * > {
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Segment(//depth 2
                                            std::vector < Pixel * > {
                                                    new Segment(//depth 3
                                                            std::vector < Pixel * > {
                                                                    new Pixel(0, 1) //depth 4
                                                            }
                                                    ),
                                                    new Pixel(0, 1) //depth 2
                                            }
                                    )
                            }
                    ),
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Segment(//depth 2
                                            std::vector < Pixel * > {
                                                    new Pixel(0, 1) //depth 3
                                            }
                                    )
                            }
                    )
            }
    );

    TEST_ASSERT_EQUAL(3, segment.depth());
}

void givenSegmentHasSeveralBranchesAndOneBranchHasAPixelInAlternateOrder_thenDepthStopsAtFirstPixel() {
    // Segment with 2 branches (depth 4 and 3) but branch with depth 4 has a Pixel at depth 2,
    // depth should be 2 (first encountered Pixel marks the bottom).
    Segment segment = Segment( //depth 0
            std::vector < Pixel * > {
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Segment(//depth 2
                                            std::vector < Pixel * > {
                                                    new Pixel(0, 1) //depth 3
                                            }
                                    )
                            }
                    ),
                    new Segment( //depth 1
                            std::vector < Pixel * > {
                                    new Segment(//depth 2
                                            std::vector < Pixel * > {
                                                    new Segment(//depth 3
                                                            std::vector < Pixel * > {
                                                                    new Pixel(0, 1) //depth 4
                                                            }
                                                    ),
                                                    new Pixel(0, 1) //depth 2
                                            }
                                    )
                            }
                    )
            }
    );

    TEST_ASSERT_EQUAL(3, segment.depth());
}

void setup() {
    UNITY_BEGIN();  // Start the Unity test session
    RUN_TEST(givenSegmentHasNoChildren_thenThrowException);
    RUN_TEST(givenSegmentHasOnePixel_thenDepthIs1);
    RUN_TEST(givenSegmentHasThreePixels_thenDepthIs1);
    RUN_TEST(givenSegmentHasOneSubSegment_thenDepthIs2);
    RUN_TEST(givenSegmentHasTwoSubSegments_thenDepthIs3);
    RUN_TEST(givenSegmentHasSeveralSubSegments_thenDepthIsShortestBranch);
    RUN_TEST(givenSegmentHasSeveralSubSegmentsInAlternateOrder_thenDepthIsShortestBranch);
    RUN_TEST(givenSegmentHasSeveralBranchesAndOneBranchHasAPixel_thenDepthStopsAtFirstPixel);
    RUN_TEST(givenSegmentHasSeveralBranchesAndOneBranchHasAPixelInAlternateOrder_thenDepthStopsAtFirstPixel);
    UNITY_END();    // End the Unity test session
}

void loop() {
    // Empty loop for PlatformIO test compatibility
}

int main(int argc, char **argv) {
    setup();
    return 0;
}

