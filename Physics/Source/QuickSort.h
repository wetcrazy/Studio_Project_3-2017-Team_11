#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <vector>

class HighScore;


// QuickSort Functions
void QuickSort(std::vector<HighScore> *list, int first_index, int last_index);
void Swap(std::vector<HighScore> *list, int first_index, int second_index);
int Partition(std::vector<HighScore> *list, int first_index, int last_index);

#endif // !QUICK_SORT_H
