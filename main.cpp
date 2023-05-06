#include <iostream>
#include "list"
//#include "Palette.h"
#include "NeuralNetwork.h"

//std::string print_list(const std::list<int> &list) {
//    std::string result = "[";
//    for (auto el: list) {
//        result += std::to_string(el) + " ";
//    }
//    if (!list.empty())
//        result.pop_back();
//    result += "]";
//    return result;
//}
//
//void
//make_delete(std::list<int> &list, std::list<int>::iterator  first, std::list<int>::iterator last,
//            const std::string &text) {
//    std::cout << "Pre " + text + ": " + print_list(list) << std::endl;
//    list.erase(first, last);
//    std::cout << "Post: " + print_list(list) << std::endl << std::endl;
//}
//
//std::list<ItemTypeTuple> createItems() {
//    std::list<ItemTypeTuple> result = {
//            {{20, 10}, 1},
//            {{10, 10}, 1},
//            {{18, 20}, 1},
//            {{30, 40}, 1},
//            {{30, 25}, 1},
//            {{20, 25}, 1},
//            {{10, 30}, 1},
//            {{13, 30}, 1},
//            {{2, 25}, 1},
//            {{7, 25}, 1},
//            {{40, 90}, 1},
//            {{150, 150}, 1},
//            {{20, 10}, 1},
//            {{40, 5}, 1},
//            {{50, 3}, 1},
//            {{25, 8}, 1},
//    };
//    return result;
//}



int main() {
//    auto itemTypes = createItems();
//    unsigned long width = 100;
//    unsigned long height = 100;

//    Palette palette(width, height, itemTypes, weights);
//    palette.performSimulation();

    std::vector<std::vector<std::vector<float>>> weights = {
            {{0.1, 0.2, 0.1},
             {0.2, 0.3, 0.1},
             {0.2, 0.3, 0.4},
            },
            {{0.1},
             {0.8},
             {0.2},
             {0.3}}
    };
//
    NeuralNetwork neuralNetwork(weights);
    std::cout << neuralNetwork.simulate({0.3, 0.9})[0];

    return 0;
}
