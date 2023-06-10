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
                                                                                                       levelsNumber(
                                                                                                               levelsNumber),
                                                                                                       levelIncrement(
                                                                                                               float(height) /
                                                                                                               float(levelsNumber - 1)) {
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
//        break; //todo REMOVE HERE!!!
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

        std::vector<float> levels(levelsNumber, 0.0f);
        //todo error here - consider types or different means of calculating indices
        int topLeftLevelIndex, bottomRightLevelIndex;
        LengthUnit lowerBound, upperBound;
        if (topLeftCP == bottomRightCP) {
            lowerBound = bottomRightCP->second;
            upperBound = topBorder;
//            topLeftLevelIndex = std::floor(double(topBorder) / levelIncrement); //levels
//            bottomRightLevelIndex = std::ceil(double(bottomRightCP->second) / levelIncrement - 1.0);
        } else {
            lowerBound = bottomRightCP->second;
            upperBound = topLeftCP->second;
//            upperBound = std::max(topBorder, topLeftCP->second);
//            topLeftLevelIndex = std::floor(double(topLeftCP->second) / levelIncrement - 1.0); //levels
//            bottomRightLevelIndex = std::ceil(double(bottomRightCP->second) / levelIncrement - 1.0);
        }

        int index = 0;
        LengthUnit level = 0;
        while (level < lowerBound) {
            index++;
            level += levelIncrement;
        }
        if (lowerBound % levelIncrement == 0)
            bottomRightLevelIndex = index + 1;
        else bottomRightLevelIndex = index;


        while (level <= upperBound) {
            index++;
            level += levelIncrement;
        }

        topLeftLevelIndex = index - 1;

        //póki poziom poniżej ostatniego, to wstawiam jedynkę
        //pomiędzy końcem a bottomrightem wstawiam x najbliższego cp
        //pomiędzy bottomright i topleft wstawiam rightbordery
        //od topleft do beg wstawiam najbliższy cp
        //powyżej beg wstawiam x bega

        auto tmpIterator = std::prev(end);
        if (tmpIterator == bottomRightCP) {
//            float value = float(bottomRightCP->first) / float(width);
            for (int i = 0; i < bottomRightLevelIndex; i++)
                features.emplace_back(1.0f);
        } else {
            level = levelIncrement;
            auto prev = std::prev(tmpIterator);
            for (int i = 0; i < bottomRightLevelIndex; i++, level += levelIncrement) {
                while (prev->second < level && prev != bottomRightCP) {
                    tmpIterator = prev;
                    prev = std::prev(prev);
                }
                features.emplace_back(float(tmpIterator->first) / float(width));
            }
        }

        for (int i = bottomRightLevelIndex; i <= topLeftLevelIndex; ++i)
            features.emplace_back(float(rightBorder) / float(width));


        topLeftLevelIndex++;
        if (topLeftCP == beg) {
            float value = float(beg->first) / float(width);
            for (int i = topLeftLevelIndex; i < levelsNumber; i++)
                features.emplace_back(value);
        } else {
            tmpIterator = std::prev(topLeftCP);
            auto prev = std::prev(tmpIterator);
            level = topLeftLevelIndex * levelIncrement;
            for (int i = topLeftLevelIndex; i < levelsNumber; i++, level += levelIncrement) {
                while (tmpIterator != beg && prev->second < level) {
                    tmpIterator = prev;
                    prev = std::prev(prev);
                }
                features.emplace_back(float(tmpIterator->first) / float(width));
            }
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



