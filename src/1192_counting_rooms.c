#include <stdio.h>

#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

#if LOCAL_DEV_ENV
#include <sys/time.h>
#endif

#define MAX_HEIGHT 1000
#define MAX_WIDTH 1000

#define USE_INT_MAP 0
#define USE_BITMAP 1

#define USE_DFS_STACK 0
#define USE_DFS_OFFSET_STACK 1

int map[MAX_HEIGHT * MAX_WIDTH + 1];

#if USE_BITMAP
unsigned int bitmap[MAX_HEIGHT * (MAX_WIDTH + 31) / 32 + 1];
#endif
//@i*2 is min, @i*2+1 is max
int rowMinMax[MAX_HEIGHT * 2];
int gMinY = 0;
int gMaxY = 0;

// Checkerboard pattern, approximately half of the space
//.#.
// #.#
//.#.
#define MAX_COLORS ((MAX_HEIGHT + 1) * (MAX_WIDTH + 1)) / 2
// indices 0 and 1 not used
// start with 2.
//- if value at index i is i, then it's valid
//- if value at index i is j and j is not i,
//   then it means that room with 'color' i actually has color at index j
int roomColors[MAX_COLORS];
int colorIdx = 0;

#if LOCAL_DEV_ENV
int numChecks = 0;
int gUsed = 0;

char formatStr[10];

void printMap(int height, int width, int space)
{
    fprintf(stderr, "IN: %d [%d\n", gUsed, gMinY);

    if (space > 0)
    {
        int n = sprintf(formatStr, " %%%dd", space);
        formatStr[n] = 0;
        fprintf(stderr, "Format:%s\n", formatStr);
    }

    for (int rowIdx = 0; rowIdx < height; rowIdx++)
    {
        for (int colIdx = 0; colIdx < width; colIdx++)
        {
            int checkIdx = rowIdx * width + colIdx;
            fprintf(stderr, space > 0 ? formatStr : " %d", map[checkIdx]);
        }
        fprintf(stderr, "\n");
    }
}
#endif

int horizonalMoves[] = {1, -1, 0, 0};
int verticalMoves[] = {0, 0, 1, -1};
int recursiveCall = 0;

void depthFirstSearch(int rowIdx, int colIdx, int height, int width)
{
    ++recursiveCall;
#if LOCAL_DEV_ENV
    fprintf(stderr, "?: %d %d | %d\n", rowIdx, colIdx, recursiveCall);
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
#if LOCAL_DEV_ENV
                fprintf(stderr, "Check?: %d %d\n", newY, newX);
#endif
                depthFirstSearch(newY, newX, height, width);
            }
        }
    }
    --recursiveCall;
}

int movements[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
#if USE_DFS_STACK
// Stores y,x pairs
int dfsStack[MAX_HEIGHT * MAX_WIDTH + 1][3];
#endif
#if USE_DFS_OFFSET_STACK
int dfsOffsetStack[MAX_HEIGHT * MAX_WIDTH + 1];
#endif

// To avoid stack errors we manage it
int depthFirstSearchWithManagedStack(int rowIdx, int colIdx, int height, int width)
{
    int offset = rowIdx * width + colIdx;
#if USE_INT_MAP
    if (map[offset] > 1)
    {
        return 0;
    }
    // map[offset] = 2;
#endif
#if USE_BITMAP
    int mapOffset = offset / 32;
    if ((bitmap[mapOffset] & ((unsigned int)0x1 << (offset % 32))) == 0)
    {
        return 0;
    }
#endif

    int stackSize = 1;
#if USE_DFS_STACK
    dfsStack[0][0] = rowIdx;
    dfsStack[0][1] = colIdx;
    dfsStack[0][2] = offset;
#endif
#if USE_DFS_OFFSET_STACK
    dfsOffsetStack[0] = offset;
#endif

    while (stackSize > 0)
    {
        --stackSize;
#if USE_DFS_STACK
        rowIdx = dfsStack[stackSize][0];
        colIdx = dfsStack[stackSize][1];
        offset = dfsStack[stackSize][2];
#endif
#if USE_DFS_OFFSET_STACK
        offset = dfsOffsetStack[stackSize];
        rowIdx = offset / width;
        colIdx = width > 1 ? offset % width : 0;
#endif
#if USE_INT_MAP
        map[offset] = 2;
#endif

#if USE_BITMAP
        mapOffset = offset / 32;
        bitmap[mapOffset] &= ~((unsigned int)0x1 << (offset % 32));
#endif
#if LOCAL_DEV_ENV
        // fprintf(stderr, "!: %d %d\n", rowIdx, colIdx);
#endif
        for (int i = 0; i < 4; i++)
        {
            int newY = rowIdx + movements[i][0];
            if (newY < gMinY || newY > gMaxY)
            {
                continue;
            }
            int j = newY << 1;
            int newX = colIdx + movements[i][1];

            if (rowMinMax[j] <= newX && newX <= rowMinMax[j + 1])
            {
                offset = newY * width + newX;
#if LOCAL_DEV_ENV
                ++numChecks;
#endif
#if USE_INT_MAP
                if (map[offset] == 1)
                {
#if USE_DFS_STACK
                    dfsStack[stackSize][0] = newY;
                    dfsStack[stackSize][1] = newX;
                    dfsStack[stackSize][2] = offset;
#endif
#if USE_DFS_OFFSET_STACK
                    dfsOffsetStack[stackSize] = offset;
#endif
                    ++stackSize;
                }
#endif
#if USE_BITMAP
                mapOffset = offset / 32;
                if ((bitmap[mapOffset] & ((unsigned int)0x1 << (offset % 32))) != 0)
                {
#if USE_DFS_STACK
                    dfsStack[stackSize][0] = newY;
                    dfsStack[stackSize][1] = newX;
                    dfsStack[stackSize][2] = offset;
#endif
#if USE_DFS_OFFSET_STACK
                    dfsOffsetStack[stackSize] = offset;
#endif
                    ++stackSize;
                }
#endif
            }
        }
    }
    return 1;
}

int solveWithColors(int height, int width)
{
    // int minY = gMinY;
    int roomCount = 0;
    colorIdx = 2;
    int minY = gMinY;
    if (minY >= height)
    {
        return 0;
    }
    int j = minY << 1;
    int columnX = rowMinMax[j], maxX = rowMinMax[j + 1];
    int *pMap = &map[minY * width + columnX];
    int prev = 0;
    // first row
    for (; columnX <= maxX; columnX++)
    {
#if LOCAL_DEV_ENV
        ++numChecks;
#endif
        if (*pMap == 1)
        {
            if (prev > 0)
            {
                *pMap = prev;
            }
            else
            {
                *pMap = colorIdx;
                roomColors[colorIdx] = colorIdx;
                ++colorIdx;
                if (colorIdx >= MAX_COLORS)
                {
                    printf("INDEX OUT OF RANGE: %d\n", colorIdx);
                    return 0;
                }
                roomColors[colorIdx] = 0;
            }
        }
        prev = *pMap;

        ++pMap;
    }
    const int maxY = gMaxY;
    int *pPrevMap;

    for (int rowIdx = minY + 1; rowIdx <= maxY; rowIdx++)
    {
        j = rowIdx << 1;
        int minX = rowMinMax[j];
        columnX = minX;
        maxX = rowMinMax[j + 1];
#if LOCAL_DEV_ENV
        ++numChecks;
#endif

        if (columnX <= maxX)
        {
            pMap = &map[(rowIdx)*width + columnX];
            pPrevMap = pMap - width;
            prev = *pPrevMap;

            // Nothing above in the 1.st column?
            if (prev == 0)
            {
                prev = colorIdx;
#if LOCAL_DEV_ENV
                // fprintf(stderr, "C: (%d,%d) %d\n", rowIdx, columnX, colorIdx);
#endif
                roomColors[colorIdx] = colorIdx;
                ++colorIdx;
                if (colorIdx >= MAX_COLORS)
                {
                    printf("INDEX OUT OF RANGE: %d\n", colorIdx);
                    return 0;
                }
                roomColors[colorIdx] = 0;
            }
            *pMap = prev;

            ++columnX;
            ++pMap;
            ++pPrevMap;

            for (; columnX <= maxX; columnX++)
            {
#if LOCAL_DEV_ENV
                ++numChecks;
#endif
                if (*pMap > 0)
                {
                    int prevAbove = *pPrevMap;

                    if (prevAbove > 0)
                    {
                        if (prev == 0)
                        {
                            *pMap = prevAbove;
                        }
                        else
                        {
                            if (prev < prevAbove)
                            {
                                int idx = roomColors[prevAbove];
                                roomColors[prevAbove] = prev;
                                while (roomColors[idx] > prev)
                                {
                                    int j = roomColors[idx];
                                    roomColors[idx] = prev;
                                    idx = j;
                                }
                                *pMap = prev;
                                // go back
                                idx = columnX;
                                int *pPrevInRow = pMap;
                                while (1 == 0 && idx >= minX && *pPrevInRow > 0)
                                {
                                    *pPrevInRow = prev;
                                    --pPrevInRow;
                                    --idx;
                                }
                            }
                            else
                            {
                                int idx = roomColors[prev];
                                roomColors[prev] = prevAbove;

                                while (roomColors[idx] > prevAbove)
                                {
                                    int j = roomColors[idx];
                                    roomColors[idx] = prevAbove;
                                    idx = j;
                                }
                                *pMap = prevAbove;
                                // go back
                                idx = columnX - 1;
                                int *pPrevInRow = pMap - 1;
                                while (idx >= minX && *pPrevInRow > 0)
                                {
                                    *pPrevInRow = prevAbove;
                                    --pPrevInRow;
                                    --idx;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (prev == 0)
                        {
#if LOCAL_DEV_ENV
                            // fprintf(stderr, "C: (%d,%d) %d\n", rowIdx, columnX, colorIdx);
#endif
                            *pMap = colorIdx;
                            roomColors[colorIdx] = colorIdx;
                            ++colorIdx;
                            if (colorIdx >= MAX_COLORS)
                            {
                                printf("INDEX OUT OF RANGE: %d\n", colorIdx);
                                return 0;
                            }
                            roomColors[colorIdx] = 0;
                        }
                        else
                        {
                            *pMap = prev;
                        }
                    }
                }
                prev = *pMap;
                ++pMap;
                ++pPrevMap;
            }
        }
    }

    for (int i = 2; i <= colorIdx; i++)
    {
        if (roomColors[i] > 0 && roomColors[i] == i)
        {
            ++roomCount;
        }
    }
#if LOCAL_DEV_ENV
    int s = 1;
    int ci = colorIdx;
    while (ci > 0)
    {
        ci /= 10;
        s++;
    }
    printMap(height, width, s);
#endif
    return roomCount;
}

int main()
{
    int colIdx, rowIdx, height, width, roomCount = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %d\n", &height, &width);

    // min{X,Y} to speed up access later
    int minY = height, maxY = 0, used = 0;
    char value;
#if USE_INT_MAP
    int *pMap = &map[0];
#endif
#if USE_BITMAP
    unsigned int *pBitMap = &bitmap[0];
    int offset = 0, mapOffset = 0;
    int lastOffset = -1;
#endif
#if LOCAL_DEV_ENV
    struct timeval begin, end;
    gettimeofday(&begin, 0);
#endif

    for (rowIdx = 0; rowIdx < height; rowIdx++)
    {
        value = ' ';
        int colMin = width, colMax = 0;
        //
        for (colIdx = 0; colIdx < width;)
        {
            scanf("%c", &value);
            if (value == '#' || value == '.')
            {
                value = (value == '#') ? 0 : 1;
                if (value > 0)
                {
                    if (colMin == width)
                    {
                        colMin = colIdx;
                    }
                    colMax = colIdx;
                    maxY = rowIdx;
                    ++used;
                }
#if USE_INT_MAP
                *pMap = value;
                ++pMap;
#endif

#if USE_BITMAP
                mapOffset = offset / 32;
                if (mapOffset != lastOffset)
                {
                    pBitMap[mapOffset] = (unsigned int)(value) << (offset % 32);
                    lastOffset = mapOffset;
                }
                else
                {
                    pBitMap[mapOffset] |= (unsigned int)(value) << (offset % 32);
                }
                ++offset;
#endif
                colIdx++;
            }
        }
        int j = rowIdx << 1;
        rowMinMax[j] = colMin;
        rowMinMax[j + 1] = colMax;

        if (used > 0 && minY == height)
        {
            minY = rowIdx;
        }
    }
#if LOCAL_DEV_ENV
    gettimeofday(&end, 0);

    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;

    fprintf(stderr, "#time: %f\n", elapsed);
#endif

#pragma GCC diagnostic pop

    if (used < 2)
    {
        printf("%d\n", used);
        return 0;
    }

    gMinY = minY;
    gMaxY = maxY;

#if LOCAL_DEV_ENV
    gUsed = used;
    // printMap(height, width, 0);
#endif

    // DFS works, but produces stack errors locally.
#if 0
    int checkIdx = 0;
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
#endif
#if 1
    for (rowIdx = minY; rowIdx < height; rowIdx++)
    {
        int j = rowIdx << 1;
        int minX = rowMinMax[j];
        int maxX = rowMinMax[j + 1];
        //
        // int checkIdx = rowIdx * width + minX;

        for (colIdx = minX; colIdx <= maxX; colIdx++)
        {
            roomCount += depthFirstSearchWithManagedStack(rowIdx, colIdx, height, width);
        }
    }
#endif
    // roomCount = solveWithColors(height, width);

    printf("%d\n", roomCount);
#if LOCAL_DEV_ENV
    fprintf(stderr, "#checks: %d\n", numChecks);
#endif

    return 0;
}