//
// Created by 16477 on 2023/9/12.
//


#include "stdio.h"
#include "omp.h"

int main() {

#pragma omp parallel
    {
        printf("Hello World!\n");
    }
    return 0;
}