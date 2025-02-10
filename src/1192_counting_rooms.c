#include <stdio.h>

#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

#if LOCAL_DEV_ENV
// #include <unistd.h>
#endif

#define MAX_HEIGHT 1000
#define MAX_WIDTH 1000

int map[MAX_HEIGHT * MAX_WIDTH + 1];
//@i*2 is min, @i*2+1 is max
int rowMinMax[MAX_HEIGHT * 2];

typedef struct
{
    int color;
    // if both values are identical, then it's a unique color
    // if not, then color 'should be colored' as realColor;
    int realColor;
} ColorState;

// Checkerboard pattern, approximately half of the space
//.#.
// #.#
//.#.
#define MAX_COLORS ((MAX_HEIGHT + 2) * (MAX_WIDTH + 2)) / 2
ColorState roomColors[MAX_COLORS];
int colorIdx = 0;

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
// Stores y,x pairs
int dfsStack[MAX_HEIGHT * MAX_WIDTH + 1][3];
int gMinY = 0;

#if LOCAL_DEV_ENV
int numChecks = 0;

void printMap(int height, int width, int used, int minY)
{
    fprintf(stderr, "IN: %d [%d\n", used, minY);

    for (int rowIdx = 0; rowIdx < height; rowIdx++)
    {
        for (int colIdx = 0; colIdx < width; colIdx++)
        {
            int checkIdx = rowIdx * width + colIdx;
            fprintf(stderr, "%d", map[checkIdx]);
        }
        fprintf(stderr, "\n");
    }
}
#endif

// To avoid stack errors we manage it
void depthFirstSearchWithManagedStack(int rowIdx, int colIdx, int height, int width)
{
    int offset = rowIdx * width + colIdx;
    if (map[offset] > 1)
    {
        return;
    }

    map[offset] = 2;
    int stackSize = 1;
    dfsStack[0][0] = rowIdx;
    dfsStack[0][1] = colIdx;
    dfsStack[0][2] = offset;

    while (stackSize > 0)
    {
        --stackSize;
        rowIdx = dfsStack[stackSize][0];
        colIdx = dfsStack[stackSize][1];
        offset = dfsStack[stackSize][2];
        map[offset] = 2;
#if LOCAL_DEV_ENV
        // fprintf(stderr, "!: %d %d\n", rowIdx, colIdx);
#endif
        for (int i = 0; i < 4; i++)
        {
            int newY = rowIdx + movements[i][0];
            if (newY < gMinY || newY >= height)
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
                if (map[offset] == 1)
                {
                    dfsStack[stackSize][0] = newY;
                    dfsStack[stackSize][1] = newX;
                    dfsStack[stackSize][2] = offset;
                    ++stackSize;
#if LOCAL_DEV_ENV
                    // fprintf(stderr, "!: %d %d\n", newY, newX);
#endif
                }
            }
        }
    }
}

int main()
{
    int colIdx, rowIdx, height, width, roomCount = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %d\n", &height, &width);
    int *pMap = &map[0];
    // min{X,Y} to speed up access later
    int minY = height, used = 0;
    char value;

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
                }
                used += value;
                *pMap = value;
                ++pMap;
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
#pragma GCC diagnostic pop

    if (used < 2)
    {
        printf("%d\n", used);
        return 0;
    }
    int checkIdx = 0;
    gMinY = minY;

#if LOCAL_DEV_ENV
    printMap(height, width, used, minY);
#endif

    // DFS works, but produces stack errors locally.
#if 0
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

    for (rowIdx = minY; rowIdx < height; rowIdx++)
    {
        int j = rowIdx << 1;
        int minX = rowMinMax[j];
        int maxX = rowMinMax[j + 1];
        //
        checkIdx = rowIdx * width + minX;
        pMap = &map[checkIdx];

        for (colIdx = minX; colIdx <= maxX; colIdx++)
        {
            if (*pMap == 1)
            {
                depthFirstSearchWithManagedStack(rowIdx, colIdx, height, width);
                ++roomCount;
            }
            ++pMap;
        }
    }

    printf("%d\n", roomCount);
#if LOCAL_DEV_ENV
    fprintf(stderr, "#checks: %d\n", numChecks);
#endif

    return 0;
}