#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"
#define INFINITIVE_VALUE 9999999
typedef struct
{
    JRB vertices;
    JRB edges;
} Graph;
Graph createGraph();
void dropGraph(Graph graph);
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
void readDataMap(Graph graph);
double shortesPath(Graph graph, int s, int t, int *path, int *length);
void Case1(Graph graph);
void Case2(Graph graph);
void Case3(Graph graph);
void Case4(Graph graph);
void Case5(Graph graph);
int menu()
{
    int choose;
    printf("\n\n");
    printf("------------------ Thanh Tri -----------------\n");
    printf("----------------------------------------------\n");
    printf("- %-50s -\n", "1: Read Graph File");
    printf("- %-50s -\n", "2: Show adjacet loccation");
    printf("- %-50s -\n", "3: Show adjacet loccation by walk...");
    printf("- %-50s -\n", "4: Sortest path between 2 location");
    printf("- %-50s -\n", "5: Show all buses through a location");
    printf("- %-50s -\n", "6: Exit");
    printf("\nYour choose: ");
    fflush(stdin);
    scanf("%d", &choose);
    return choose;
}
int main()
{

    Graph g = createGraph();
    while (1)
    {
        int check = menu();
        switch (check)
        {
        case 1:
            readDataMap(g);
            break;
        case 2:
            // Case2(g);
            break;
        case 3:
            // Case3(g);
            break;
        case 4:
            // Case4(g);
            break;
        case 5:
            // Case5(g);
            break;
        case 6:
            return 0;
        default:
        {
            printf("-------------------\n");
            printf("Input isn't correct\n");
            printf("-------------------\n");

            break;
        }
        break;
        }
    }
}
Graph createGraph()
{
    Graph g;
    g.edges = make_jrb();
    g.vertices = make_jrb();
    return g;
}
void dropGraph(Graph graph)
{
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}
void addVertex(Graph graph, int id, char *name)
{
    if (jrb_find_int(graph.vertices, id) == NULL)
    {
        jrb_insert_int(graph.vertices, id, new_jval_s(name));
    }
}
char *getVertex(Graph graph, int id)
{
    JRB node;
    node = jrb_find_int(graph.vertices, id);
    if (node == NULL)
    {
        return NULL;
    }
    else
    {
        return jval_s(node->val);
    }
}
int getIdVertex(Graph graph, char *name)
{
    JRB node = make_jrb();
    jrb_traverse(node, graph.vertices)
    {
        if (strcmp(name, jval_s(node->val)) == 0)
        {
            return jval_i(node->key);
        }
    }
    return -1;
}
void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB tree = make_jrb();
    if (jrb_find_int(graph.edges, v1) == NULL)
    {
        jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }
    else
    {
        JRB node = jrb_find_int(graph.edges, v1);
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v2) == NULL)
            jrb_insert_int(tree, v2, new_jval_d(weight));
    }
}
double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node1 = jrb_find_int(graph.edges, v1);
    if (node1 == NULL)
    {
        return INFINITIVE_VALUE;
    }
    else
    {
        JRB tree;
        tree = (JRB)jval_v(node1->val);
        JRB node2 = jrb_find_int(tree, v2);
        if (node2 == NULL)
        {
            return INFINITIVE_VALUE;
        }
        else
        {
            return jval_d(node2->val);
        }
    }
}
int indegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v))
        {
            output[total] = jval_i(node->key);
            total++;
        }
    }
    return total;
}

int outdegree(Graph graph, int v, int *output)
{
    int total = 0;
    JRB treeNode = jrb_find_int(graph.edges, v);
    JRB tree = (JRB)jval_v(treeNode->val);
    if (tree == NULL)
        return 0;

    JRB node;
    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }

    return total;
}
int trim(char *s)
{
    int d = 0, i = 0, prev = -1;
    while (s[i])
    {
        if (s[i] == ' ' && (prev < 0 || s[prev] == ' '))
        {
            d++;
        }
        else
        {
            s[i - d] = s[i];
            prev = i;
        }
        i++;
    }
    i -= d;
    if (i && s[i - 1] == ' ')
        s[i - 1] = 0, d++;
    else
        s[i] = 0;

    return d;
}
void readDataMap(Graph graph)
{
    int n_vertex, n_edge;
    char *temp, *token, *weight, *vertex1, *vertex2;
    temp = (char *)malloc(100 * sizeof(char));
    token = (char *)malloc(100 * sizeof(char));
    vertex1 = (char *)malloc(100 * sizeof(char));
    vertex2 = (char *)malloc(100 * sizeof(char));
    weight = (char *)malloc(100 * sizeof(char));
    FILE *f;
    f = fopen("dothi.txt", "r");
    if (f == NULL)
    {
        printf(" Error\n");
        exit(1);
    }
    fgets(temp, 100, f);
    temp[strlen(temp) - 1] = '\0';
    token = strtok(temp, " ");
    n_vertex = atoi(token);
    token = strtok(NULL, " ");
    n_edge = atoi(token);
    printf("%d - %d\n", n_vertex, n_edge);
    while (fgets(temp, 20, f) != NULL)
    {
        temp[strlen(temp) - 1] = '\0';
        token = strtok(temp, " ");
        trim(token);
        strcpy(vertex1, token);
        addVertex(graph, atoi(vertex1), vertex1);
        printf("dinh 1: %s\n",getVertex(graph,atoi(vertex1)));
        token = strtok(NULL, " ");
        trim(token);
        strcpy(vertex2, token);
        addVertex(graph, atoi(vertex2), token);
        printf("dinh 2: %s\n",getVertex(graph,atoi(vertex2)));
        token = strtok(NULL, " ");
        trim(token);
        printf("weight: %d\n",atoi(token));
        addEdge(graph, atoi(vertex1),atoi(vertex2), atoi(token));
        addEdge(graph, atoi(vertex2),atoi(vertex1), atoi(token));
        printf(("hau\n"));
    }
    JRB temp_jrb = make_jrb();
    JRB temp_jrb1 = make_jrb();
    printf("   |");
    jrb_traverse(temp_jrb, graph.vertices)
    {
        printf("%d   ", jval_i(temp_jrb->key));
    }
    printf("\n");
    jrb_traverse(temp_jrb1, graph.vertices)
    {
        printf("%-3d|", jval_s(temp_jrb1->key));
        jrb_traverse(temp_jrb, graph.vertices)
        {
            if (getEdgeValue(graph, jval_i(temp_jrb->key), jval_i(temp_jrb1->key)) < INFINITIVE_VALUE)
            {
                printf("1   ");
            }
            else
            {
                printf("0   ");
            }
        }
        printf("\n\n");
    }
    jrb_traverse(temp_jrb,graph.vertices)
    {
        printf("%s/",getVertex(graph,jval_i(temp_jrb->key)));
    }
    fclose(f);
}
