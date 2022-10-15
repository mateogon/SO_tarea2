
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>
#include <pthread.h> 
#include <bits/stdc++.h>
using namespace std;
 
// To add an edge
void addEdge(vector <pair<int, int> > adj[], int u,
                                     int v, int wt)
{
    adj[u].push_back(make_pair(v, wt));
}
 
// Print adjacency list representation of graph
void printGraph(vector<pair<int,int> > adj[], int V)
{
    int v, w;
    for (int u = 0; u < V; u++)
    {
        cout << "Node " << u << " makes an edge with \n";
        for (auto it = adj[u].begin(); it!=adj[u].end(); it++)
        {
            v = it->first;
            w = it->second;
            cout << "\tNode " << v << " with edge weight ="
                 << w << "\n";
        }
        cout << "\n";
    }
}
//retorna vecinos de un nodo
vector<pair<int,int> > get_neighbors(vector<pair<int,int> > adj[], int node){
   return adj[node];
}
//retorna int random entre min y max
int rangeRandomAlg2 (int min, int max){
    int n = max - min + 1;
    int remainder = RAND_MAX % n;
    int x;
    do{
        x = rand();
    }while (x >= RAND_MAX - remainder);
    return min + x % n;
}

void print_route(pair<int,vector<int>> route){
   cout << "Costo ruta:" << route.first <<"\n";
   for (auto it = route.second.begin(); it!=route.second.end(); it++)
   {
      printf("%d ",*it);
   }
   cout << "\n";
}

//recorre el grafo de manera aleatoria
//pair<int,vector<int>> 
void *traverse_graph( void * adj){

   int cost = 0;
   int node = 0;
   int end_node = 10;
   vector<pair<int,int> >* graph = (vector<pair<int,int> > *)adj;
   vector<pair<int,int> > neighbors; // vecinos
   pair<int,vector<int>> route; // costo, lista nodos ruta
   route.second.push_back(node); //nodo inicial 0
   int randNum,  w;

   while (node != 10){

      neighbors = graph[node];//get_neighbors(*graph,node);

      randNum = rangeRandomAlg2(0,neighbors.size()-1);//elige vecino aleatorio
      
      node = neighbors[randNum].first; //se mueve al nuevo nodo

      route.second.push_back(node);//agrega el nuevo nodo a la ruta

      cost += neighbors[randNum].second; //suma el costo al total
   }

   route.first = cost;
   print_route(route);
   pthread_exit(NULL);
   return NULL;
   //return route;
}



void print_neighbors(vector<pair<int,int> > adj[], int V, int node){
   int v, w;
   cout << "Node " << node << " makes an edge with \n";
   for (auto it = adj[node].begin(); it!=adj[node].end(); it++)
   {
      v = it->first;
      w = it->second;
      cout << "\tNode " << v << " with edge weight = " << w << "\n";
   }
   cout << "\n";
}
// Driver code
int main()
{
    srand(time(NULL)); //seed time
    int V = 11;
    vector<pair<int, int> > adj[V];
    addEdge(adj, 0, 1, 10);
    addEdge(adj, 0, 2, 15);
    addEdge(adj, 0, 3, 20);

    addEdge(adj, 1, 4, 30);
    addEdge(adj, 1, 5, 40);

    addEdge(adj, 2, 4, 37);
    addEdge(adj, 2, 5, 25);
    addEdge(adj, 2, 6, 30);
    addEdge(adj, 2, 3, 7);

    addEdge(adj, 3, 2, 7);
    addEdge(adj, 3, 4, 20);
    addEdge(adj, 3, 5, 30);

    addEdge(adj, 4, 5, 9);
    addEdge(adj, 4, 8, 30);
    addEdge(adj, 4, 9, 22);

    addEdge(adj, 5, 4, 9);
    addEdge(adj, 5, 6, 11);
    addEdge(adj, 5, 7, 25);
    addEdge(adj, 5, 8, 38);
    addEdge(adj, 5, 9, 29);

    addEdge(adj, 6, 5, 11);
    addEdge(adj, 6, 7, 29);
    addEdge(adj, 6, 8, 25);
    addEdge(adj, 6, 9, 38);

    addEdge(adj, 7, 8, 6);
    addEdge(adj, 7, 10, 7);

    addEdge(adj, 8, 7, 6);
    addEdge(adj, 8, 10, 10);

    addEdge(adj, 9, 10, 1);
    pthread_t thread1,thread2,thread3,thread4,thread5;

    pthread_create(&thread1, NULL, traverse_graph, (void *)adj);
    pthread_create(&thread2, NULL, traverse_graph, (void *)adj);
    pthread_create(&thread3, NULL, traverse_graph, (void *)adj);
    pthread_create(&thread4, NULL, traverse_graph, (void *)adj);
    pthread_create(&thread5, NULL, traverse_graph, (void *)adj);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    return 0;
}