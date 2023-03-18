//
// Created by deikare on 12.03.23.
//

#ifndef PBAD_2D_BIN_PACKING_PALLETTE_H
#define PBAD_2D_BIN_PACKING_PALLETTE_H

#include "list"
#include "NeuralNetwork.h"

using CounterPoint = std::pair<unsigned long, unsigned long>;
using ItemType = std::pair<unsigned long, unsigned long>;
using ItemTypeTuple = std::pair<ItemType, unsigned long>;

class Pallette {
private:
    const unsigned long width;
    const unsigned long height;

    std::list<CounterPoint> cpList;
    std::list<ItemTypeTuple> itemTypes; //pair of pairs is faster than tuple, see https://stackoverflow.com/questions/6687107/difference-between-stdpair-and-stdtuple-with-only-two-members

    const NeuralNetwork network;

    double performInsertionStep();

    struct InsertionTrialResult {
        std::_List_iterator<CounterPoint> topLeftCp;
        std::_List_iterator<CounterPoint> bottomRightCp;
        Features features;
        bool topLeftEqual;
        bool bottomRightEqual;
        unsigned long rightBorder;
        unsigned long topBorder;
    };

    void tryInsertionForItem(const std::_List_iterator<CounterPoint> &cpIterator,
                             const std::_List_iterator<CounterPoint> &beg, const std::_List_iterator<CounterPoint> &end,
                             const std::_List_iterator<ItemTypeTuple> &itemTypesIterator, unsigned long itemWidth,
                             unsigned long itemHeight, double &bestRating,
                             struct InsertionTrialResult &bestTrialResult,
                             std::_List_iterator<ItemTypeTuple> &bestItemTypeIter);

    void updateCounterPoints(const Pallette::InsertionTrialResult &bestTrialResult);

    void updateItemList(const std::_List_iterator<ItemTypeTuple> &bestItemTypeIter);

public:
    Pallette(unsigned long width, unsigned long height,
             const std::list<std::pair<ItemType, unsigned long>> &itemTypes,
             const std::vector<double> &weights);

    double performSimulation();
};


#endif //PBAD_2D_BIN_PACKING_PALLETTE_H
