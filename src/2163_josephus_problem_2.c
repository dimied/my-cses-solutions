#include <stdio.h>

#define MAX_N 200001
#define VALUES_PER_NODE 8

#define USE_ASSIGN_BITS 1

#if USE_ASSIGN_BITS && VALUES_PER_NODE < 33
#define DO_USE_ASSIGN_BITS 1
#else
#define DO_USE_ASSIGN_BITS 0
#endif

#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

typedef struct
{
    void *pParent;
    // number of valid cells
    int size;
#if DO_USE_ASSIGN_BITS
    unsigned int assigned;
#endif
    // removed values are negative
    int values[VALUES_PER_NODE];
} ValueNode;

typedef struct
{
    int leftSize;
    int rightSize;
    int state;
    ValueNode *pLeftValues;
    ValueNode *pRightValues;
    void *pLeftSub;
    void *pRightSub;
    void *pParent;
} InnerNode;

int numbers[MAX_N];

#define MAX_LIMIT (MAX_N + VALUES_PER_NODE) / VALUES_PER_NODE

InnerNode innerNodes[MAX_LIMIT + 20];
ValueNode valueNodes[MAX_LIMIT];

int valueNodesUsed = 0, numMerges = 0, numMerges2 = 0;
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

#if DO_USE_ASSIGN_BITS
#define ALL_ASSIGN_BITS_SET ((1UL << VALUES_PER_NODE) - 1)
#endif
/**
 * Fills the nodes with values
 */
void fillNodes(int N)
{
    int dstIdx = 0, i;
#if DO_USE_ASSIGN_BITS

#endif
    // Fill value nodes
    for (i = 0; i < N; i++)
    {
        dstIdx = i / VALUES_PER_NODE;
        int j = i % VALUES_PER_NODE;
        valueNodes[dstIdx].values[j] = i + 1;
        valueNodes[dstIdx].size = j + 1;
#if DO_USE_ASSIGN_BITS
        valueNodes[dstIdx].assigned = (unsigned int)ALL_ASSIGN_BITS_SET;
#endif
    }

#if DO_USE_ASSIGN_BITS
    // need to adjust just the last one
    valueNodes[dstIdx].assigned = (unsigned int)((1UL << valueNodes[dstIdx].size) - 1);
#endif

    const int numValueNodes = dstIdx + 1;
    valueNodesUsed = numValueNodes;
    ValueNode *pValueNode = &valueNodes[0];

    for (i = 0; i < numValueNodes; i++)
    {
        dstIdx = i / 2;
        InnerNode *pNode = &innerNodes[dstIdx];

        pValueNode->pParent = pNode;

        if (i % 2 == 0)
        {
            pNode->pParent = 0;
            pNode->pLeftSub = 0;
            pNode->pRightSub = 0;
            pNode->leftSize = pValueNode->size;
            pNode->rightSize = 0;
            pNode->pLeftValues = pValueNode;
            pNode->pRightValues = 0;
        }
        else
        {
            pNode->rightSize = pValueNode->size;
            pNode->pRightValues = pValueNode;
        }
        ++pValueNode;
    }
    //
    if (dstIdx > 0)
    {
        dstIdx++;

        int rowIdx, rowLen = dstIdx;

        InnerNode *pSubNodes = &innerNodes[0];
        InnerNode *pNewNodes = pSubNodes + rowLen;
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
                    pInnerNode->rightSize = s;
                    pInnerNode->pRightSub = pSubNode;
                }

                pSubNode->pParent = pInnerNode;
            }
            //++levels;
            // len += rowLen;
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
    int merged = 0, numChildren = 0;

    if (pN->pLeftValues == pNode)
    {
        pN->leftSize--;
        ++numChildren;
        if (pN->leftSize == 0)
        {
            // try to merge
            if (pN->pRightValues != 0)
            {
                pN->leftSize = pN->rightSize;
                pN->rightSize = 0;
                pN->pLeftValues = pN->pRightValues;
                pN->pRightValues = 0;

                ++merged;
            }
            else
            {
                --numChildren;
                pN->pLeftValues = 0;
            }
        }
    }

    if (pN->pRightValues == pNode)
    {
        pN->rightSize--;
        ++numChildren;
        // try to merge
        if (pN->rightSize == 0)
        {
            pN->pRightValues = 0;
            --numChildren;
        }
        else if (pN->pLeftValues == 0)
        {
            pN->leftSize = pN->rightSize;
            pN->rightSize = 0;
            pN->pLeftValues = pN->pRightValues;
            pN->pRightValues = 0;
            ++merged;
        }
    }

#if LOCAL_DEV_ENV
    numMerges += merged;
#endif

    if (numChildren == 2 && (pN->leftSize + pN->rightSize) <= VALUES_PER_NODE)
    {
#if LOCAL_DEV_ENV
        ++numMerges2;
#endif
    }

    // Update the values in the path
    while (pN->pParent != 0)
    {
        InnerNode *pParent = (InnerNode *)pN->pParent;
        if (pParent->pLeftSub == pN)
        {
            --pParent->leftSize;
            if (pParent->leftSize == 0)
            {
                pParent->pLeftSub = 0;
                if (pParent->pRightSub != 0)
                {
                    pParent->pLeftSub = pParent->pRightSub;
                    pParent->pRightSub = 0;
                    pParent->leftSize = pParent->rightSize;
                    pParent->rightSize = 0;
                }
            }
        }
        else if (pParent->pRightSub == pN)
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
#define ALLOW_DEBUG 0
#define PRINT_BUFFER_SIZE 32
#define PRINT_INC 4
#define PRINT_LINE(BUF, IDX, F) printf(F, BUF[IDX], BUF[IDX + 1], BUF[IDX + 2], BUF[IDX + 3]);

char *printFormats[2] = {
    "%d %d %d %d",
    " %d %d %d %d",
};

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
    //
    // But we can do it in O(N*log(N))
    //-we build a tree in O(N)
    //-we remove every item in O(log(n))
    fillNodes(N);

    i = k + 1;
    int a = 0;
    int printBuffer[PRINT_BUFFER_SIZE];
    int printIdx = 0;

    while (N > 0)
    {
        i = i % N;

        if (i == 0)
        {
            i = N;
        }

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
            if (i <= start + pNode->leftSize)
            {
                ValueNode *pVN = pNode->pLeftValues;
                if (pVN != 0)
                {
                    start = i - start;

#if DO_USE_ASSIGN_BITS
                    int j = -1;
                    unsigned int assignment = pVN->assigned;
                    while (start > 0 && assignment > 0)
                    {
                        start -= (int)(assignment & 0x1);
                        assignment >>= 1;
                        ++j;
                    }
                    if (j >= 0)
                    {
                        pVN->assigned &= ~(0x1 << j);
                        value = pVN->values[j];
                        pVN->values[j] = -1;
                    }
#else
                    int j = 0;
                    for (; j < VALUES_PER_NODE; j++)
                    {
                        if (pVN->values[j] > 0)
                        {
                            --start;
                            if (start == 0)
                            {
                                value = pVN->values[j];
                                pVN->values[j] = -1;

                                break;
                            }
                        }
                    }
#endif

                    if (value > 0)
                    {
                        decrementSize(pVN);
                    }
                    else
                    {
                        printf("FAILED:L: %d|%d\n", i, start);
                    }
                    break;
                }
                else
                {
                    pNode = pNode->pLeftSub;
                    continue;
                }
            }
            else
            {
                start += pNode->leftSize;
                ValueNode *pVN = pNode->pRightValues;

                if (pVN != 0)
                {
                    start = i - start;

#if DO_USE_ASSIGN_BITS
                    int j = -1;
                    unsigned int assignment = pVN->assigned;
                    while (start > 0 && assignment > 0)
                    {
                        start -= (int)(assignment & 0x1);
                        assignment >>= 1;
                        ++j;
                    }
                    if (j >= 0)
                    {
                        pVN->assigned &= (~(0x1 << j));
                        value = pVN->values[j];
                        pVN->values[j] = -1;
                    }
#else
                    int j = 0;
                    for (; j < VALUES_PER_NODE; j++)
                    {
                        if (pVN->values[j] > 0)
                        {
                            --start;
                            if (start == 0)
                            {
                                value = pVN->values[j];
                                pVN->values[j] = -1;

                                break;
                            }
                        }
                    }
#endif
                    if (value > 0)
                    {
                        decrementSize(pVN);
                    }
                    else
                    {
                        printf("FAILED:R: %d|%d\n", i, start);
                    }
                    break;
                }
                else
                {
                    pNode = pNode->pRightSub;
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
            printBuffer[printIdx] = value;
            ++printIdx;
        }
        else
        {
            break;
        }
        if (printIdx == PRINT_BUFFER_SIZE)
        {
            for (int j = 0; j < PRINT_BUFFER_SIZE; j += PRINT_INC)
            {
                PRINT_LINE(printBuffer, j, printFormats[a]);
                a = 1;
            }
            printIdx = 0;
        }
        i += k;
        --N;
    }
    // Print the rest
    for (int j = 0; j < printIdx; j++)
    {
        if (a != 0)
        {
            putchar(' ');
        }
        printf("%d", printBuffer[j]);
        ++a;
    }

#if 0
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
#endif

    putchar('\n');

#if LOCAL_DEV_ENV
    fprintf(stderr, "#merges: %d|%d\n", numMerges, numMerges2);
#endif

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