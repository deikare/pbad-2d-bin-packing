//
// Created by deikare on 12.03.23.
//

#include "Palette.h"

#include <utility>

Palette::Palette(const LengthUnit width, const LengthUnit height,
                 const std::list<std::pair<ItemType, unsigned long>> &itemTypes,
                 const std::vector<std::vector<std::vector<float>>> &weights, unsigned levelsNumber) : width(width),
                                                                                                       height(height),
                                                                                                       remainingArea(
                                                                                                               width *
                                                                                                               height),
                                                                                                       itemTypes(
                                                                                                               itemTypes),
                                                                                                       network(NeuralNetwork(
                                                                                                               weights)),
                                                                                                       levelIncrement(
                                                                                                               height /
                                                                                                               (levelsNumber -
                                                                                                                1)) {
    counterPoints.emplace_back(0, 0);
    itemsNumber = 0;
    for (auto &itemType: itemTypes) {
        itemsNumber += itemType.second;
    }
}

double Palette::performSimulation() {
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

double Palette::performInsertionStep() {
    double bestRating = -1;

    struct InsertionTrialResult bestTrialResult;
    auto cpBeg = counterPoints.begin();
    std::list<ItemTypeTuple>::iterator bestItemTypeIter;

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
        remainingArea -= bestTrialResult.area;
    }

    return bestRating;
}

void Palette::tryInsertionForItem(const std::list<CounterPoint>::iterator &cpIterator,
                                  const std::list<CounterPoint>::iterator &beg,
                                  const std::list<CounterPoint>::iterator &end,
                                  const std::list<ItemTypeTuple>::iterator &itemTypesIterator,
                                  const LengthUnit itemWidth, const LengthUnit itemHeight, double &bestRating,
                                  Palette::InsertionTrialResult &bestTrialResult,
                                  std::list<ItemTypeTuple>::iterator &bestItemTypeIterator) {
    std::pair<bool, bool> result = {true, true};

    LengthUnit topBorder = cpIterator->second + itemHeight;
    LengthUnit rightBorder = cpIterator->first + itemWidth;

    if (rightBorder > width)
        result.first = false;

    if (topBorder > height)
        result.second = false;
    else if (result.first) { //item is legal to place
        InsertionTrialResult trialResult;

        LengthUnit totalWastedWidth = 0;
        LengthUnit totalWastedHeight = 0;
        LengthUnit totalWastedArea = 0;

        auto topLeftCP = cpIterator;
        while (topLeftCP != beg) {
            auto prev = std::prev(topLeftCP);
            if (prev->second > topBorder)
                break;

            LengthUnit wastedWidth = topLeftCP->first - prev->first;
            LengthUnit wastedHeight = topBorder - prev->second;
            totalWastedWidth += wastedWidth;
            totalWastedHeight += wastedHeight;
            totalWastedArea += wastedWidth * wastedHeight;

            topLeftCP = prev;
        }

        auto bottomRightCP = cpIterator;
        while (true) {
            auto next = std::next(bottomRightCP);
            if (next == end || next->first > rightBorder)
                break;

            LengthUnit wastedWidth = rightBorder - next->first;
            LengthUnit wastedHeight = bottomRightCP->second - next->second;
            totalWastedWidth += wastedWidth;
            totalWastedHeight += wastedHeight;
            totalWastedArea += wastedWidth * wastedHeight;

            bottomRightCP = next;
        }

        trialResult.bottomRightCp = bottomRightCP;
        trialResult.topLeftCp = topLeftCP;
        trialResult.rightBorder = rightBorder;
        trialResult.topBorder = topBorder;
        LengthUnit itemArea = itemHeight * itemWidth;
        trialResult.area = itemArea;

        // TODO calculate features
//        Features features = {
//                (float) itemWidth / (float) width,
//                (float) itemHeight / (float) height,
//                float(itemArea) / float(remainingArea),
//                float(totalWastedWidth) / float(width),
//                float(totalWastedHeight) / float(height),
//                float(totalWastedArea) / float(remainingArea)
//        };
        Features features{};

        //póki poziom poniżej ostatniego, to wstawiam jedynkę
        //pomiędzy końcem a bottomrightem wstawiam x najbliższego cp
        //pomiędzy bottomright i topleft wstawiam rightbordery
        //od topleft do beg wstawiam najbliższy cp
        //powyżej beg wstawiam x bega

        LengthUnit lowerBound, upperBound;
        lowerBound = bottomRightCP->second;
        upperBound = topBorder;
        auto tmpIterator = std::prev(end);
        auto lowestCpHeight = tmpIterator->second;
        LengthUnit level = 0;

        if (lowestCpHeight > 0) {
            while (level <= lowestCpHeight) {
                features.emplace_back(1.0f);
                level += levelIncrement;
            }
        }

        auto widthAsFloat = float(width);

        if (lowerBound > 0) {
            while (level <= lowerBound) {
                auto prev = std::prev(tmpIterator);
                while (prev != bottomRightCP && prev->second < level) {
                    tmpIterator = prev;
                    prev = std::prev(prev);
                }
                features.emplace_back(float(tmpIterator->first) / widthAsFloat);
                level += levelIncrement;
            }
        }

        auto itemLevelValue = float(rightBorder) / widthAsFloat;
        while (level <= upperBound) {
            features.emplace_back(itemLevelValue);
            level += levelIncrement;
        }

        if (topLeftCP != beg) {
            auto firstCpHeight = beg->second;
            tmpIterator = topLeftCP;
            auto prev = std::prev(tmpIterator);

            while (level <= firstCpHeight) {
                while (tmpIterator != beg && prev->second < level) {
                    tmpIterator = prev;
                    prev = std::prev(prev);
                }
                features.emplace_back(float(tmpIterator->first) / float(width));
                level += levelIncrement;
            }
        }

        itemLevelValue = float(beg->first) / widthAsFloat;
        while (level <= height) {
            features.emplace_back(itemLevelValue);
            level += levelIncrement;
        }

        features.emplace_back(
                float(std::distance(topLeftCP, bottomRightCP) + 1) / float(counterPoints.size())); //used cps amount

        for (auto it = itemTypes.begin(); it != itemTypesIterator; it++) //how many items yet to place
            features.emplace_back(float(it->second) / float(itemsNumber));

        features.emplace_back(float(itemTypesIterator->second - 1) / float(itemsNumber));

        auto itemTypesEnd = itemTypes.end();
        for (auto it = itemTypesIterator; it != itemTypesEnd; it++)
            features.emplace_back(float(it->second) / float(itemsNumber));

        for (auto i = itemTypes.size(); i < itemTypesNumberLimit; i++)  //complete missing itemTypes to always same size
            features.emplace_back(0.0f);

        LengthUnit remainingHeight = height - topBorder; //how much space is wasted if same type would be inserted
        features.emplace_back(float(remainingHeight % itemHeight) / float(remainingHeight));

        LengthUnit remainingWidth = width - rightBorder;
        features.emplace_back(float(remainingWidth % itemWidth) / float(remainingWidth));

        features.emplace_back(rightBorder == width ? 1.0f : 0.0f); //whether the edges match
        features.emplace_back(topBorder == height ? 1.0f : 0.0f);

//        auto rating = network.simulate(features);
        std::vector<float> rating = {1};

        if (rating[0] > bestRating) { //if improve
            bestTrialResult = trialResult;
            bestRating = rating[0];
            bestItemTypeIterator = itemTypesIterator;
        }
    }
}


void Palette::updateCounterPoints(const Palette::InsertionTrialResult &bestTrialResult) {
    auto bottomRightCp = bestTrialResult.bottomRightCp;
    auto topLeftCp = bestTrialResult.topLeftCp; //TODO think whether there is simpler approach

    auto topBorder = bestTrialResult.topBorder;
    auto rightBorder = bestTrialResult.rightBorder;

    if (topLeftCp == bottomRightCp) {
        if (topBorder != height)
            counterPoints.emplace(bottomRightCp, topLeftCp->first,
                                  topBorder); //topleft == bottomright, so there is new topleft to add before bottomright
    } else {
        counterPoints.erase(std::next(topLeftCp),
                            bottomRightCp); //remove unnecessary counterpoints in range [topleft + 1, bottomright)
        if (topBorder == height)
            counterPoints.pop_front();
        else topLeftCp->second = topBorder;
    }

    //remove last cp if it's placed at right border
    if (rightBorder == width)
        counterPoints.pop_back();
    else bottomRightCp->first = rightBorder;
}

void Palette::updateItemList(const std::list<ItemTypeTuple>::iterator &bestItemTypeIterator) {
    if (bestItemTypeIterator->second == 1) //if there is single element to decrease count
        itemTypes.erase(bestItemTypeIterator); //erase it
    else (bestItemTypeIterator->second)--;
    itemsNumber--;
}



