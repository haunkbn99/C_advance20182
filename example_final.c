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
            Case2(g);
            break;
        case 3:
            Case3(g);
            break;
        case 4:
            Case4(g);
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
double shortesPath(Graph graph, int s, int t, int *path, int *length)
{
    double distance[1000], min;
    int previous[1000], u, visited[1000], output[100], number;
    for (int i = 0; i < 1000; i++)
    {
        distance[i] = INFINITIVE_VALUE;
        visited[i] = 0;
        previous[i] = 0;
    }
    distance[s] = 0;
    previous[s] = s;
    visited[s] = 1;
    Dllist ptr, queue, node;
    queue = new_dllist();
    dll_append(queue, new_jval_i(s));
    while (!dll_empty(queue))
    {
        node = dll_first(queue);
        int u = jval_i(node->val);
        dll_delete_node(node);
        number = outdegree(graph, u, output);
        for (int i = 0; i < number; i++)
        {
            if (visited[output[i]] == 0)
            {
                visited[output[i]] = 1;
                dll_append(queue, new_jval_i(output[i]));
            }
            if ((getEdgeValue(graph, u, output[i]) + distance[u]) < distance[output[i]])
            {
                distance[output[i]] = getEdgeValue(graph, u, output[i]) + distance[u];
                previous[output[i]] = u;
            }
        }
    }
    path[0] = t;
    *length = 1;
    int cur = t;
    while (previous[cur] != s)
    {
        path[*length] = previous[cur];
        *length = *length + 1;
        cur = previous[cur];
    }

    path[*length] = s;
    return distance[t];
}
void readDataMap(Graph graph)
{
    int n_vertex, n_edge;
    int castle1, castle2, time;
    FILE *f;
    f = fopen("dothi.txt", "r");
    if (f == NULL)
    {
        printf(" Error\n");
        exit(1);
    }
    fscanf(f, "%d\t%d\n", &n_vertex, &n_edge);
    while (!feof(f))
    {
        fscanf(f, "%d\t%d\t%d\n", &castle1, &castle2, &time);
        addVertex(graph, castle1, "");
        addVertex(graph, castle2, "");
        addEdge(graph, castle1, castle2, time);
        addEdge(graph, castle2, castle1, time);
    }

    JRB temp_jrb = make_jrb();
    JRB temp_jrb1 = make_jrb();
    printf("    ");
    jrb_traverse(temp_jrb, graph.vertices)
    {
        printf("%d   ", jval_i(temp_jrb->key));
    }
    printf("\n\n");
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
    fclose(f);
}
void Case2(Graph graph)
{
    JRB temp = make_jrb();
    int *output = (int *)malloc(100 * sizeof(int));
    printf("Danh sach lien cua cua tat ca cac Thanh\n");
    jrb_traverse(temp, graph.vertices)
    {
        printf("\n-   Castle %d: ", jval_i(temp->key));
        int total = outdegree(graph, jval_i(temp->key), output);
        for (int i = 0; i < total; i++)
        {
            printf("%d ", output[i]);
        }
    }
}
void Case3(Graph graph)
{
    int check = 0, count = 0, max = 0, castle[100];
    JRB temp = make_jrb();
    int *output = (int *)malloc(100 * sizeof(int));
    printf("Danh sach cac thanh co the den no truc tiep tu mot thanh khac bang cach di bo\n");
    jrb_traverse(temp, graph.vertices)
    {
        check = 0;
        int total = outdegree(graph, jval_i(temp->key), output);
        for (int i = 0; i < total; i++)
        {
            if (getEdgeValue(graph, jval_i(temp->key), output[i]) < 50)
            {
                check = 1;
                break;
            }
        }
        if (check == 0)
        {
            printf("- Castle: %d\n", jval_i(temp->key));
        }
    }
    jrb_traverse(temp, graph.vertices)
    {
        int total = outdegree(graph, jval_i(temp->key), output);
        // printf("max:%d/total:%d/key:%d\n",max,total,jval_i(temp->key));
        if (total == max)
        {
            castle[count++] = jval_i(temp->key);
        }
        else if (total > max)
        {
            max = total;
            count = 0;
            castle[count++] = jval_i(temp->key);
        }
    }
    printf("\nCac thanh co nhieu duong noi truc tiep nhat:\n");
    for (int i = 0; i < count; i++)
    {
        printf("- Castle: %d\n", castle[i]);
    }
}
void Case4(Graph graph)
{
    int start, stop, output[100], lenght;
    printf("Nhap thanh tri xuat phat: ");
    fflush(stdin);
    scanf("%d", &start);
    printf("Nhap thanh tri dich: ");
    fflush(stdin);
    scanf("%d", &stop);
    double distant = shortesPath(graph, start, stop, output, &lenght);
    if (lenght < 0)
    {
        printf("ROUTE NOT FOUND\n");
    }
    else
    {

        printf("Duong di ngan nhat tu Castle: %d -> Castle: %d\n", start, stop);
        printf("        - Do dai: %.0lf\n", distant);
        printf("        ");
        for (int i = 0; i <= lenght; i++)
        {
            printf("- ");
            printf("%d ", output[lenght - i]);
        }
    }
}
void Case5(Graph graph)
{
    
}