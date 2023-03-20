//
// Created by deikare on 12.03.23.
//

#include "Pallette.h"

#include <utility>

Pallette::Pallette(const LengthUnit width, const LengthUnit height,
                   const std::list<std::pair<ItemType, unsigned long>> &itemTypes,
                   const std::vector<double> &weights) : width(width), height(height), itemTypes(itemTypes),
                                                         network(NeuralNetwork(weights)) {
    counterPoints.emplace_back(0, 0);
}

double Pallette::performSimulation() {
    double meanResult = 0.0;
    unsigned long insertsNumber = 0;

    while (!itemTypes.empty() && !counterPoints.empty()) {
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
    auto cpBeg = counterPoints.begin();
    std::_List_iterator<ItemTypeTuple> bestItemTypeIter;

    auto itemTypesEnd = itemTypes.end();
    for (auto itemTypesIter = itemTypes.begin(); itemTypesIter != itemTypesEnd; itemTypesIter++) {
        auto cpEnd = counterPoints.end();
        for (auto cpIterator = counterPoints.begin();
             cpIterator != cpEnd; cpIterator++) { //TODO upgrade iterative search to bisection search
            tryInsertionForItem(cpIterator, cpBeg, cpEnd, itemTypesIter, itemTypesIter->first.first,
                                itemTypesIter->first.second, bestRating,
                                bestTrialResult, bestItemTypeIter);
//            tryInsertionForItem(cpIterator, cpBeg, cpEnd, itemTypesIter, itemTypesIter->first.second,
//                                itemTypesIter->first.first, bestRating,
//                                bestTrialResult, bestItemTypeIter);
        }
    }

    if (bestRating >= 0) {
        updateCounterPoints(bestTrialResult);
        updateItemList(bestItemTypeIter);
    }

    return bestRating;
}

void Pallette::tryInsertionForItem(const std::_List_iterator<CounterPoint> &cpIterator,
                                   const std::_List_iterator<CounterPoint> &beg,
                                   const std::_List_iterator<CounterPoint> &end,
                                   const std::_List_iterator<ItemTypeTuple> &itemTypesIterator,
                                   const LengthUnit itemWidth, const LengthUnit itemHeight, double &bestRating,
                                   Pallette::InsertionTrialResult &bestTrialResult,
                                   std::_List_iterator<ItemTypeTuple> &bestItemTypeIterator) {
    std::pair<bool, bool> result = {true, true};

    LengthUnit topBorder = cpIterator->second + itemHeight;
    LengthUnit rightBorder = cpIterator->first + itemWidth;

    if (rightBorder > width)
        result.first = false;

    if (topBorder > height)
        result.second = false;
    else if (result.first) {
        InsertionTrialResult trialResult;

        auto topLeftCP = cpIterator;
        while (topLeftCP != beg) {
            auto prev = std::prev(topLeftCP);
            if (prev->second > topBorder)
                break;

            topLeftCP = prev;
        }

        auto bottomRightCP = cpIterator;
        while (true) {
            auto next = std::next(bottomRightCP);
            if (next == end || next->first > rightBorder)
                break;

            bottomRightCP = next;
        }


        trialResult.bottomRightCp = bottomRightCP;
        trialResult.topLeftCp = topLeftCP;
        trialResult.rightBorder = rightBorder;
        trialResult.topBorder = topBorder;

        // TODO calculate features
        std::vector<double> features = {};
        auto rating = network.rateItem(features);

        if (rating > bestRating) {
            bestTrialResult = trialResult;
            bestRating = rating;
            bestItemTypeIterator = itemTypesIterator;
        }
    }
}


void Pallette::updateCounterPoints(const Pallette::InsertionTrialResult &bestTrialResult) {
    auto bottomRightCp = bestTrialResult.bottomRightCp;
    auto topLeftCp = bestTrialResult.topLeftCp; //TODO think whether there is simpler approach

    if (topLeftCp == bottomRightCp) {
        counterPoints.emplace(bottomRightCp, topLeftCp->first, bestTrialResult.topBorder);
        bottomRightCp->first = bestTrialResult.rightBorder;
        topLeftCp = std::prev(topLeftCp);
    } else {
        bottomRightCp->first = bestTrialResult.rightBorder;

        topLeftCp->second = bestTrialResult.topBorder;

        counterPoints.erase(std::next(topLeftCp), bottomRightCp); //remove unnecessary counterpoints - counterpoints in range [topleft + 1, bottomright)
    }

    if (bottomRightCp->first == width) //remove last or first cp if
        counterPoints.pop_back();

    if (topLeftCp->second == height)
        counterPoints.pop_front();
}

void Pallette::updateItemList(const std::_List_iterator<ItemTypeTuple> &bestItemTypeIterator) {
    if (bestItemTypeIterator->second == 1)
        itemTypes.erase(bestItemTypeIterator);
    else (bestItemTypeIterator->second)--;
}



