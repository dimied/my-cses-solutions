#include <stdio.h>

int main()
{
    int lastChar = -1, currentChar, currentCount = 0, maxCount = 0;

    while ((currentChar = getchar()) && (currentChar != '\n'))
    {
        if (currentChar == lastChar)
        {
            ++currentCount;
        }
        else
        {
            if (maxCount < currentCount)
            {
                maxCount = currentCount;
            }
            lastChar = currentChar;
            currentCount = 1;
        }
    }
    if (maxCount < currentCount)
    {
        maxCount = currentCount;
    }
    printf("%d\n", maxCount);

    return 0;
}