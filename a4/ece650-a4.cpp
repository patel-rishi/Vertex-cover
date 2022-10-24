#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <list>

#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

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
    result.clear();

    return true;
}

vector<int> edges_length(string edge_list){
    std::vector<int> vect;

    std::stringstream ss(edge_list);

    for (int i; ss >> i;) {
        vect.push_back(i);    
        if (ss.peek() == ',')
            ss.ignore();
    }

    return vect;
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
                if (x>0){
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

                // ----------------------------SAT--------------------------------

                int k = 1;
                if(vertices==1){
                    cout << "" << endl;
                }else{
                    while(k<vertices){
                    
                        std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

                        //Creating n*k atomic literals
                        std::vector <std::vector<Minisat::Lit>> table(vertices);
                        for (int i = 0; i < vertices; ++i) 
                        {
                            for (int j = 0; j < k; ++j) 
                            {
                                Minisat::Lit literal = Minisat::mkLit(solver->newVar());
                                table[i].push_back(literal);
                            }
                        }

                        // Creating 1st set of clauses
                        for (int i = 0; i < k; i++) {
                            Minisat::vec<Minisat::Lit> vals;
                            for (int j=0; j<vertices; j++){
                                vals.push(table[j][i]);
                            }
                            solver->addClause(vals);
                        }

                        // // Creating 2nd set of clauses
                        for (int m = 0; m < vertices; m++) {
                            for(int p = 0; p < k-1; p++){
                                for(int q = p+1; q < k; q++){
                                    solver->addClause(~table[m][p],~table[m][q]);
                                }
                            }
                        }

                        // Creating 3rd set of clauses
                        for (int m = 0; m < k; m++) {
                            for(int p = 0; p < vertices-1; p++){
                                for(int q = p+1; q < vertices; q++){
                                    solver->addClause(~table[p][m],~table[q][m]);
                                }
                            }
                        }

                        // Creating 4th set of clauses
                        vector<int> vect = edges_length(edges);
                        for(int i = 0; i < int(vect.size()); i+=2){
                            Minisat::vec<Minisat::Lit> vals;
                            for(int j = 0; j < k; j++){
                                vals.push(table[vect.at(i)-1][j]);
                                vals.push(table[vect.at(i+1)-1][j]);
                            }
                            solver->addClause(vals);
                        }
                        
                        bool sat = solver->solve();

                        if(sat && int(vect.size())!=0){
                            vector<int> output_list;
                            for (int i = 0; i < vertices; i++) {
                                for (int j=0; j<k; j++){
                                    if(!Minisat::toInt(solver->modelValue(table[i][j]))){
                                        output_list.push_back(i);
                                    }
                                }
                            }
                            sort(output_list.begin(), output_list.end());
                            for (auto element : output_list)
                                cout << element+1 << " ";
                            cout << endl;

                            fflush(stdout);
                            output_list.clear();

                            break;
                        }else if(int(vect.size())==0){
                            cout << "" << endl;
                            fflush(stdout);
                            break;
                        }
                        k++;
                        vect.clear();
                        solver.reset (new Minisat::Solver());
                    } 
                }
                
				
                // -------------------------------SAT-------------------------------
                
            }else{
                cout << "Error: Invalid command!" << endl;
            }
        }
    }
    return 0;
}
