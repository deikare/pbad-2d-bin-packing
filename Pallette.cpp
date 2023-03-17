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
        if (result < 0)
            break;
        insertsNumber++;
        meanResult = (meanResult * (insertsNumber - 1) + result) / insertsNumber;
    }

    return meanResult;
}

double Pallette::performInsertionStep() {
    double bestRating = -1;

    struct InsertionTrialResult bestTrialResult;
    auto cpBeg = cpList.begin();
    std::_List_iterator <ItemTypeTuple> bestItemTypeIter;

    auto itemTypesEnd = itemTypes.end();
    for (auto itemTypesIter = itemTypes.begin(); itemTypesIter != itemTypesEnd; itemTypesIter++) {
        auto cpEnd = cpList.end();
        for (auto cpIterator = cpList.begin(); cpIterator != cpEnd; cpIterator++) {
            tryInsertionForItem(cpIterator, cpBeg, cpEnd, itemTypesIter, itemTypesIter->first.first, itemTypesIter->first.second, bestRating,
                                bestTrialResult, bestItemTypeIter);
            tryInsertionForItem(cpIterator, cpBeg, cpEnd, itemTypesIter, itemTypesIter->first.second, itemTypesIter->first.first, bestRating,
                                bestTrialResult, bestItemTypeIter);
        }
    }

    if (bestRating >= 0) {
        updateCounterPoints(bestTrialResult, bestItemTypeIter->first);
        updateItemList(bestItemTypeIter);
    }

    return bestRating;
}

void Pallette::tryInsertionForItem(const std::_List_iterator<CounterPoint> &cpIterator,
                                   const std::_List_iterator<CounterPoint> &beg,
                                   const std::_List_iterator<CounterPoint> &end,
                                   const std::_List_iterator<ItemTypeTuple> &itemTypesIterator,
                                   const unsigned long itemWidth, const unsigned long itemHeight, double &bestRating,
                                   Pallette::InsertionTrialResult &bestTrialResult,
                                   std::_List_iterator<ItemTypeTuple> &bestItemTypeIter) {
    auto tmpTrialResult = getInsertionTrialResult(cpIterator, beg, end, itemWidth, itemHeight);
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
                                  const unsigned long itemWidth, const unsigned long itemHeight) {
    InsertionTrialResult result;

    unsigned long topBorder = cpIterator->second + itemHeight;
    auto topLeftCP = cpIterator;
    while (topLeftCP != beg) {
        auto prev = std::prev(topLeftCP);
        if (prev->second > topBorder) {
            result.topLeftEqual = topLeftCP->second == topBorder;
            break;
        }

        topLeftCP = prev;
    }

    unsigned long rightBorder = cpIterator->first + itemWidth;
    auto bottomRightCP = cpIterator;
    while (true) {
        auto next = std::next(topLeftCP);
        if (next == end || next->first > rightBorder) {
            result.bottomRightEqual = bottomRightCP->first == rightBorder;
            break;
        }

        bottomRightCP = next;
    }

    result.bottomRightCp = bottomRightCP;
    result.topLeftCp = topLeftCP;

    // TODO calculate features

    return result;
}

void Pallette::updateCounterPoints(const Pallette::InsertionTrialResult &bestTrialResult, const ItemType &itemType) {
    auto bottomRightCp = bestTrialResult.bottomRightCp;
    auto topLeftCp = bestTrialResult.topLeftCp;

    if (topLeftCp == bottomRightCp) {
        cpList.emplace(bottomRightCp, topLeftCp->first, topLeftCp->second + bestTrialResult.itemHeight);
        bottomRightCp->first += bestTrialResult.itemWidth;
    } else {
        if (!bestTrialResult.bottomRightEqual)
            bottomRightCp->first += bestTrialResult.itemWidth;

        if (!bestTrialResult.topLeftEqual)
            topLeftCp->second += bestTrialResult.itemHeight;

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



