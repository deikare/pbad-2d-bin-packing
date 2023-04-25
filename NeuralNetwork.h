//
// Created by deikare on 17.03.23.
//

#ifndef PBAD_2D_BIN_PACKING_NEURALNETWORK_H
#define PBAD_2D_BIN_PACKING_NEURALNETWORK_H

#include "vector"
#include <cmath>

using Features = std::vector<double>;

class NeuralNetwork{

public:
    NeuralNetwork(std::vector<std::vector<double>> weights);

    double simulate(std::vector<double> inputs);

private:
    std::vector<std::vector<double>> weights;

    std::vector<double> calc_inputs(std::vector<double> prev_outputs, std::vector<double> weights, int outputs_count);
    std::vector<double> calc_outputs(std::vector<double> inputs);
};

#endif //PBAD_2D_BIN_PACKING_NEURALNETWORK_H
