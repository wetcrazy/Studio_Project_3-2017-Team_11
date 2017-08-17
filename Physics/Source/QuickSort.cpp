#include "QuickSort.h"

// QuickSort Functions
void QuickSort(vector<HighScore> *list, int first_index, int last_index)
{
	if (first_index < last_index)
	{
		int pivot = Partition(list, first_index, last_index);
		QuickSort(list, first_index, pivot - 1);
		QuickSort(list, pivot + 1, last_index);
	}
	for (int check_index = 1; check_index <= last_index; ++check_index) // Make numbers in Rank in order
	{
		list->at(check_index).Data.rank = std::to_string(check_index);
	}
}

void Swap(vector<HighScore> *list, int first_index, int second_index)
{
	HighScore temp = list->at(first_index);
	list->at(first_index) = list->at(second_index);
	list->at(second_index) = temp;
}

int Partition(vector<HighScore> *list, int first_index, int last_index)
{
	int middle_index = (first_index + last_index) / 2;
	int small_index = first_index;

	int pivot_value = atoi(list->at(middle_index).Data.score.c_str());
	Swap(list, first_index, middle_index);

	for (int check_index = (first_index + 1); check_index <= last_index; ++check_index)// Sort By Score
	{
		if (atoi(list->at(check_index).Data.score.c_str()) > pivot_value)
		{
			++small_index;
			Swap(list, small_index, check_index);
		}
	}
	Swap(list, first_index, small_index);
	return small_index;
}