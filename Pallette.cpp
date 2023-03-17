//
// Created by deikare on 12.03.23.
//

#include "Pallette.h"

#include <utility>

Pallette::Pallette(const unsigned long width, const unsigned long height,
                   const std::list<std::pair<ItemType, unsigned long>> &itemTypes,
                   const std::vector<double> &weights) : width(width), height(height), itemTypes(itemTypes),
                                                         network(NeuralNetwork(weights)) {
    cpList.emplace_back(0, 0);
}

double Pallette::performSimulation() {
    double meanResult = 0.0;
    unsigned long insertsNumber = 0;

    while (!itemTypes.empty()) { //TODO add check of bounds
        double result = performInsertionStep();
        insertsNumber++;
        meanResult = (meanResult * (insertsNumber - 1) + result) / insertsNumber;
    }

    return meanResult;
}

double Pallette::performInsertionStep() {
    double bestRating = 0.0;

    struct InsertionTrialResult bestTrialResult = Pallette::InsertionTrialResult();
    auto cpBeg = cpList.begin();
    auto bestItemTypeIter = itemTypes.begin();

    auto itemTypesEnd = itemTypes.end();
    for (auto itemTypesIter = itemTypes.begin(); itemTypesIter != itemTypesEnd; itemTypesIter++) {
        auto cpEnd = cpList.end();
        for (auto cpIterator = cpList.begin(); cpIterator != cpEnd; cpIterator++) {
            tryInsertionForItem(cpIterator, cpBeg, cpEnd, itemTypesIter, false, bestRating,
                                bestTrialResult, bestItemTypeIter);
            tryInsertionForItem(cpIterator, cpBeg, cpEnd, itemTypesIter, true, bestRating,
                                bestTrialResult, bestItemTypeIter);
        }
    }

    updateCounterPoints(bestTrialResult, bestItemTypeIter->first);
    updateItemList(bestItemTypeIter);

    return bestRating;
}

void Pallette::tryInsertionForItem(const std::_List_iterator<CounterPoint> &cpIterator,
                                   const std::_List_iterator<CounterPoint> &beg,
                                   const std::_List_iterator<CounterPoint> &end,
                                   const std::_List_iterator<ItemTypeTuple> &itemTypesIterator,
                                   bool pivot, double &bestRating,
                                   Pallette::InsertionTrialResult &bestTrialResult,
                                   std::_List_iterator<ItemTypeTuple> &bestItemTypeIter) {
    auto tmpTrialResult = getInsertionTrialResult(cpIterator, beg, end,
                                                  itemTypesIterator->first, pivot);
    auto rating = network.rateItem(tmpTrialResult.features);

    if (rating > bestRating) {
        bestTrialResult = tmpTrialResult;
        bestRating = rating;
        bestItemTypeIter = itemTypesIterator;
    }
}


Pallette::InsertionTrialResult
Pallette::getInsertionTrialResult(const std::_List_iterator<CounterPoint> &cpIterator,
                                  const std::_List_iterator<CounterPoint> &beg,
                                  const std::_List_iterator<CounterPoint> &end,
                                  const ItemType &itemType, bool pivot) {
    InsertionTrialResult result;

    unsigned long itemWidth;
    unsigned long itemHeight;
    initializeItemSize(itemWidth, itemHeight, pivot, itemType);

    unsigned long topBorder = cpIterator->second + itemHeight;
    auto topLeftCP = cpIterator;
    while (topLeftCP != beg) {
        auto prev = std::prev(topLeftCP);
        if (prev->second > topBorder)
            break;

        topLeftCP = prev;
    }

    unsigned long rightBorder = cpIterator->first + itemWidth;
    auto bottomRightCP = cpIterator;
    while (true) {
        auto next = std::next(topLeftCP);
        if (next == end || next->first > rightBorder)
            break;

        bottomRightCP = next;
    }

    result.bottomRightCp = bottomRightCP;
    result.topLeftCp = topLeftCP;
    result.pivot = pivot;

    // TODO calculate features

    return result;
}

void Pallette::updateCounterPoints(const Pallette::InsertionTrialResult &bestTrialResult, const ItemType &itemType) {
    auto bottomRightCp = bestTrialResult.bottomRightCp;
    auto topLeftCp = bestTrialResult.topLeftCp;

    unsigned long itemWidth;
    unsigned long itemHeight;
    initializeItemSize(itemWidth, itemHeight, bestTrialResult.pivot, itemType);

    if (topLeftCp == bottomRightCp)
        cpList.emplace(bottomRightCp, topLeftCp->first, topLeftCp->second + itemHeight);
    else {
        topLeftCp->second += itemHeight;
        auto toRemove = std::next(topLeftCp);
        while (toRemove != bottomRightCp)
            toRemove = cpList.erase(toRemove);
    }
}

void Pallette::updateItemList(const std::_List_iterator<ItemTypeTuple> &bestItemTypeIter) {
    if (bestItemTypeIter->second == 1)
        itemTypes.erase(bestItemTypeIter);
    else (bestItemTypeIter->second)--;
}


void Pallette::initializeItemSize(unsigned long &itemWidth, unsigned long &itemHeight, bool pivot,
                                  const ItemType &itemType) {
    if (pivot) {
        itemWidth = itemType.second;
        itemHeight = itemType.first;
    } else {
        itemWidth = itemType.first;
        itemHeight = itemType.second;
    }
}



