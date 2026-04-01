#ifndef SORTING_H
#define SORTING_H

#include "bits/stdc++.h"
using namespace std;

class Sorting
{
private:
    static vector<int> Merge(vector<int> right, vector<int> left)
    {
        vector<int> merge_Vector;
        int i=0, j=0;
        while (i < right.size() && j < left.size())
        {
            if (right[i] < left[j])
            {
                merge_Vector.push_back(right[i++]);
            }
            else
            {
                merge_Vector.push_back(left[j++]);
            }
        }

        while (i < right.size())
            merge_Vector.push_back(right[i++]);

        while (j < left.size())
            merge_Vector.push_back(left[j++]);

        return merge_Vector;
    }

public:
    static vector<int> InsertionSort(vector<int>& v);

    static vector<int> SelectionSort(vector<int>& v);

    static vector<int> BubbleSort(vector<int>& v);

    static vector<int> MergeSort(vector<int>& v);

    static vector<int> QuickSort(vector<int>& v);

    static vector<int> ShellSort(vector<int>& v);

    static vector<int> CountingSort(vector<int>& v);

    // static vector<int> RadixSort(vector<int>& v);

    static vector<int> shellSort(vector<int>& v);
};


#endif //SORTING_H
