#include <stdio.h>

#define MAX_HEIGHT 1000
#define MAX_WIDTH 1000

int map[MAX_HEIGHT * MAX_WIDTH+1];

int horizonalMoves[] = {1, -1, 0, 0};
int verticalMoves[] = {0, 0, 1, -1};
int recursiveCall = 0;

void depthFirstSearch(int rowIdx, int colIdx, int height, int width)
{
    ++recursiveCall;
#ifdef LOCAL_DEV_ENV
#if LOCAL_DEV_ENV
    fprintf(stderr, "?: %d %d | %d\n", rowIdx, colIdx, recursiveCall);
#endif
#endif
    map[rowIdx * width + colIdx] = 2;

    for (int i = 0; i < 4; i++)
    {
        int newX = colIdx + horizonalMoves[i];
        int newY = rowIdx + verticalMoves[i];

        if (0 <= newX && newX < width && 0 <= newY && newY < height)
        {
            int offset = newY * width + newX;
            if (map[offset] == 1)
            {
#ifdef LOCAL_DEV_ENV
#if LOCAL_DEV_ENV
                fprintf(stderr, "Check?: %d %d\n", newY, newX);
#endif
#endif
                depthFirstSearch(newY, newX, height, width);
            }
        }
    }
    --recursiveCall;
}

int main()
{
    int colIdx, rowIdx, height, width, roomCount = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %d\n", &height, &width);
    int *pMap = &map[0];
    //min* to speed up access later
    int minX = width, minY = height, used = 0;
    char value;

    for (rowIdx = 0; rowIdx < height; rowIdx++)
    {
        *pMap = 3;
        value = ' ';
        for (colIdx = 0; colIdx < width;)
        {
            scanf("%c", &value);
            if (value == '#' || value == '.')
            {
                value = (value == '#') ? 0 : 1;
                if (value > 0)
                {
                    if (minX > colIdx)
                    {
                        minX = colIdx;
                    }
                }
                used += value;
                *pMap = value;
                ++pMap;
                colIdx++;
            }
        }
        
        if (used > 0 && minY == height)
        {
            minY = rowIdx;
        }
    }
    int checkIdx = 0;

#ifdef LOCAL_DEV_ENV
#if LOCAL_DEV_ENV
    fprintf(stderr, "IN: %d [%d, %d\n", used, minY, minX);

    for (rowIdx = 0; rowIdx < height; rowIdx++)
    {
        for (colIdx = 0; colIdx < width; colIdx++)
        {
            checkIdx = rowIdx * width + colIdx;
            fprintf(stderr,"%d", map[checkIdx]);
        }
        fprintf(stderr,"\n");
    }
#endif
#endif
    
    //DFS works, but produces stack errors locally.
    for (rowIdx = minY; rowIdx < height; rowIdx++)
    {
        checkIdx = rowIdx * width + minX;
        pMap = &map[checkIdx];
        for (colIdx = minX; colIdx < width; colIdx++)
        {
            if (*pMap == 1)
            {
                depthFirstSearch(rowIdx, colIdx, height, width);
                roomCount++;
            }
            ++pMap;
        }
    }

    printf("%d\n", roomCount);

    return 0;
}