//
// Created by deikare on 12.03.23.
//

#ifndef PBAD_2D_BIN_PACKING_PALETTE_H
#define PBAD_2D_BIN_PACKING_PALETTE_H

#include "list"
#include "NeuralNetwork.h"

using LengthUnit = unsigned long;
using CounterPoint = std::pair<LengthUnit, LengthUnit>;
using ItemType = std::pair<LengthUnit, LengthUnit>;
using ItemTypeTuple = std::pair<ItemType, unsigned long>;


class Palette {
private:
    const LengthUnit width;
    const LengthUnit height;
    LengthUnit remainingArea;

    const LengthUnit levelIncrement;

    unsigned long itemsNumber;

    static const unsigned itemTypesNumberLimit = 20;

    std::list<CounterPoint> counterPoints;
    std::list<ItemTypeTuple> itemTypes; //pair of pairs is faster than tuple, see https://stackoverflow.com/questions/6687107/difference-between-stdpair-and-stdtuple-with-only-two-members

    NeuralNetwork network;

    double performInsertionStep();

    struct InsertionTrialResult {
        std::list<CounterPoint>::iterator topLeftCp;
        std::list<CounterPoint>::iterator bottomRightCp;
        LengthUnit rightBorder{};
        LengthUnit topBorder{};
        LengthUnit area{};
    };

    void tryInsertionForItem(const std::list<CounterPoint>::iterator &cpIterator,
                             const std::list<CounterPoint>::iterator &beg,
                             const std::list<CounterPoint>::iterator &end,
                             const std::list<ItemTypeTuple>::iterator &itemTypesIterator,
                             LengthUnit itemWidth, LengthUnit itemHeight, double &bestRating,
                             Palette::InsertionTrialResult &bestTrialResult,
                             std::list<ItemTypeTuple>::iterator &bestItemTypeIterator);

    void updateCounterPoints(const Palette::InsertionTrialResult &bestTrialResult);

    void updateItemList(const std::list<ItemTypeTuple>::iterator &bestItemTypeIterator);

public:
    Palette(LengthUnit width, LengthUnit height,
            const std::list<std::pair<ItemType, unsigned long>> &itemTypes,
            const std::vector<std::vector<std::vector<float>>> &weights, unsigned levelsNumber);

    double performSimulation();
};


#endif //PBAD_2D_BIN_PACKING_PALETTE_H
