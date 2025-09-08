#include <iostream>
#include <string>

#include "item.hpp"

/*void QuickSort(Item** items, size_t count) {

    size_t i = 0;
    size_t j = count - 1;

    Item* mid = items[count-1];

    do {

        while (*items[i] < *mid) {
            i++;
        }
        while (!(*items[j] < *mid)) {
            j--;
        }
        if (i <= j) {
            Item* tmp = items[i];
            items[i] = items[j];
            items[j] = tmp;

            i++;
            j--;
        }
    } while (i <= j);

    if (j > 0) {
        QuickSort(items, j + 1);
    }
    if (i < count) {
        QuickSort(&items[i], count - i);
    }
}
*/

void Swap(Item** left, Item** right) {
    Item* tmp;

    tmp = *left;
    *left = *right;
    *right = tmp;
}

void Partition(Item** data, int left, int right, Item* middle, int* outLo, int* outHi) {

    int lo = left;
    int hi = right - 1;

    while (lo < hi) {
        if ((*data[lo] < *middle) || (*data[lo] == *middle))
            lo++;
        else if (!(*data[hi] < *middle) && (!(*data[hi] == *middle))) 
            hi--;
        else {
            Swap(data + lo, data + hi);
            lo++;
            hi--;
        }
    }

    *outHi = (!(*data[lo] < *middle) && (!(*data[lo] == *middle))) ? lo : lo + 1;

    hi = (!(*data[lo] < *middle) && (!(*data[lo] == *middle))) ? lo - 1 : lo;
    lo = left;

    while (lo < hi) {
        if (*data[lo] < *middle)
            lo++;
        else if (*data[hi] == *middle)
            hi--;

        else {
            Swap(data + lo, data + hi);
            lo++;
            hi--;
        }
    }

    *outLo = *data[lo] == *middle ? lo : lo + 1;
}


void QuickSort(Item** data, int left, int right) {
    int lo, hi;
    while (left + 1 < right) {
        Item* middle = data[left + rand() % (right - left)];
        Partition(data, left, right, middle, &lo, &hi);
        if (lo - left > right - hi) {
            QuickSort(data, hi, right);
            right = lo; 
        } else {
            QuickSort(data, left, lo);
            left = hi;
        }
    }
}



int main() {

    size_t count;

    printf("Enter count:\n");

    std::cin>>count;

    Real** mas = new Real*[count];

    printf("Enter array:\n");

    for (size_t i = 0; i < count; i++) {
        Real* a = new Real[sizeof(Real)];
        std::cin >> *a;
        mas[i] = a;
    }

    printf("Array:\n");

    for (size_t i = 0; i < count; i++) {
        std::cout << *mas[i] << std::endl;
    }
    QuickSort((Item**)mas, 0, count);

    for (size_t i = 0; i < count; i++) {
        std::cout << *mas[i] << std::endl;
    }
    for (size_t i = 0; i < count; i++) {
        delete[] mas[i];
    }
    delete[] mas;
}