#include <stdio.h>

#define MAX_N 200001
#define VALUES_PER_NODE 8

typedef struct
{
    void *pParent;
    // number of valid cells
    int size;
    // removed values are negative
    int values[VALUES_PER_NODE];
} ValueNode;

typedef struct
{
    void *pParent;
    // int id;
    int leftSize;
    int rightSize;
    ValueNode *pLeftValues;
    ValueNode *pRightValues;
    void *pLeftSub;
    void *pRightSub;
} InnerNode;

int numbers[MAX_N];

#define MAX_LIMIT (MAX_N + VALUES_PER_NODE) / VALUES_PER_NODE

InnerNode innerNodes[MAX_LIMIT * 2];
ValueNode valueNodes[MAX_LIMIT];
int valueNodesUsed = 0;

InnerNode *pTop;

void josephine2(int N)
{
    int i;
    for (i = 1; i <= N; i++)
    {
        numbers[i] = i;
    }
    // We can use a tree or some special map
    // We add values in O(n*log(n))
    // We search and remove values in O(n*log(n))
    // We don't have a built-in tree or map in C
    // We can do it in a different way
    // We remove on every round every second values, i.e. half of values
    // We rearrange the values, this would require n/2 operations
    // I.e. we would remove and rearrange 1/2^i of the initial value
    // in every round 'i' and in the sum n values and we would require log(n) rounds.
    putchar('2');
    i = 4;

    // First round
    int removed = 1, j = 3;
    numbers[2] = 3;

    while (i <= N)
    {
        printf(" %d", numbers[i]);
        // rearrange
        numbers[j] = i + 1;
        ++j;
        i += 2;
        ++removed;
    }
    i -= N;
    N -= removed;

    while (N > 1)
    {
        removed = 0;
        int j = i;
        while (i <= N)
        {
            //"remove"
            printf(" %d", numbers[i]);
            // rearrange
            numbers[j] = numbers[i + 1];
            ++j;
            i += 2;
            ++removed;
        }
        i -= N;
        N -= removed;
    }
    if (N == 1)
    {
        printf(" %d\n", numbers[1]);
    }
    else
    {
        putchar('\n');
    }
}

void fillNodes(int N)
{
    int dstIdx = 0, i;
    // Fill value nodes
    for (i = 0; i < N; i++)
    {
        dstIdx = i / VALUES_PER_NODE;
        int j = i % VALUES_PER_NODE;
        valueNodes[dstIdx].values[j] = i + 1;
        valueNodes[dstIdx].size = j + 1;
    }

    const int numValueNodes = dstIdx + 1;
    valueNodesUsed = numValueNodes;

    for (i = 0; i < numValueNodes; i++)
    {
        dstIdx = i / 2;
        InnerNode *pNode = &innerNodes[dstIdx];

        valueNodes[i].pParent = pNode;

        if (i % 2 == 0)
        {
            pNode->pParent = 0;
            pNode->pLeftSub = 0;
            pNode->pRightSub = 0;
            pNode->leftSize = valueNodes[i].size;
            pNode->rightSize = 0;
            pNode->pLeftValues = &valueNodes[i];
            pNode->pRightValues = 0;
        }
        else
        {
            pNode->rightSize = valueNodes[i].size;
            pNode->pRightValues = &valueNodes[i];
        }
    }
    int levels = 1, len = dstIdx;
    //
    if (dstIdx > 0)
    {
        dstIdx++;

        len = 0;
        int rowIdx, rowLen = dstIdx;

        InnerNode *pLevelNodes = &innerNodes[0];
        InnerNode *pSubNodes = pLevelNodes;
        InnerNode *pNewNodes = pLevelNodes + rowLen;
        pTop = pSubNodes;

        while (rowLen > 1)
        {
            for (i = 0; i < rowLen; i++)
            {
                rowIdx = i / 2;
                InnerNode *pInnerNode = pNewNodes + rowIdx;
                InnerNode *pSubNode = pSubNodes + i;

                int s = pSubNode->leftSize + pSubNode->rightSize;

                if (i % 2 == 0)
                {
                    pTop = pInnerNode;
                    pInnerNode->pParent = 0;
                    pInnerNode->pLeftValues = 0;
                    pInnerNode->pRightValues = 0;
                    pInnerNode->leftSize = s;
                    pInnerNode->pLeftSub = pSubNode;
                    pInnerNode->pRightSub = 0;
                }
                else
                {
                    // printf("R: %d = %d + %d\n", s, pSubNode->leftSize, pSubNode->rightSize);
                    pInnerNode->rightSize = s;
                    pInnerNode->pRightSub = pSubNode;
                }

                pSubNode->pParent = pInnerNode;
            }
            ++levels;
            len += rowLen;
            rowLen = rowIdx + 1;
            pSubNodes = pNewNodes;
            pNewNodes += rowLen;
        }
    }
    else
    {
        pTop = &innerNodes[dstIdx];
    }
}

void decrementSize(ValueNode *pNode)
{
    pNode->size--;
    InnerNode *pN = pNode->pParent;

    if (pN->pLeftValues == pNode)
    {
        pN->leftSize--;
        if (pN->leftSize == 0)
        {
            //try to merge
            if (pN->pRightValues != 0)
            {
                pN->pLeftValues = pN->pRightValues;
                pN->leftSize = pN->rightSize;
                pN->rightSize = 0;
                pN->pRightValues = 0;
            }
            else
            {
                pN->pLeftValues = 0;
            }
        }
    }
    if (pN->pRightValues == pNode)
    {
        pN->rightSize--;
        //try to merge
        if (pN->rightSize == 0)
        {
            pN->pRightValues = 0;
        }
        else if (pN->pLeftValues == 0)
        {
            pN->pLeftValues = pN->pRightValues;
            pN->leftSize = pN->rightSize;
            pN->rightSize = 0;
        }
    }

    while (pN->pParent != 0)
    {
        InnerNode *pParent = (InnerNode *)pN->pParent;
        if ((pParent)->pLeftSub == pN)
        {
            --pParent->leftSize;
            if (pParent->leftSize == 0)
            {
                pParent->pLeftSub = 0;
            }
        }
        if ((pParent)->pRightSub == pN)
        {
            --pParent->rightSize;
            if (pParent->rightSize == 0)
            {
                pParent->pRightSub = 0;
            }
        }
        pN = pParent;
    }
}

#define STORE_LEVEL_INFO 0
#define ALLOW_DEBUG 0

int main()
{
    int i, N, k;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %d", &N, &k);
#pragma GCC diagnostic pop
    if (N == 1)
    {
        puts("1\n");
        return 0;
    }
    // Nothing to skip
    if (k == 0)
    {
        putchar('1');
        for (i = 2; i <= N; i++)
        {
            printf(" %d", i);
        }
        putchar('\n');
        return 0;
    }

    if (k == 1)
    {
        josephine2(N);
        return 0;
    }
    // Brute force is O(N^2)
    // But we can do in O(N*log(N))
    fillNodes(N);
#if STORE_LEVEL_INFO
    int levelValues[100][5];
#endif
    // int debug = 0;

    i = k + 1;
    int resIdx = 0, a = 0;
    while (N > 1)
    {
        if (a > 0)
        {
            i += k;
        }

        i = i % N;
        if (i == 0)
        {
            i = N;
        }
#if STORE_LEVEL_INFO
        int levelIdx = 0;
#endif
#if ALLOW_DEBUG
        if (debug > 0)
        {
            printf("\n|%d\n", i);
        }
#endif

        // find index
        InnerNode *pNode = pTop;
        int start = 0, value = 0;
#if ALLOW_DEBUG
        if (debug > 0)
        {
            puts("+++++");
            for (int j = 0; j < valueNodesUsed; j++)
            {
                putchar('[');
                for (int k = 0; k < VALUES_PER_NODE; k++)
                {
                    printf(" %d ", valueNodes[j].values[k]);
                }
                puts("]");
            }
            puts("+++++");
        }
#endif

        while (value == 0)
        {
            int c = 0;
#if STORE_LEVEL_INFO
            levelValues[levelIdx][4] = -1;
#endif

            if (i <= start + pNode->leftSize)
            {
#if STORE_LEVEL_INFO
                levelValues[levelIdx][0] = 'L';
                levelValues[levelIdx][2] = start + 1;
                levelValues[levelIdx][3] = start + pNode->leftSize;
#endif

                ValueNode *pVN = pNode->pLeftValues;
                if (pVN != 0)
                {
                    start = i - start;

#if STORE_LEVEL_INFO
                    levelValues[levelIdx][1] = 'V';
                    levelValues[levelIdx][4] = start;
#endif
#if ALLOW_DEBUG
                    if (debug > 0)
                    {
                        for (int j = 0; j < VALUES_PER_NODE; j++)
                        {
                            printf("%d ", pVN->values[j]);
                        }
                        puts("");
                    }
#endif

                    for (int j = 0; j < VALUES_PER_NODE; j++)
                    {
                        if (pVN->values[j] > 0)
                        {
                            --start;
                            if (start == 0)
                            {
                                c = pVN->values[j];
                                pVN->values[j] = -1;

                                break;
                            }
                        }
#if ALLOW_DEBUG
                        else if (debug > 0)
                        {
                            printf("...%d\n", j);
                        }
#endif
                    }
                    if (c > 0)
                    {
                        value = c;
                        decrementSize(pVN);
                    }
                    else
                    {
                        printf("FAILED:L: %d|%d\n", i, start);
                    }
#if STORE_LEVEL_INFO
                    levelIdx++;
#endif
                    break;
                }
                else
                {
                    pNode = pNode->pLeftSub;
#if STORE_LEVEL_INFO
                    levelValues[levelIdx][1] = 'S';

                    levelIdx++;
#endif
                    continue;
                }
            }
            else
            {
                start += pNode->leftSize;
#if STORE_LEVEL_INFO
                levelValues[levelIdx][0] = 'R';

                levelValues[levelIdx][2] = start + 1;
                levelValues[levelIdx][3] = start + pNode->rightSize;
#endif

                ValueNode *pVN = pNode->pRightValues;
                if (pVN != 0)
                {
                    start = i - start;
#if STORE_LEVEL_INFO
                    levelValues[levelIdx][1] = 'V';
                    levelValues[levelIdx][4] = start;
#endif
#if ALLOW_DEBUG
                    if (debug > 0)
                    {
                        for (int j = 0; j < VALUES_PER_NODE; j++)
                        {
                            printf("%d ", pVN->values[j]);
                        }
                        puts("");
                    }
#endif
                    for (int j = 0; j < VALUES_PER_NODE; j++)
                    {
                        if (pVN->values[j] > 0)
                        {
                            --start;
                            if (start == 0)
                            {
                                c = pVN->values[j];
                                pVN->values[j] = -1;
                                break;
                            }
                        }
#if ALLOW_DEBUG
                        else if (debug > 0)
                        {
                            printf("...%d\n", j);
                        }
#endif
                    }
                    if (c > 0)
                    {
                        value = c;
                        decrementSize(pVN);
                    }
                    else
                    {
                        printf("FAILED:R: %d|%d\n", i, start);
                    }
#if STORE_LEVEL_INFO
                    levelIdx++;
#endif
                    break;
                }
                else
                {
                    pNode = pNode->pRightSub;
#if STORE_LEVEL_INFO
                    levelValues[levelIdx][1] = 'S';
                    levelIdx++;
#endif
                    continue;
                }
            }
        }
#if STORE_LEVEL_INFO
#if ALLOW_DEBUG
        if (debug > 0)
        {
            for (int j = 0; j < levelIdx; j++)
            {
                printf("\n%c%c [%d %d]@%d\n", levelValues[j][0], levelValues[j][1], levelValues[j][2], levelValues[j][3], levelValues[j][4]);
            }
            puts("----");
        }
#endif
#endif

        if (value > 0)
        {
            printf((a == 0) ? "%d" : " %d", value);
            numbers[resIdx] = value;
            resIdx++;
        }
        else
        {
            break;
        }
        ++a;

        --N;
    }
    for (i = 0; i < valueNodesUsed; i++)
    {
        if (valueNodes[i].size > 0)
        {
            for (int j = 0; j < VALUES_PER_NODE; j++)
            {
                if (valueNodes[i].values[j] > 0)
                {
                    numbers[resIdx] = valueNodes[i].values[j];
                    resIdx++;
                    printf(" %d", valueNodes[i].values[j]);
                    i = valueNodesUsed;
                    break;
                }
            }
        }
    }

    putchar('\n');

#if ALLOW_DEBUG
    if (debug > 0)
    {
        puts("");
        for (int j = 0; j < resIdx; j++)
        {
            printf("%d ", numbers[j]);
        }
        printf("\nS: %d\n", resIdx);
    }
#endif

    return 0;
}