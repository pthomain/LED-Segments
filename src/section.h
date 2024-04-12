#pragma once

#ifndef SECTION_H
#define SECTION_H

class Section {
public:
    int start;
    int end;
    int sectionSize;

    Section(int start, int end): start(start), end(end), sectionSize(end - start + 1) {}
};

#endif //SECTION_H