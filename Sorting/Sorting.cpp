#include "Sorting.h"

vector<int> Sorting::MergeSort(vector<int>& v)
{
    if (v.size() == 1)
        return v;

    int mid = v.size() / 2;

    vector<int> right(v.begin(), v.begin() + mid);
    vector<int> left(v.begin() + mid, v.end());

    right = MergeSort(right);
    left = MergeSort(left);

    v = Merge(right, left);

    return v;
}
// ===========================================================================================================

vector<int> Sorting::QuickSort(vector<int>& v)
{
    if (v.size() <= 1)
        return v;

    int mid = v.size() / 2;
    int pivot = v[mid], Pswap(0);

    swap(v[mid], v.back());

    for (int loc = 0; loc < v.size(); ++loc)
    {
        if (v[loc] > pivot)
        {
            continue;
        }


        if (v[loc] == v[Pswap])
        {
            ++Pswap;
            continue;
        }

        else if (v[loc] < v[Pswap]) swap(v[loc], v[Pswap++]);
    }

    vector<int> left(v.begin(), v.begin() + Pswap - 1);
    vector<int> right(v.begin() + Pswap - 1, v.end());

    left = QuickSort(left);
    right = QuickSort(right);

    v = Merge(right, left);

    return v;
}

// ===========================================================================================================

vector<int> Sorting::InsertionSort(vector<int>& v)
{
    if(v.size() == 1)
        return v;

    for(int i=1; i < v.size(); ++i)
    {
        while((v[i]< v[i-1]))
        {
            swap(v[i], v[i-1]);
            --i;
        }
    }

    return v;
}

// ===========================================================================================================

vector<int> Sorting::SelectionSort(vector<int>& v)
{
    if (v.size() == 1)
        return v;

    for (int i=0, j; i < v.size(); ++i)
    {
        int min = v.size()-1;
        for (j = i; j < v.size(); ++j)
        {
            if (v[j] < v[min])
                min = j;
        }

        if (min != i) swap(v[min], v[i]);
    }

    return v;
}

// ===========================================================================================================

vector<int> Sorting::BubbleSort(vector<int>& v)
{
    if (v.size() == 1)
        return v;

    bool is_Sorted = true;

    for (int i=0; i<v.size(); ++i)
    {
        for (int j=v.size()-1; j > i; --j)
        {
            if (v[j] < v[j-1])
            {
                swap(v[j], v[j-1]);
                is_Sorted = false;
            }
        }
        if (is_Sorted) return v;
    }

    return v;
}

// ===========================================================================================================

vector<int> Sorting::CountingSort(vector<int>& v)
{
    if (v.size() == 1)
        return v;

    int max = 0;

    for (int x : v)
        if (x > max)
            max = x;

    int arr[max] = {0};

    for (int x : v)
        ++arr[x-1];

    for (int i = 1; i<max; ++i)
        arr[i] += arr[i - 1];

    vector<int> temp;
    temp.resize(v.size());

    for (int x : v)
        temp[--arr[x-1]] = x;

    v = temp;
    return v;
}

// ===========================================================================================================

// vector<int> Sorting::RadixSort(vector<int>& v)
// {
//     if (v.size() == 1)
//         return v;
//
//     return v;
// }

// ===========================================================================================================

vector<int> Sorting::shellSort(vector<int>& v)
{
    if (v.size() == 1)
        return v;
    int n = v.size();

    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int temp = v[i];
            int j;

            for (j = i; j >= gap && v[j - gap] > temp; j -= gap)
            {
                v[j] = v[j - gap];
            }

            v[j] = temp;
        }
    }
    return v;
}