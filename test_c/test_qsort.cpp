#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int qsort(std::vector<char*>& str_vec, int start, int end) {
    printf("start:%d end:%d.\n", start, end);
    if (start >= end) {
        return 0;
    }

    char* pivot = str_vec[start];
    int i = start;
    int j = end;

    while (i<j) {
        while (j>i && strcmp(pivot, str_vec[j]) <= 0) {
            j--;
        }
        str_vec[i] = str_vec[j];


        while (j>i && strcmp(str_vec[i], pivot) <= 0) {
            i++;
        }
        str_vec[j] = str_vec[i];

    }
    str_vec[i] = pivot;

    for (int i=0; i<str_vec.size(); i++) {
        printf("%s\n", str_vec[i]);
    }

    qsort(str_vec, start, i-1);
    qsort(str_vec, i+1, end);
    return 0;
}

int sort(std::vector<char*>& str_vec) {
    int size = str_vec.size();
    qsort(str_vec, 0, size-1);

    for (int i=0; i<size; i++) {
        printf("%s\n", str_vec[i]);
    }
    printf("done.\n");
    fflush(stdout);

    return 0;
}

int main(int argc, char* argv[]) {
    std::vector<char*> str_vec;
    str_vec.push_back("4");
    str_vec.push_back("3");
    str_vec.push_back("2");
    str_vec.push_back("1");

    sort(str_vec);
    return 0;
}
