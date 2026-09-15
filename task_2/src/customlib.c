#include"customlib.h"

int add(int a,int b)
{
    int sum=a+b;
    if(sum==7)
    {
        return addThree(sum);
    }
    else
    {
        return sum;
    }
}

int addThree(int num)
{
    return num+3;
}
