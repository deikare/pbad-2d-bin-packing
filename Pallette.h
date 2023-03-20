//
// Created by deikare on 12.03.23.
//

#ifndef PBAD_2D_BIN_PACKING_PALLETTE_H
#define PBAD_2D_BIN_PACKING_PALLETTE_H

#include "list"
#include "NeuralNetwork.h"

using LengthUnit = unsigned long;
using CounterPoint = std::pair<LengthUnit, LengthUnit>;
using ItemType = std::pair<LengthUnit, LengthUnit>;
using ItemTypeTuple = std::pair<ItemType, unsigned long>;


class Pallette {
private:
    const LengthUnit width;
    const LengthUnit height;

    std::list<CounterPoint> counterPoints;
    std::list<ItemTypeTuple> itemTypes; //pair of pairs is faster than tuple, see https://stackoverflow.com/questions/6687107/difference-between-stdpair-and-stdtuple-with-only-two-members

    const NeuralNetwork network;

    double performInsertionStep();

    struct InsertionTrialResult {
        std::_List_iterator<CounterPoint> topLeftCp;
        std::_List_iterator<CounterPoint> bottomRightCp;
        LengthUnit rightBorder{};
        LengthUnit topBorder{};
    };

    void tryInsertionForItem(const std::_List_iterator<CounterPoint> &cpIterator,
                             const std::_List_iterator<CounterPoint> &beg, const std::_List_iterator<CounterPoint> &end,
                             const std::_List_iterator<ItemTypeTuple> &itemTypesIterator, LengthUnit itemWidth,
                             LengthUnit itemHeight, double &bestRating,
                             struct InsertionTrialResult &bestTrialResult,
                             std::_List_iterator<ItemTypeTuple> &bestItemTypeIterator);

    void updateCounterPoints(const Pallette::InsertionTrialResult &bestTrialResult);

    void updateItemList(const std::_List_iterator<ItemTypeTuple> &bestItemTypeIterator);

public:
    Pallette(LengthUnit width, LengthUnit height,
             const std::list<std::pair<ItemType, unsigned long>> &itemTypes,
             const std::vector<double> &weights);

    double performSimulation();
};


#endif //PBAD_2D_BIN_PACKING_PALLETTE_H
