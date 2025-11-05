#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 100

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

void DFS_matrix_recursive(int v, int** G, int* NUM, int size_G) {
    NUM[v] = 1;
    printf("%d ", v + 1);
    for (int i = 0; i < size_G; i++) {
        if (G[v][i] == 1 && NUM[i] == 0) {
            DFS_matrix_recursive(i, G, NUM, size_G);
        }
    }
}

void DFS_matrix_non_recursive(int start, int** G, int* NUM, int size_G) {
    int stack[MAX_SIZE];
    int top = -1;

    stack[++top] = start;
    NUM[start] = 1;
    printf("%d ", start + 1);

    while (top >= 0) {
        int v = stack[top];

        int found = 0;
        for (int i = 0; i < size_G; i++) {
            if (G[v][i] == 1 && NUM[i] == 0) {
                NUM[i] = 1;
                stack[++top] = i;
                printf("%d ", i + 1);
                found = 1;
                break;
            }
        }
        if (!found) {
            top--;
        }
    }
}

void DFS_list(int v, Node** adjList, int* NUM, int n) {
    NUM[v] = 1;
    printf("%d ", v + 1);
    Node* current = adjList[v];
    while (current != NULL) {
        int neighbor = current->vertex;
        if (NUM[neighbor] == 0) {
            DFS_list(neighbor, adjList, NUM, n);
        }
        current = current->next;
    }
}

void addEdgeSorted(Node** adjList, int src, int dest) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = dest;
    newNode->next = NULL;

    if (adjList[src] == NULL || adjList[src]->vertex > dest) {
        newNode->next = adjList[src];
        adjList[src] = newNode;
    }
    else {
        Node* current = adjList[src];
        while (current->next != NULL && current->next->vertex < dest) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(NULL));

    int n;
    printf("Введите количество вершин: ");
    scanf("%d", &n);

    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            }
            else {
                matrix[i][j] = matrix[j][i] = rand() % 2;
            }
        }
    }

    printf("Матрица смежности:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    Node** adjList = (Node**)malloc(n * sizeof(Node*));
    for (int i = 0; i < n; i++) {
        adjList[i] = NULL;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                addEdgeSorted(adjList, i, j);
            }
        }
    }

    printf("\nСписки смежности (в порядке возрастания):\n");
    for (int i = 0; i < n; i++) {
        printf("Вершина %d: ", i + 1);
        Node* current = adjList[i];
        while (current != NULL) {
            printf("%d ", current->vertex + 1);
            current = current->next;
        }
        printf("\n");
    }

    int* NUM1 = (int*)calloc(n, sizeof(int));
    printf("\nРекурсивный обход через матрицу: ");
    for (int i = 0; i < n; i++) {
        if (NUM1[i] == 0) {
            DFS_matrix_recursive(i, matrix, NUM1, n);
        }
    }
    printf("\n");

    int* NUM2 = (int*)calloc(n, sizeof(int));
    printf("Нерекурсивный обход через матрицу: ");
    for (int i = 0; i < n; i++) {
        if (NUM2[i] == 0) {
            DFS_matrix_non_recursive(i, matrix, NUM2, n);
        }
    }
    printf("\n");

    int* NUM3 = (int*)calloc(n, sizeof(int));
    printf("Обход через списки: ");
    for (int i = 0; i < n; i++) {
        if (NUM3[i] == 0) {
            DFS_list(i, adjList, NUM3, n);
        }
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
        Node* current = adjList[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(matrix);
    free(adjList);
    free(NUM1);
    free(NUM2);
    free(NUM3);

    return 0;
}