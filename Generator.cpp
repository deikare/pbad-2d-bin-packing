#include "Generator.h"

Generator::Generator(int min_length, int max_length, int min_count, int max_count, int items_type_count, double difficulty) : min_length(min_length), max_length(max_length), min_count(min_count), max_count(max_count), difficulty(difficulty), items_type_count(items_type_count) {}

void Generator::generate() {
    srand(time(0));

    std::list<std::pair<ItemType, unsigned long>> new_items;

    for(int i = 0 ; i < items_type_count; i++){
        bool isAdded = false;

        int width = (rand() % (max_length - min_length)) + min_length;
        int height = (rand() % (max_length - min_length)) + min_length;
        int count = (rand() % (max_count - min_count)) + min_count;

        std::pair<ItemType, unsigned long> newItem;
        newItem.first.first = width;
        newItem.first.second = height;
        newItem.second = count;

        for(int j = 0; j < new_items.size(); j++){
            auto list_front = new_items.begin();
            std::advance(list_front, j);

            if(list_front->first.first == newItem.first.first && list_front->first.second == newItem.first.second){
                list_front->second += newItem.second;
                isAdded = true;
                break;
            }
        }

        if(!isAdded){
            new_items.push_back(newItem);
        }
    }

    int palette_with = 0;
    int palette_height = 0;

    for(int i = 0; i < new_items.size(); i++){
        auto item = new_items.begin();

        palette_with+= item->first.first * item->second;
        palette_height+= item->first.second * item->second;

        std::advance(item, 1);
    }

    palette_with /= difficulty;
    palette_height /= difficulty;

    palette_size =  std::make_pair(palette_with, palette_height);
    items = new_items;
}