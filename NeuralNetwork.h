//
// Created by deikare on 17.03.23.
//

#ifndef PBAD_2D_BIN_PACKING_NEURALNETWORK_H
#define PBAD_2D_BIN_PACKING_NEURALNETWORK_H

#include "vector"
#include <cmath>
#include <iostream>

using Features = std::vector<double>;

class NeuralNetwork {

public:
    NeuralNetwork(std::vector<std::vector<std::vector<float>>>);

    std::vector<float> simulate(std::vector<float> inputs);

private:
    std::vector<std::vector<std::vector<float>>> weights;

    float calculateSigm(float value);

    float calculateNeuron(int layerIndex, int neuronIndex, std::vector<float> previousLayerNeurons);

    int getLayerSize(int index);

    int getLayersCount();
};

#endif //PBAD_2D_BIN_PACKING_NEURALNETWORK_H
