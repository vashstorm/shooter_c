/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : 1.cpp
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-06 14:33:42
 *  Version : 0.01
 * **************************************************************/

#include <iostream>
#include <unistd.h>
#include "../queue.h"
#include <vector>
using namespace std;

int main(int argc, char const* argv[])
{
    vector<int> vi {0,1,2,3,4,5,6,7,8,9};
    SH_QUEUE* sq = new SH_QUEUE;
    shq_init(sq);

    for (auto x: vi) {
        shq_push(sq, x);
    }

    int i;
    int res = 0;
    while (shq_size(sq))  {
        i = shq_pop(sq, &res);  
        printf("i [%d], res [%d]\n", i, res);
    }

    shq_destory(sq);

    delete sq;

    return 0;
}
