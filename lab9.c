#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Node for linked list
struct Node
{
    struct RecordType record;
    struct Node *next;
};

// HashType structure
struct HashType
{
    struct Node **array;
    int size;
};

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// Function to create a new node for the linked list
struct Node *createNode(struct RecordType data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode)
    {
        newNode->record = data;
        newNode->next = NULL;
    }
    return newNode;
}

// Function to insert a record into the hash table
void insertRecord(struct HashType *hTable, struct RecordType data)
{

    int index = hash(data.id, hTable->size);

    struct Node *newNode = createNode(data);

    if (hTable->array[index] == NULL)
    {
        hTable->array[index] = newNode;
    }
    else
    {
        struct Node *curr = hTable->array[index];
        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = newNode;
    }
}

// Function to display records in the hash structure
void displayRecordsInHash(struct HashType *hTable)
{
    printf("\nHash Table:\n");
    for (int i = 0; i < hTable->size; ++i)
    {
        printf("Index %d ->", i);
        struct Node *curr = hTable->array[i];
        while (curr != NULL)
        {
            printf(" %d %c %d ->", curr->record.id, curr->record.name, curr->record.order);
            curr = curr->next;
        }
        printf(" NULL\n");
    }
}

// Function to free memory used by the hash table
void freeHashTable(struct HashType *hTable)
{
    if (hTable)
    {
        for (int i = 0; i < hTable->size; ++i)
        {
            struct Node *curr = hTable->array[i];
            while (curr != NULL)
            {
                struct Node *temp = curr;
                curr = curr->next;
                free(temp);
            }
        }
        free(hTable->array);
        free(hTable);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);

        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create the hash table
    struct HashType *hTable = (struct HashType *)malloc(sizeof(struct HashType));
    if (hTable)
    {
        hTable->size = recordSz * 2;
        hTable->array = (struct Node **)calloc(hTable->size, sizeof(struct Node *));

        for (int i = 0; i < recordSz; ++i)
        {
            insertRecord(hTable, pRecords[i]);
        }

        displayRecordsInHash(hTable);

        freeHashTable(hTable);
    }
    else
    {
        printf("Cannot allocate memory for the hash table.\n");
    }

    free(pRecords);

    return 0;
}
