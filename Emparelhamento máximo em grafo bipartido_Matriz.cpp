#include <bits/stdc++.h>

using namespace std;

void leituraGrafo(vector<vector<int>> &, int);

void escritaGrafo(vector<vector<int>> &);

bool grafobipartido(vector<vector<int>> &, int);

bool bfs(vector<vector<int>> &, int, int, vector<int> &);

int fordFulkerson(vector<vector<int>> &, int, int);

int emparelhamentoMaximo(vector<vector<int>> &, int);


int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> Grafo(n, vector<int>(n, 0));
    leituraGrafo(Grafo, m);
    
    if (grafobipartido(Grafo, n)) {
        cout << endl;
        escritaGrafo(Grafo);
        cout << "O grafo é bipartido." << endl;
        int EmparelhamentoMaximo = emparelhamentoMaximo(Grafo, n);
        cout << endl;
        cout << "O emparelhamento máximo é: " << EmparelhamentoMaximo << endl;
    } else {
        cout << "O grafo não é bipartido." << endl;
        return 0;
    }
    return 0;
}

void leituraGrafo(vector<vector<int>> &G, int m) {
    int a, b;
    while (m--) {
        cin >> a >> b;
        G[a][b] = 1;
        G[b][a] = 1;
    }
}

void escritaGrafo(vector<vector<int>> &G) {
    int n = G.size();
    cout << "Matriz de Adjacência:" << endl;
    cout << "   ";
    for (int u = 0; u < n; u++) {
        cout << u << " ";
    }
    cout << endl;
    for (int u = 0; u < n; u++) {
        cout << u << ": ";
        for (int v = 0; v < n; v++) {
            cout << G[u][v] << " ";
        }
        cout << endl;
    }
}

bool grafobipartido(vector<vector<int>> &Grafo, int n) {
    vector<int> color(n, -1);
    // Aqui faz um bfs
    for (int i = 0; i < n; ++i) {
        if (color[i] == -1) {
            queue<int> q;
            q.push(i);
            color[i] = 0;
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                
                for (int v = 0; v < n; ++v) {
                    if (Grafo[u][v]) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            q.push(v);
                        } else if (color[v] == color[u]) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool bfs(vector<vector<int>> &residualGraph, int s, int t, vector<int> &parent) {
    int n = residualGraph.size();
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}

int fordFulkerson(vector<vector<int>> &graph, int s, int t) {
    int u, v;
    int n = graph.size();
    vector<vector<int>> residualGraph = graph;
    vector<int> parent(n);
    int max_flow = 0;

    while (bfs(residualGraph, s, t, parent)) {
        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, residualGraph[u][v]);
        }

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            residualGraph[u][v] -= path_flow;
            residualGraph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }

    return max_flow;
}

int emparelhamentoMaximo(vector<vector<int>> &Grafo, int n) {
    int source = n;
    int sink = n + 1;
    
    vector<vector<int>> flowGraph(n + 2, vector<int>(n + 2, 0));
    
    int partition1 = n / 2;
    int partition2 = n - partition1;
    
    for (int u = 0; u < partition1; ++u) {
        flowGraph[source][u] = 1;
        for (int v = partition1; v < n; ++v) {
            flowGraph[u][v] = Grafo[u][v];
        }
    }
    
    for (int v = partition1; v < n; ++v) {
        flowGraph[v][sink] = 1;
    }
    
    return fordFulkerson(flowGraph, source, sink);
}