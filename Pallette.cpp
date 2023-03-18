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

    while (!itemTypes.empty() && !cpList.empty()) {
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
    std::_List_iterator<ItemTypeTuple> bestItemTypeIter;

    auto itemTypesEnd = itemTypes.end();
    for (auto itemTypesIter = itemTypes.begin(); itemTypesIter != itemTypesEnd; itemTypesIter++) {
        auto cpEnd = cpList.end();
        for (auto cpIterator = cpList.begin();
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
                                   const unsigned long itemWidth, const unsigned long itemHeight, double &bestRating,
                                   Pallette::InsertionTrialResult &bestTrialResult,
                                   std::_List_iterator<ItemTypeTuple> &bestItemTypeIter) {
    std::pair<bool, bool> result = {true, true};

    unsigned long topBorder = cpIterator->second + itemHeight;
    unsigned long rightBorder = cpIterator->first + itemWidth;

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
            bestItemTypeIter = itemTypesIterator;
        }
    }
}


void Pallette::updateCounterPoints(const Pallette::InsertionTrialResult &bestTrialResult) {
    auto bottomRightCp = bestTrialResult.bottomRightCp;
    auto topLeftCp = bestTrialResult.topLeftCp; //TODO think whether there is simpler approach

    if (topLeftCp == bottomRightCp) {
        cpList.emplace(bottomRightCp, topLeftCp->first, bestTrialResult.topBorder);
        bottomRightCp->first = bestTrialResult.rightBorder;
        topLeftCp = std::prev(topLeftCp);
    } else {
        bottomRightCp->first = bestTrialResult.rightBorder;

        topLeftCp->second = bestTrialResult.topBorder;

        cpList.erase(std::next(topLeftCp), bottomRightCp);
    }

    if (bottomRightCp->first == width)
        cpList.pop_back();

    if (topLeftCp->second == height)
        cpList.pop_front();
}

void Pallette::updateItemList(const std::_List_iterator<ItemTypeTuple> &bestItemTypeIter) {
    if (bestItemTypeIter->second == 1)
        itemTypes.erase(bestItemTypeIter);
    else (bestItemTypeIter->second)--;
}



