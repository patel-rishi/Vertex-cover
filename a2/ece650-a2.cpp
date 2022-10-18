// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <list>

using namespace std;

string removeCharacters(string S, char c){
    S.erase(remove(
                S.begin(), S.end(), c),
            S.end());
    return S;
}

bool check_edges(string x, int vertices){
    vector<string> result;
    stringstream s_stream(x);
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, ',');
        result.push_back(substr);
    }
    for(int i = 0; i<(int)result.size(); i++) {
        if(stoi(result.at(i))>vertices){
            cout << "Error: Invalid edges" << endl;
            return false;
        }
    }
    return true;
}

class Graph{
    int V;
	list<int> *adj;
public:
	Graph(int V);

	// function to add an edge to graph
	void addEdge(int v, int w);

	// prints BFS traversal from a given source s
	void BFS(int s, int des);

    void destroy_adj();

};

Graph::Graph(int V){
	this->V = V;
	adj = new list<int>[V];
}

void Graph::destroy_adj(){
    delete[] adj;
}

void Graph::addEdge(int v, int w){
	adj[v].push_back(w); // Add w to v’s list.
    adj[w].push_back(v);
}

void Graph::BFS(int s, int des){
	// Mark all the vertices as not visited
    int source = s;
	bool *visited = new (nothrow) bool[V];
    int *pred = new (nothrow) int[V];
    if (NULL==visited || NULL==pred){
        cout << "Error: Memory not allocated!" << endl;
        exit(1);
    }else{
        for(int i = 0; i < V; i++){
            if(i==s){
                visited[i] = true;
                pred[i] = 0;
            }else{
                visited[i] = false;
                pred[i] = -1;
            }
        }
        
        // Create a queue for BFS
        list<int> queue;
        queue.push_back(s);
        
        // 'i' will be used to get all adjacent
        // vertices of a vertex
        list<int>::iterator i;

        while(!queue.empty()){
            // Dequeue a vertex from queue and print it
            s = queue.front();
            queue.pop_front();

            //Break the statement when we are done traversing the required path
            if (s==des){
                break;
            }
            // Get all adjacent vertices of the dequeued
            // vertex s. If a adjacent has not been visited,
            // then mark it visited, store its predecessor and enqueue it
            for (i = adj[s].begin(); i != adj[s].end(); ++i){
                if (!visited[*i]){
                    visited[*i] = true;
                    pred[*i] = s;
                    queue.push_back(*i);
                }
            }
        }
        if (pred[des]!=-1 && pred[des]!=source){
            int* rev_path = new (nothrow) int[V];
            if (NULL==rev_path){
                cout << "Error: Memory not allocated" << endl;
            }else{
                // Initializing the rev_path array with default values
                for(int i = 0; i < V; i++){
                    rev_path[i] = 0;
                }

                // Storing values of predecessors in rev_path starting from destination
                int index = V-1;
                int temp_des = pred[des];
                while (true){
                    rev_path[index] = temp_des;
                    temp_des = pred[temp_des];
                    index--;
                    if (temp_des==source) break;
                }

                // Print out the traversal
                cout << source+1 << '-';
                for (int i=0; i<V; i++){
                    if (rev_path[i]==0) continue;
                    cout << rev_path[i]+1 << '-';
                }
                cout << des+1 << endl;
                delete[] rev_path;
            }
        }else if (pred[des]==source){
            // Traversal path does not contain any intermediate nodes
            cout << source+1 << '-' << des+1 << endl;
        }else{
            cout << "Error: No path exists!" << endl;
        }
        delete[] visited;
        delete[] pred;
    }
}

int main() {
    int vertices=0;
    string edges="";

    // read from stdin until EOF
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        if (line!="") {
            if(line[0]=='V'){
                int x = stoi(line.substr(2));
                if (x>1){
                    vertices = x;
                    edges = "";
                }else{
                    cout << "Error: Invalid value of number of vertices!" << endl;
                }
            }else if(line[0]=='E'){
                int start_index = line.find('{');
                string temp = line.substr(start_index+1);
                temp = temp.substr(0, temp.find('}'));
                if (temp.length()!=0){
                    temp = removeCharacters(temp, '<');
                    temp = removeCharacters(temp, '>');
                    if (check_edges(temp, vertices)){
                        edges = temp;
                    }
                }
                
            }else if(line[0]=='s'){
                if (edges==""){
                    cout << "Error: No edges exist for this graph!" << endl;
                }else{
                    string temp = line.substr(2);
                    int space_index = temp.find(' ');
                    int source = stoi(temp.substr(0,space_index));
                    int destination = stoi(temp.substr(space_index+1));
                    if (source==destination){
                        cout << "Error: Invalid command!" << endl;
                        continue;
                    }
                    if (vertices>0){
                        Graph g(vertices);
                        vector<string> result;
                        stringstream s_stream(edges);
                        while(s_stream.good()) {
                            string substr;
                            getline(s_stream, substr, ',');
                            result.push_back(substr);
                        }
                        int i = 0;
                        while(i<(int)result.size()) {
                            int x = stoi(result.at(i))-1;
                            int y = stoi(result.at(i+1))-1;
                            g.addEdge(x, y);
                            i = i + 2;
                        }
                        if (source<=vertices && destination<=vertices){
                            g.BFS(source-1, destination-1);
                        }else{
                            cout << "Error: Invalid vertices!" << endl;
                        }
                        g.destroy_adj();
                    }
                }
            }else{
                cout << "Error: Invalid command!" << endl;
            }
        }
    }
    return 0;
}
