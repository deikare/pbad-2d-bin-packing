#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(std::vector<std::vector<double>> weights): weights(weights){}

double NeuralNetwork::simulate(std::vector<double> initial_inputs){
    auto first_outputs = initial_inputs;

    auto second_inputs = calc_inputs(first_outputs, weights[0], weights[1].size());
    auto second_outputs = calc_outputs(second_inputs);

    auto third_inputs = calc_inputs(second_outputs, weights[1], 1);
    auto third_outputs = third_inputs;

    return third_inputs[0];
}

std::vector<double> NeuralNetwork::calc_inputs(std::vector<double> prev_outputs, std::vector<double> weights, int outputs_count){
    std::vector<double> inputs;
    double temp_value = 0;

    for (int output_index=0; output_index<outputs_count; output_index++) {
        for(int prev_output_index=0; prev_output_index<prev_outputs.size(); prev_output_index ++)
        {
            temp_value += prev_outputs[prev_output_index]*weights[prev_output_index];
        }
        inputs.push_back(temp_value);
    }
    temp_value=0;

    return inputs;
};

std::vector<double> NeuralNetwork::calc_outputs(std::vector<double> inputs){
    std::vector<double> outputs;

    for(int i=0; i<inputs.size(); i++) {
        outputs.push_back(1 / (1 + exp(-inputs[i])));
    }

    return outputs;
};
