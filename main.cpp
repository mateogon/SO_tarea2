
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
#include <mutex>
#include <semaphore>
using namespace std;

mutex m;
multimap<int, vector<int>> rutas;
//counting_semaphore<3> semaforo;

// To add an edge
void addEdge(vector <pair<int, int> > adj[], int u, int v, int wt)
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

void print_route(int cost, vector<int> route){
   cout << "Costo ruta:" << cost <<"\n";
   for (auto it = route.begin(); it!=route.end(); it++)
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
   //print_route(route);
   m.lock();
   rutas.insert({route.first,route.second});
   m.unlock();
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


int main()
{
    srand(time(NULL)); //seed time
    int V = 11;
    int thread_amount = 10;
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

    vector<pthread_t> threads(thread_amount);

   for(int i = 0; i<thread_amount; i++) { //crear threads
    pthread_t new_thread;
    pthread_create(&new_thread, NULL, traverse_graph, (void *)adj);
    threads.push_back(new_thread);
   }
    for(auto t : threads) {
      pthread_join((pthread_t)t, NULL);
    }
   
    
   for (auto it = rutas.begin(); it!=rutas.end(); it++)
   {
      print_route(it->first,it->second);
   }
    return 0;
}