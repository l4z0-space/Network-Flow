#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <string>
using namespace std;

/* 
    Network Flow
    Edmond's Karp Algorithm

    > Uses BFS for finding the shortest path in terms of hops
    between start and finish node.
    > Doesn't get trapped to send flow via long paths.

    INPUT FORMAT
        | number of edges - n
        | start, sink - s, t
        | n pair of nodes - a, b

*/

const int MAX_V = 100;
const long long int INF = 1e5;
int capacity[MAX_V][MAX_V], max_flow, curr_flow, source, sink;
vector<int> parent;
map<int, vector<int> > Graph;
int edges; 

vector<vector<int>> AugmentingPaths;
vector<int> CurrAugPath;
vector<int> Flows;

void getInput(){
    cout << "\nGetting input...\n";
    cin >> edges;
    cin >> source >> sink;

    while(edges--){
        int a, b, c;
        cin >> a >> b >> c;
        capacity[a][b] = c;
        Graph[a].push_back(b);
    }
    cout << "Success.\n\n";
}


void showOutput(){
    cout << "Augmenting paths...\n";
    int flowIt = 0;
    for ( vector<int> path: AugmentingPaths ){
        int flow = Flows[flowIt++];
        if(path.size() > 1){
            reverse(path.begin(), path.end());
            cout << "  Path: [ ";
            int it = path.size();
            for ( int node : path) {
                it--;
                cout << node;
                if (it) cout << " -> ";
            }
            cout << " ] flow: " << flow << "\n" ;
        }
    }
    cout << "Maximum flow: " << max_flow << endl;
}


void augment(int curr_node, int minEdge){
    CurrAugPath.push_back(curr_node);
    if(curr_node == source){
        curr_flow = minEdge;
        return;
    }
    else if ( parent[curr_node] != -1 ){
        augment(parent[curr_node], min(minEdge, capacity[parent[curr_node]][curr_node]));
        // Change edge weights
        capacity[parent[curr_node]][curr_node] -= curr_flow;
        capacity[curr_node][parent[curr_node]] += curr_flow;
    }
}


int main(){

    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    getInput();
    max_flow = 0;
    while(true){
        curr_flow = 0;
        // Run BFS
        vector<int> dist(MAX_V, INF);
        dist[source] = 0;
        queue<int> q; q.push(source);
        parent.assign(MAX_V, -1);

        while(!q.empty()){
            int current_node = q.front();
            q.pop();
            if ( current_node == sink ) 
                break;
            for ( int nextNode : Graph[current_node] ){
                // Very important to check capacity is > 0 in the current edge 
                if (dist[nextNode] == INF && capacity[current_node][nextNode] > 0 ){
                    dist[nextNode] = dist[current_node] + 1;
                    q.push(nextNode);
                    parent[nextNode] = current_node;
                }
            }
        }
        // Augment path
        CurrAugPath.clear();
        augment(sink, INF);

        // Prepare for output
        AugmentingPaths.push_back(CurrAugPath);
        Flows.push_back(curr_flow);

        if (curr_flow == 0) break;

        // Increase maximum flow
        max_flow += curr_flow;
    }
    showOutput();
}


