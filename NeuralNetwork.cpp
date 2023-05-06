#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(std::vector<std::vector<std::vector<float>>> weights): weights(weights){}

std::vector<float> NeuralNetwork::simulate(std::vector<float> inputs){
    int layersCount = getLayersCount();
    std::vector<float> previousLayerNeurons = inputs;
    std::vector<float> layerNeurons;

    for(int layerIndex = 1; layerIndex < layersCount; layerIndex++){
        int neuronsCount =  getLayerSize(layerIndex);

        for(int neuronIndex = 0;neuronIndex < neuronsCount; neuronIndex++){
            float neuron = calculateNeuron(layerIndex, neuronIndex, previousLayerNeurons);
            layerNeurons.push_back(neuron);
        }

        previousLayerNeurons = layerNeurons;
    }

    return layerNeurons;
}

float NeuralNetwork::calculateSigm(float value){
    return 1 / (1 + exp(-value));
};

float NeuralNetwork::calculateNeuron(int layerIndex, int neuronIndex, std::vector<float> previousLayerNeurons) {
    int previousLayerIndex = layerIndex - 1;
    int previousLayerNeuronsCount = previousLayerNeurons.size();
    float value = 0;

    for(int previousLayerNeuronIndex = 0; previousLayerNeuronIndex < previousLayerNeuronsCount; previousLayerNeuronIndex++){
        float weight = weights[previousLayerIndex][previousLayerNeuronIndex][neuronIndex];
        float neuron = previousLayerNeurons[previousLayerNeuronIndex];

        value += weight * neuron;
    }

    // Add bias
    value+= weights[previousLayerIndex][previousLayerNeuronsCount][neuronIndex];

    return calculateSigm(value);
};

int NeuralNetwork::getLayerSize(int index) {
    if(index == 0){
        return weights[index].size() -1;
    }

    return weights[index - 1][0].size();
}

int NeuralNetwork::getLayersCount() {
    return weights.size() + 1;
}
