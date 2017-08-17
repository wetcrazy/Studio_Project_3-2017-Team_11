#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include "ClassHighScore.h"

// QuickSort Functions
void QuickSort(vector<HighScore> *list, int first_index, int last_index);
void Swap(vector<HighScore> *list, int first_index, int second_index);
int Partition(vector<HighScore> *list, int first_index, int last_index);

#endif // !QUICK_SORT_H
