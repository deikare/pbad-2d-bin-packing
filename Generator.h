//
// Created by wmoczydl on 22.05.23.
//

#ifndef PBAD_2D_BIN_PACKING_GENERATOR_H
#define PBAD_2D_BIN_PACKING_GENERATOR_H

#include <utility>
#include "list"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#include "Palette.h"

class Generator {

public:
    Generator(int min_length, int max_length, int min_count, int max_count, int items_type_count, double difficulty);

    void generate();

    void count_palette_size();

    std::list<std::pair<ItemType, unsigned long>> getItems(){
        return items;
    }

    std::pair<LengthUnit, LengthUnit> getPaletteSize(){
        return palette_size;
    }

private:
    int min_length;
    int max_length;
    int min_count;
    int max_count;
    int items_type_count;
    double difficulty;

    std::list<std::pair<ItemType, unsigned long>> items;
    std::pair<LengthUnit, LengthUnit> palette_size =  std::make_pair(0, 0);
};

#endif //PBAD_2D_BIN_PACKING_GENERATOR_H
