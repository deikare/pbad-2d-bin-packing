//
// Created by deikare on 17.03.23.
//

#ifndef PBAD_2D_BIN_PACKING_NEURALNETWORK_H
#define PBAD_2D_BIN_PACKING_NEURALNETWORK_H

#include "vector"

using Features = std::vector<double>;

class NeuralNetwork {
private:
    //TODO wojtek
public:
    explicit NeuralNetwork(const std::vector<double>& weights);

    double rateItem(const Features& features) const; //trzeba będzie się zdecydować na jakiś wektor cech w argumencie

};


#endif //PBAD_2D_BIN_PACKING_NEURALNETWORK_H
