#include <iostream>
#include <algorithm>
#include <vector>

template<typename T>
void printArray(std::vector<T>&v, int n) {
    for ( T &num : v)
        std::cout << num << " ";
}


// ─────────────────────────────────────────────
//  Insertion Sort   O(n) best / O(n²) avg,worst
// ─────────────────────────────────────────────
/*
 * Complexity:
 *   Best Case:    O(n)  → already sorted
 *   Average Case: O(n²) → random order
 *   Worst Case:   O(n²) → reverse sorted
 *
 * Worst Case Math:
 *   Moves       = 2(n-1) + n(n-1)/2 = n²/2 + 3n/2 - 2
 *   Comparisons = 1+2+...+(n-1)     = n(n-1)/2 = n²/2 - n/2
 *
 * Average Case Math:
 *   Moves       = 2(n-1) + n(n-1)/4   = n²/4 + 7n/4 - 2
 *   Comparisons = 1+2+...+(n-1) * 1/2 = n(n-1)/4 = n²/4 - n/4
 *
 * Best Case Math:
 *   Moves       = 2(n-1) = 2n - 2
 *   Comparisons = n-1
 */
template<typename T>
void insertionSort(std::vector<T>& v, int n) {
    for (int i = 1, j; i < n; i++) {
        T tmp = v[i];                                   // save current element

        for (j = i; j > 0 && v[j - 1] > tmp; j--)
            v[j] = v[j - 1];                           // shift larger elements right

        v[j] = tmp;                                     // drop element in its correct place
    }
}

// ─────────────────────────────────────────────
//  Selection Sort   O(n²) always
// ─────────────────────────────────────────────
/*
 * Complexity:
 *   Best Case:    O(n²) → already sorted
 *   Average Case: O(n²) → random order
 *   Worst Case:   O(n²) → reverse sorted
 *
 * Worst/Best/Average Case Math (all the same, no early exit):
 *   Comparisons = (n-1) + (n-2) + ... + 1 = n(n-1)/2 = n²/2 - n/2
 *   Swaps       = n-1
 *   Moves       = 3(n-1) → swap function
 */
template<typename T>
void selectionSort(std::vector<T>& v, int n) {
    for (int i = 0, j; i < n; i++) {
        int minIdx = i;                                 // assume current is minimum

        for (j = i + 1; j < n; j++)
            if (v[j] < v[minIdx])
                minIdx = j;                             // found a smaller element

        if (minIdx != i)                                // skip swap if already minimum
            std::swap(v[i], v[minIdx]);
    }
}

// ─────────────────────────────────────────────
//  Bubble Sort   O(n) best / O(n²) avg,worst
// ─────────────────────────────────────────────
/*
 * Complexity:
 *   Best Case:    O(n)  → already sorted (swapped flag breaks early)
 *   Average Case: O(n²) → random order
 *   Worst Case:   O(n²) → reverse sorted
 *
 * Worst Case Math:
 *   Comparisons = (n-1) + (n-1) + ... + (n-1) = n(n-1) = n² - n
 *   Swaps       = (n-1) + (n-2) + ... + 1     = n(n-1)/2 = n²/2 - n/2
 *
 * Average Case Math:
 *   Comparisons = n(n-1)/2 = n²/2 - n/2
 *   Swaps       = n(n-1)/4 = n²/4 - n/4
 *
 * Best Case Math:
 *   Comparisons = n-1  (one pass, no swaps found → break)
 *   Swaps       = 0
 */
template<typename T>
void bubbleSort(std::vector<T>& v, int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;                           // optimization: stop if already sorted

        for (int j = 0; j < n - 1; j++)
            if (v[j] > v[j + 1]) {
                std::swap(v[j], v[j + 1]);             // bubble larger element right
                swapped = true;
            }

        if (!swapped) break;                            // no swaps → array is sorted → stop
    }
}

// ─────────────────────────────────────────────
//  Shell Sort   O(n log n) best / O(n²) worst
// ─────────────────────────────────────────────
/*
 * Complexity:
 *   Best Case:    O(n log n) → already sorted
 *   Average Case: O(n^1.5)   → random order
 *   Worst Case:   O(n²)      → bad gap pattern (even/odd blind spot)
 *
 * How it works:
 *   → insertion sort but jumps by gap instead of 1
 *   → large gap moves elements far quickly
 *   → by gap=1, array is nearly sorted → final pass is fast
 *
 * Gap Sequence (n/2, n/4, ..., 1):
 *   gap = n/2  → first pass  (big leaps, tiny subarrays)
 *   gap = n/4  → second pass (smaller leaps)
 *   gap = 1    → last pass   (regular insertion sort)
 *
 * Worst Case Math:
 *   Passes      = log n                (gap halves each time)
 *   Per pass    = n(n-1)/2             (full insertion sort)
 *   Total       = n(n-1)/2 * log n     = n²/2 * log n → O(n²)
 *
 * Average Case Math:
 *   Passes      = log n
 *   Per pass    = n^1.5 / log n
 *   Total       = n^1.5
 *
 * Best Case Math:
 *   Passes      = log n                (still shrink gap to 1)
 *   Per pass    = n-gap                (while loop never runs)
 *   Total       = (n-gap) * log n      → O(n log n)
 */
template<typename T>
void shellSort(std::vector<T>& v, int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {         // shrink gap each outer pass

        for (int i = gap; i < n; i++) {
            T tmp = v[i];                               // save element to insert
            int j = i;

            while (j >= gap && v[j - gap] > tmp) {     // shift elements by gap
                v[j] = v[j - gap];
                j -= gap;
            }
            v[j] = tmp;                                 // place element in correct gap-position
        }
    }
}

// ─────────────────────────────────────────────
//  Merge Sort   O(n log n) always
// ─────────────────────────────────────────────
/*
 * Complexity:
 *   Best Case:    O(n log n) → already sorted
 *   Average Case: O(n log n) → random order
 *   Worst Case:   O(n log n) → reverse sorted
 *
 * How it works:
 *   → split array in half recursively until 1 element
 *   → merge halves back together in sorted order
 *   → all sorting happens in merge, not mergeSort
 *
 * Worst/Best/Average Case Math (all the same!):
 *   levels      = log n        (how many times we can halve n)
 *   per level   = n            (merge touches every element once)
 *   comparisons = n * log n    → O(n log n)
 *   moves       = n * log n    → O(n log n)
 *
 * Space:
 *   O(n) extra memory          (for L and R temp vectors)
 */
template<typename T>
void merge(std::vector<T>& v, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<T> l(n1), r(n2);

    for (int i = 0; i < n1; i++) l[i] = v[left + i];       // copy left half
    for (int i = 0; i < n2; i++) r[i] = v[mid + i + 1];    // copy right half

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {                             // merge both halves in order
        if (l[i] <= r[j]) v[k++] = l[i++];
        else               v[k++] = r[j++];
    }

    while (i < n1) v[k++] = l[i++];                        // remaining left elements
    while (j < n2) v[k++] = r[j++];                        // remaining right elements
}

template<typename T>
void mergeSort(std::vector<T>& v, int left, int right) {
    if (left >= right) return;                              // base case: 1 element

    int mid = (left + right) / 2;
    mergeSort(v, left, mid);                                // sort left half
    mergeSort(v, mid + 1, right);                           // sort right half
    merge(v, left, mid, right);                             // merge both halves
}

// ─────────────────────────────────────────────
//  Quick Sort   O(n log n) avg / O(n²) worst
// ─────────────────────────────────────────────
/*
 * Complexity:
 *   Best Case:    O(n log n) → pivot always splits array in half
 *   Average Case: O(n log n) → random order
 *   Worst Case:   O(n²)      → already sorted (pivot always smallest/largest)
 *
 * How it works:
 *   → pick pivot (leftmost element)
 *   → partition: move smaller elements left, larger elements right
 *   → recursively sort left and right sides
 *
 * Best Case Math:
 *   levels      = log n        (pivot always splits evenly)
 *   per level   = n            (partition touches every element)
 *   comparisons = n * log n    → O(n log n)
 *
 * Worst Case Math:
 *   levels      = n            (pivot always smallest/largest)
 *   per level   = n            (partition touches every element)
 *   comparisons = n * n        → O(n²)
 *
 * Space:
 *   Auxiliary         = O(1)        (no extra arrays, only variables)
 *   Stack Best/Avg    = O(log n)    (balanced splits → log n stack frames)
 *   Stack Worst       = O(n)        (unbalanced splits → n stack frames)
 */
template<typename T>
int partition(std::vector<T>& v, int left, int right) {
    T pivot = v[left];                                      // pick leftmost as pivot
    int i = left;                                           // i tracks last element < pivot

    for (int j = left + 1; j <= right; j++)
        if (v[j] < pivot)
            std::swap(v[j], v[++i]);                        // move smaller element to left side

    std::swap(v[left], v[i]);                               // place pivot in its correct position
    return i;
}

template<typename T>
void quickSort(std::vector<T>& v, int left, int right) {
    if (left >= right) return;                              // base case: 1 element

    int p = partition(v, left, right);
    quickSort(v, left, p - 1);                              // sort left of pivot
    quickSort(v, p + 1, right);                             // sort right of pivot
}

// ─────────────────────────────────────────────
//  Counting Sort   O(n + k)   [integers only]
// ─────────────────────────────────────────────
/*
 * Complexity:
 *   Best Case:    O(n + k) → k is the range of input (maxVal)
 *   Average Case: O(n + k) → same, doesn't depend on order
 *   Worst Case:   O(n + k) → same, always does all steps
 *
 * How it works:
 *   → count occurrences of each value    O(n)
 *   → accumulate counts (prefix sum)     O(k)
 *   → place elements in correct position O(n)
 *   → copy output back to v              O(n)
 *
 * When is it fast?
 *   k << n  (small range) → O(n)    ex: sorting ages 0-100
 *   k >> n  (large range) → O(k)    ex: sorting 10 numbers in range 0-1000000
 *
 * Note:
 *   → not comparison based! breaks O(n log n) barrier
 *   → only works on non-negative integers
 */
template<typename T>
void countingSort(std::vector<T>& v, int n) {
    if (n <= 1) return;

    T maxVal = *std::max_element(v.begin(), v.end());
    std::vector<int> count(maxVal + 1, 0);
    std::vector<T>   output(n);

    for (int i = 0; i < n; i++)
        count[v[i]]++;                                      // count occurrences of each value

    for (int i = 1; i <= maxVal; i++)
        count[i] += count[i - 1];                          // prefix sum → gives correct positions

    for (int i = n - 1; i >= 0; i--) {                     // traverse backwards → stable sort
        T   val    = v[i];
        int pos    = count[val] - 1;                        // final position of this element
        output[pos] = val;
        count[val]--;                                       // decrement so next duplicate goes left
    }

    for (int i = 0; i < n; i++)
        v[i] = output[i];                                  // copy sorted result back
}

// ─────────────────────────────────────────────
//  Radix Sort   O(n * d)   [integers only]
// ─────────────────────────────────────────────
/*
 * Complexity:
 *   Best Case:    O(n * d) → d is number of digits in maxVal
 *   Average Case: O(n * d) → same, doesn't depend on order
 *   Worst Case:   O(n * d) → same, always processes all digits
 *
 * How it works:
 *   → sort by each digit from least to most significant
 *   → uses counting sort for each digit pass
 *   → d passes total (d = number of digits in maxVal)
 *
 * Note:
 *   → not comparison based! breaks O(n log n) barrier
 *   → only works on non-negative integers
 *   → stable sort (preserves order of equal elements)
 *   → k is always 10 (digits 0-9) unlike counting sort
 */
template<typename T>
void countingForRadix(std::vector<T>& v, int n, int exp) {
    std::vector<int> count(10, 0);
    std::vector<T>   output(n);

    for (int i = 0; i < n; i++)
        count[(v[i] / exp) % 10]++;                        // count by current digit

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];                          // prefix sum

    for (int i = n - 1; i >= 0; i--) {                     // traverse backwards → stable sort
        int d = (v[i] / exp) % 10;
        output[--count[d]] = v[i];                         // place element at correct position
    }

    for (int i = 0; i < n; i++)
        v[i] = output[i];                                  // copy sorted result back
}

template<typename T>
void radixSort(std::vector<T>& v, int n) {
    if (n <= 1) return;
    T maxVal = *std::max_element(v.begin(), v.end());

    for (int exp = 1; maxVal / exp > 0; exp *= 10)         // one pass per digit
        countingForRadix(v, n, exp);
}

// ─────────────────────────────────────────────
//  Bucket Sort   O(n+k) avg / O(n²) worst   [floats in [0,1)]
// ─────────────────────────────────────────────
/*
 * Complexity:
 *   Best Case:    O(n + k) → elements uniformly distributed across buckets
 *   Average Case: O(n + k) → roughly uniform distribution
 *   Worst Case:   O(n²)    → all elements fall into same bucket
 *
 * How it works:
 *   → create n buckets for range [0, 1)
 *   → scatter elements into buckets by v[i] * n
 *   → sort each bucket with insertion sort
 *   → gather buckets back into v
 *
 * Note:
 *   → only works on floats in range [0, 1)
 *   → bucNo = v[i] * n maps float to bucket index
 *   → bucNo >= n check handles edge case when v[i] = 1.0
 */
template<typename T>
void insertionSortForBucket(std::vector<T>& v) {
    for (int i = 1, j; i < (int)v.size(); i++) {
        T tmp = v[i];
        for (j = i; j > 0 && v[j - 1] > tmp; j--)
            v[j] = v[j - 1];
        v[j] = tmp;
    }
}

template<typename T>
void bucketSort(std::vector<T>& v, int n) {
    if (n <= 1) return;

    std::vector<std::vector<T>> bucket(n);

    // scatter: map each element to its bucket
    for (int i = 0; i < n; i++) {
        int bucNo = (int)(v[i] * n);                       // bucket index = value * n
        if (bucNo >= n) bucNo = n - 1;                     // edge case: v[i] = 1.0
        bucket[bucNo].push_back(v[i]);
    }

    // sort each bucket individually
    for (int i = 0; i < n; i++)
        insertionSortForBucket(bucket[i]);

    // gather: collect elements back from all buckets in order
    int idx = 0;
    for (auto& b : bucket)                                 // each bucket row
        for (auto& x : b)                                  // each element in bucket
            v[idx++] = x;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<float> v(n);
    for (int i = 0; i < n; i++) {
        std::cin >> v[i];
    }

    bucketSort(v, n);
    printArray(v, n);
}