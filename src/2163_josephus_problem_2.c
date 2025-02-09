#include <stdio.h>

#define MAX_N 200001
#define VALUES_PER_NODE 4

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
    //  1 points to value
    // int level;
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
    // int id = 1;

    for (i = 0; i < numValueNodes; i++)
    {
        dstIdx = i / 2;
        InnerNode *pNode = &innerNodes[dstIdx];

        valueNodes[i].pParent = pNode;

        if (i % 2 == 0)
        {
            // pNode->id = id++;
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
                    // printf("L: %d = %d + %d\n", s, pSubNode->leftSize, pSubNode->rightSize);
                    pTop = pInnerNode;
                    // pInnerNode->id = id++;
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
#if 0
                printf("S: %d", pInnerNode->id);
                printf("<- %d", ((InnerNode *)pInnerNode->pLeftSub)->id);
                if (i % 2 == 1)
                {
                    printf("-> %d", ((InnerNode *)pInnerNode->pRightSub)->id);
                }
                printf("\n");
#endif

                pSubNode->pParent = pInnerNode;
            }
            ++levels;
            len += rowLen;
            rowLen = rowIdx + 1;
            pSubNodes = pNewNodes;
            pNewNodes += rowLen;
#if 0
            for(int j=0; j < 3;j++) {
                //puts("-------");
            }
#endif
        }
    }
    else
    {
        pTop = &innerNodes[dstIdx];
    }
    // printf("S: %d|%d||%d|%d|%d\n", pTop->leftSize, pTop->rightSize, len, numValueNodes, levels);
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
            pN->pLeftValues = 0;
        }
    }
    if (pN->pRightValues == pNode)
    {
        pN->rightSize--;
        if (pN->rightSize == 0)
        {
            pN->pRightValues = 0;
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
    int levelValues[100][5];
    int debug = 0;

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
        int levelIdx = 0;
        if (debug > 0)
        {
            printf("\n|%d\n", i);
        }

        // find index
        InnerNode *pNode = pTop;
        int start = 0, value = 0;
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

        while (value == 0)
        {
            int c = 0;
            levelValues[levelIdx][4] = -1;

            if (i <= start + pNode->leftSize)
            {
                levelValues[levelIdx][0] = 'L';
                levelValues[levelIdx][2] = start + 1;
                levelValues[levelIdx][3] = start + pNode->leftSize;

                ValueNode *pVN = pNode->pLeftValues;
                if (pVN != 0)
                {
                    levelValues[levelIdx][1] = 'V';
                    // levelValues[levelIdx][3] = start;
                    start = i - start;
                    levelValues[levelIdx][4] = start;
                    if (debug > 0)
                    {
                        for (int j = 0; j < VALUES_PER_NODE; j++)
                        {
                            printf("%d ", pVN->values[j]);
                        }
                        puts("");
                    }

                    // printf("CL: %d@%d\n", i, start);

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
                        else if (debug > 0)
                        {
                            printf("...%d\n", j);
                        }
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
                    levelIdx++;
                    break;
                }
                else
                {
                    pNode = pNode->pLeftSub;
                    levelValues[levelIdx][1] = 'S';
                    levelIdx++;
                    continue;
                }
            }
            else
            {
                start += pNode->leftSize;
                levelValues[levelIdx][0] = 'R';

                levelValues[levelIdx][2] = start + 1;
                levelValues[levelIdx][3] = start + pNode->rightSize;

                ValueNode *pVN = pNode->pRightValues;
                if (pVN != 0)
                {
                    levelValues[levelIdx][1] = 'V';

                    start = i - start;
                    levelValues[levelIdx][4] = start;
                    // printf("CR: %d@%d\n", i, start);
                    if (debug > 0)
                    {
                        for (int j = 0; j < VALUES_PER_NODE; j++)
                        {
                            printf("%d ", pVN->values[j]);
                        }
                        puts("");
                    }

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
                        else if (debug > 0)
                        {
                            printf("...%d\n", j);
                        }
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
                    levelIdx++;
                    break;
                }
                else
                {
                    pNode = pNode->pRightSub;
                    levelValues[levelIdx][1] = 'S';
                    levelIdx++;
                    continue;
                }
            }
        }
        if (debug > 0)
        {
            for (int j = 0; j < levelIdx; j++)
            {
                printf("\n%c%c [%d %d]@%d\n", levelValues[j][0], levelValues[j][1], levelValues[j][2], levelValues[j][3], levelValues[j][4]);
            }
            puts("----");
        }

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
                }
            }
        }
    }

    if (debug > 0)
    {
        puts("");
        for (int j = 0; j < resIdx; j++)
        {
            printf("%d ", numbers[j]);
        }
        printf("\nS: %d\n", resIdx);
    }

#if 0
    i = k + 1;
    // int removed[MAX_N];
    // int remIdx = 0;
    int s = 1, numRemoved;
    if (i <= N)
    {
        // k++;
        while (k <= N)
        {
            int dst = i, src = i;
            numRemoved = 0;
            while (i <= N)
            {
                printf((s > 0) ? "%d" : " %d", numbers[i]);
                --s;
                ++numRemoved;
                i += k + 1;
            }
            i -= N;
            N -= numRemoved;

            for (; numRemoved > 0; numRemoved--)
            {
                // skip removed
                ++src;
                for (int j = 0; j < k; j++)
                {
                    numbers[dst] = numbers[src];
                    ++src;
                    ++dst;
                }
            }
            numRemoved = 0;
        }
        switch (N)
        {
        case 1:
            printf(" %d\n", numbers[1]);
            return 0;
        case 0:
            putchar('\n');
            return 0;
        default:
            break;
        }
    }

    if (k > N)
    {
        i = (i) % N;
        while (i == 0 && N > 1)
        {
            printf((s > 0) ? "%d" : " %d", numbers[N]);
            --N;
            i = (k + 1) % N;
        }
        int count = 0;
        while (N > 1)
        {
            int removed = 0;
            int start = i;
            int last = i - 1;
            while (last < i && i < N)
            {
                printf((s > 0) ? "%d" : " %d", numbers[i]);
                numbers[i] = -1; // mark as invalid
                last = i;
                --s;
                ++removed;
                i = (i + k + 1) % N;
                if (i == 0)
                {
                    printf((s > 0) ? "%d" : " %d", numbers[N]);
                    numbers[N] = -1;
                    --N;
                    i = (k + 1) % N;
                    break;
                }
            }

            int dst = start;
            int src = dst + 1;
            for (; src <= N; src++)
            {
                if (numbers[src] > 0)
                {
                    numbers[dst] = numbers[src];
                    ++dst;
                }
            }
            N -= removed;
#if 0
            printf("R:%d %d\n", removed, i);

            count++;
            if (count == 20)
            {
                break;
            }
#endif
        }
        if (N == 1)
        {
            printf(" %d", numbers[1]);
        }
    }
#endif

    putchar('\n');

    return 0;
}