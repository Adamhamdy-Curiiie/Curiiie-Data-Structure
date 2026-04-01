#include "Sorting.cpp"

using namespace std;


int main()
{

    vector<int> v = {3,23,49,16,32,39} /*{3, 49, 23, 8, 5, 16}*/ /*{3,8,5}*/ , v1 = {3,8,1,5,9,4};
    Sorting::MergeSort(v);

    for (int x : v)
        cout << x << ' ';


    return 0;
}