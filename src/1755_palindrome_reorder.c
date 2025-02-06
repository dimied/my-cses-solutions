#include <stdio.h>

#define NUMBER_OF_CHARS ('Z' - 'A' + 1)

int main()
{
    int characterCounters[NUMBER_OF_CHARS] = {0};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    int currentChar, oddIndex = -1;

    while ((currentChar = getchar()) && (currentChar != '\n'))
    {
        ++characterCounters[currentChar - 'A'];
    }
#pragma GCC diagnostic pop

    // We can't create a palindrom with multiple characters occuring odd number of times
    // And if there is one character occuring odd number of times,
    // it will be in the middle of the result.
    for (int i = 0; i < NUMBER_OF_CHARS; i++)
    {
        if (characterCounters[i] % 2 != 0)
        {
            if (oddIndex != -1)
            {
                printf("NO SOLUTION\n");
                return 0;
            }
            oddIndex = i;
        }
    }

    currentChar = 'A';
    int count, i = 0;

    // To avoid if (i != oddIndex)
    // on every iteration we use multiple loops
    for (; i < oddIndex; i++)
    {
        count = characterCounters[i] >> 1;
        while (count > 0)
        {
            printf("%c", currentChar);
            --count;
        }
        ++currentChar;
    }
    i = oddIndex + 1;
    currentChar = i + 'A';

    for (; i < NUMBER_OF_CHARS; i++)
    {
        count = characterCounters[i] >> 1;
        while (count > 0)
        {
            printf("%c", currentChar);
            --count;
        }
        ++currentChar;
    }

    i = NUMBER_OF_CHARS - 1;

    if (oddIndex >= 0)
    {
        currentChar = oddIndex + 'A';
        count = characterCounters[oddIndex];
        while (count > 0)
        {
            printf("%c", currentChar);
            --count;
        }
        currentChar = 'Z';

        for (; i > oddIndex; i--)
        {

            count = characterCounters[i] >> 1;
            while (count > 0)
            {
                printf("%c", currentChar);
                --count;
            }
            --currentChar;
        }
        --currentChar;
        --i;
    }
    else
    {
        currentChar = 'Z';
    }

    for (; i >= 0; i--)
    {
        count = characterCounters[i] >> 1;
        while (count > 0)
        {
            printf("%c", currentChar);
            --count;
        }
        --currentChar;
    }
    printf("\n");

    return 0;
}