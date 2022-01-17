#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

// BUTTERFLY Vs SPIDER
// HAMIDREZA GANDOMI

//                                     CONSTANTS
#define QUEUE_SIZE 100

//                                     QUEUE
struct queue
{
    int items[QUEUE_SIZE];
    int front;
    int rear;
};

struct queue *createQueue();
void enqueue(struct queue *q, int);
int dequeue(struct queue *q);
int isEmpty(struct queue *q);
void printQueue(struct queue *q);

//                                     NODE
struct Node
{
    int vertex;
    struct Node *next;
};

struct Node *createNode(int);

//                                     GRAPH
struct Graph
{
    int verticesCount;
    struct Node **adjacents;
    int *visited;
};

struct Graph *createGraph(int vertices);
void resetVisited(struct Graph *graph);
void addEdge(struct Graph *graph, int src, int dest);
int removeEdge(struct Graph *graph, int src, int dest);
struct Node *bfs(struct Graph *graph, int startVertex);
struct Node *backtrace(int *parents, int startVertex, int endVertex);

//                                     etc
void getInputs();
int linearSearch(int *array, int size, int key);
void StartGameAsButterfly();
void startGameAsSpider();
void printLinkedList(struct Node *head);
int searchLinkedList(struct Node *head, int key);
void gotoxy(int x, int y);

//                                     VARIABLES
struct Graph *graph;

int **map;
int *butterfliesLocation;
int spiderLocation;

int mapSize;
int spidersCount;
int butterfliesCount;

int gameType;
char MapAddress[500];
char InputType[1];
int FileMatrix[100][100];
int FileButterflies[100];

//                                     MAIN
int main()
{

    getInputs();

    if (gameType == 1)
        StartGameAsButterfly();
    else if (gameType == 2)
        startGameAsSpider();
    return 0;
}

//                                     QUEUE
struct queue *createQueue()
{
    struct queue *q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

int isEmpty(struct queue *q)
{
    if (q->rear == -1)
        return 1;
    else
        return 0;
}

void enqueue(struct queue *q, int value)
{
    if (q->rear == QUEUE_SIZE - 1)
        printf("\nQueue is Full!!");
    else
    {
        if (q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}

int dequeue(struct queue *q)
{
    int item;
    if (isEmpty(q))
    {
        item = -1;
    }
    else
    {
        item = q->items[q->front];
        q->front++;
        if (q->front > q->rear)
        {
            q->front = q->rear = -1;
        }
    }
    return item;
}

void printQueue(struct queue *q)
{
    int i = q->front;
    if (isEmpty(q))
    {
        printf("Queue is empty");
    }
    else
    {
        printf("\nQueue contains \n");
        for (i = q->front; i < q->rear + 1; i++)
        {
            printf("%d ", q->items[i]);
        }
    }
}

//                                     NODE
struct Node *createNode(int v)
{
    struct Node *newNode = malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

//                                     GRAPH
struct Graph *createGraph(int vertices)
{
    struct Graph *graph = malloc(sizeof(struct Graph));

    graph->verticesCount = vertices;
    graph->adjacents = malloc(vertices * sizeof(struct Node *));
    graph->visited = malloc(vertices * sizeof(int));

    int i;
    for (i = 0; i < vertices; i++)
    {
        graph->adjacents[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;
}

void resetVisited(struct Graph *graph)
{
    graph->visited = malloc(graph->verticesCount * sizeof(int));

    for (int i = 0; i < graph->verticesCount; i++)
        graph->visited[i] = 0;
}

void addEdge(struct Graph *graph, int src, int dest)
{
    // Add edge from src to dest
    struct Node *newNode = createNode(dest);
    newNode->next = graph->adjacents[src];
    graph->adjacents[src] = newNode;

    // Add edge from dest to src
    newNode = createNode(src);
    newNode->next = graph->adjacents[dest];
    graph->adjacents[dest] = newNode;
}

int removeEdge(struct Graph *graph, int src, int dest)
{
    // Remove edge from src to dest
    struct Node *p = graph->adjacents[src];

    if (p->vertex == dest)
    {
        graph->adjacents[src] = p->next;
    }
    else
    {
        while (p->next && p->next->vertex != dest)
            p = p->next;

        if (!p->next)
            return 0;

        p->next = p->next->next;
    }

    // Remove edge from dest to src
    p = graph->adjacents[dest];

    if (p->vertex == src)
    {
        graph->adjacents[dest] = p->next;
    }
    else
    {
        while (p->next && p->next->vertex != src)
            p = p->next;

        if (!p->next)
            return 0;

        p->next = p->next->next;
    }

    return 1;
}

struct Node *bfs(struct Graph *graph, int startVertex)
{
    resetVisited(graph);

    int *parents = (int *)malloc(sizeof(int) * mapSize);

    struct queue *queue = createQueue();

    graph->visited[startVertex] = 1;
    enqueue(queue, startVertex);

    while (!isEmpty(queue))
    {
        int currentVertex = dequeue(queue);

        if (linearSearch(butterfliesLocation, butterfliesCount, currentVertex))
            return backtrace(parents, startVertex, currentVertex);

        struct Node *temp = graph->adjacents[currentVertex];

        while (temp)
        {
            int adjacentVertex = temp->vertex;

            if (graph->visited[adjacentVertex] == 0)
            {
                graph->visited[adjacentVertex] = 1;
                parents[adjacentVertex] = currentVertex;
                enqueue(queue, adjacentVertex);
            }

            temp = temp->next;
        }
    }

    return NULL;
}

struct Node *backtrace(int *parents, int startVertex, int endVertex)
{
    int currentVertex = endVertex;

    struct Node *path = createNode(currentVertex);

    while (currentVertex != startVertex)
    {
        currentVertex = parents[currentVertex];

        struct Node *node = createNode(currentVertex);

        node->next = path;
        path = node;
    }

    return path;
}

//                                     etc
void getInputs()
{

    // Change console text color
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    // Save current attributes
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    // Change console text color to green
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    gotoxy(50, 0);
    printf("BUTTERFLY Vs SPIDER");
    gotoxy(45, 1);
    printf("-----------------------------\n\n");

    // Change console text color to intensity
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);

    // Get Input Type
    printf("Do you want enter input manually or read from file ( manually = m \\ file = f ) : ");
    scanf("%s", InputType);
    printf("------------------------------\n");

    if (strcmp(InputType, "m") == 0)
    {
        // Get map Size
        printf("Enter the nod number of the graph : ");
        scanf("%d", &mapSize);

        // Set map's size
        map = (int **)malloc(sizeof(int) * mapSize);
        for (int i = 0; i < mapSize; i++)
            map[i] = (int *)malloc(sizeof(int) * mapSize);

        // Set graph's size
        graph = createGraph(mapSize);

        // Get map elements
        printf("Enter the nods :\n");
        for (int i = 0; i < mapSize; i++)
        {
            for (int j = 0; j < mapSize; j++)
            {
                scanf("%d", &map[i][j]);

                if (i > j && map[i][j])
                    addEdge(graph, i, j);
            }
        }

        // Get spiders Count
        printf("Enter the number of the Spiders : ");
        scanf("%d", &spidersCount);

        // Get spiders Location
        printf("Enter the location of the Spiders : ");
        scanf("%d", &spiderLocation);
        spiderLocation--;

        // Get butterflies Count
        printf("Enter the number of the Butterflies : ");
        scanf("%d", &butterfliesCount);

        // Set butterfliesLocation's size
        butterfliesLocation = (int *)malloc(sizeof(int) * butterfliesCount);

        // Get butterflies Location
        printf("Enter the location of the Butterflies : ");
        if (butterfliesCount == 1)
        {
            int i = 0;
            scanf("%d", &butterfliesLocation[i]);
            butterfliesLocation[i]--;
            printf("Do you want play for Spider or Butterfly ( for butterfly type 1 // for spider type 2 ) : ");
        }
        else if (butterfliesCount > 1)
        {
            for (int i = 0; i < butterfliesCount; i++)
            {
                scanf("%d ", &butterfliesLocation[i]);
                butterfliesLocation[i]--;
                if (i == butterfliesCount - 2)
                {
                    printf("Do you want play for Spider or Butterfly ( for butterfly type 1 // for spider type 2 ) : ");
                }
            }
        }
        // Get game Type
        scanf("%d", &gameType);
    }

    else if (strcmp(InputType, "f") == 0)
    {
        FILE *mapfile;
        int cant_read_map = 0;
        while (1)
        {

            printf("Type the map file's address : ");
            scanf("%s", MapAddress);
            if ((mapfile = fopen(MapAddress, "r")) == NULL)
            {
                cant_read_map = 1;
                printf("~ Can't read file, so I ask you again..\n\n");
            }
            else
            {
                break;
            }
        }
        if (cant_read_map == 0)
        {

            printf(".\n.\n.");
            // Get map Size
            fscanf(mapfile, "%d", &mapSize);

            // Set map's size
            map = (int **)malloc(sizeof(int) * mapSize);
            for (int i = 0; i < mapSize; i++)
                map[i] = (int *)malloc(sizeof(int) * mapSize);

            // Set graph's size
            graph = createGraph(mapSize);

            // Get map elements
            for (int m = 0; m < mapSize; m++)
            {
                for (int n = 0; n < mapSize; n++)
                {
                    fscanf(mapfile, "%d", &FileMatrix[m][n]);
                    map[m][n] = FileMatrix[m][n];
                    if (m > n && map[m][n])
                        addEdge(graph, m, n);
                }
            }

            // Get spiders Count
            fscanf(mapfile, "%d", &spidersCount);

            // Get spiders Location
            fscanf(mapfile, "%d", &spiderLocation);
            spiderLocation--;

            // Get butterflies Count
            fscanf(mapfile, "%d", &butterfliesCount);

            // Set butterfliesLocation's size
            butterfliesLocation = (int *)malloc(sizeof(int) * butterfliesCount);

            // Get butterflies Location
            for (int i = 0; i < butterfliesCount; i++)
            {
                fscanf(mapfile, "%d", &FileButterflies[i]);
                FileButterflies[i]--;
                butterfliesLocation[i] = FileButterflies[i];
            }

            // Get gameType
            fscanf(mapfile, "%d", &gameType);

            fclose(mapfile);
        }
    }
    else
    {
        printf("Your type input is wrong ... ");
    }
    // Restore original colors
    SetConsoleTextAttribute(hConsole, saved_attributes);
}

int linearSearch(int *array, int size, int key)
{
    for (int i = 0; i < size; i++)
        if (array[i] == key)
            return 1;

    return 0;
}

void StartGameAsButterfly()
{
    struct Node *path;
    int nextLocation;
    int firstVertex;
    int secondVertex;

    for (int i = 1;; i += 2)
    {
        if (!linearSearch(butterfliesLocation, butterfliesCount, spiderLocation))
        {
            printf("\n------------------------------\n");
            printf("Turn %d\n", i);

            path = bfs(graph, spiderLocation);

            if (path)
            {
                nextLocation = path->next->vertex;

                printf("Spider moved from node %d to node %d\n", spiderLocation + 1, nextLocation + 1);

                spiderLocation = nextLocation;
            }
            else
            {
                printf("\nBUTTERFLIES WON !!!\n");
                break;
            }
        }

        if (linearSearch(butterfliesLocation, butterfliesCount, spiderLocation))
        {
            printf("\nSPIDER WON !!!\n");
            break;
        }

        printf("\n------------------------------\n");

        printf("Enter the first  vertex of the edge to remove: ");
        scanf("%d", &firstVertex);
        printf("Enter the second vertex of the edge to remove: ");
        scanf("%d", &secondVertex);

        while (!removeEdge(graph, firstVertex - 1, secondVertex - 1))
        {
            printf("\n");
            printf("Please enter valid edge vertices !\n");
            printf("\n");

            printf("Enter the first  vertex of the edge to remove: ");
            scanf("%d", &firstVertex);
            printf("Enter the second vertex of the edge to remove: ");
            scanf("%d", &secondVertex);
        }

        printf("\n------------------------------\n");
        printf("Turn %d\n", i + 1);
        printf("Butterflies removed the edge between node %d to node %d\n", firstVertex, secondVertex);
    }
}

void startGameAsSpider()
{
    struct Node *path;
    int nextLocation;
    int firstVertex;
    int secondVertex;

    for (int i = 1;; i += 2)
    {
        path = bfs(graph, spiderLocation);

        if (path)
        {
            while (path->next->next)
                path = path->next;

            firstVertex = path->vertex;
            secondVertex = path->next->vertex;

            removeEdge(graph, firstVertex, secondVertex);

            printf("\n------------------------------\n");
            printf("Turn %d\n", i);
            printf("Butterflies removed the edge between node %d to node %d\n", firstVertex + 1, secondVertex + 1);

            path = bfs(graph, spiderLocation);
        }

        if (!path)
        {
            printf("\nBUTTERFLIES WON !!!\n");
            break;
        }

        printf("\n------------------------------\n");

        printf("Enter the vertex for spider next move: ");
        scanf("%d", &nextLocation);

        while (!searchLinkedList(graph->adjacents[spiderLocation], nextLocation - 1))
        {
            printf("\n");
            printf("Please enter valid vertex !\n");
            printf("\n");

            printf("Enter the vertex for spider next move: ");
            scanf("%d", &nextLocation);
        }

        printf("\n------------------------------\n");
        printf("Turn %d\n", i + 1);
        printf("Spider moved from node %d to node %d\n", spiderLocation + 1, nextLocation);

        spiderLocation = nextLocation - 1;

        if (linearSearch(butterfliesLocation, butterfliesCount, spiderLocation))
        {
            printf("\nSPIDER WON !!!\n");
            break;
        }
    }
}

void printLinkedList(struct Node *head)
{
    struct Node *p = head;

    printf("%d", p->vertex);

    p = p->next;

    while (p)
    {
        printf(" -> %d", p->vertex);
        p = p->next;
    }

    printf("\n");
}

int searchLinkedList(struct Node *head, int key)
{
    struct Node *p = head;

    while (p)
    {
        if (p->vertex == key)
            return 1;

        p = p->next;
    }

    return 0;
}
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
