#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <csignal>

using namespace std;

string getString(char x){
    string s(1, x);
    return s;  
}

vector<string> street_names_pool;
int generate_names(){
    for (char i = 'A'; i < 'Z'; i++){
        for (char j = 'A'; j < 'Z'; j++){
            for (char k = 'A'; k < 'Z'; k++){
                string temp = getString(i);
                temp = temp.append(getString(j));
                temp = temp.append(getString(k));
                street_names_pool.push_back(temp);
            }
        }        
    }
    return 0;
}

void signalHandler( int signum ) {
   exit(signum);  
}

bool max_value_check(int temp, int min){
    if (temp>=min){
        return true;
    }else{
        cerr << "Error: Invalid input." << endl;
        return false;
    }
}

int get_random_in_range(int min, int max){
    unsigned int randval;
    FILE *f;

    f = fopen("/dev/random", "r");
    fread(&randval, sizeof(randval), 1, f);
    fclose(f);

    int range = max - min + 1;
    return randval%range + min;
}

int get_coordinate(int max){
    return rand()%(2*max) - max;
}

bool check_slope(int x1,int x2,int x3,int x4,int y1,int y2,int y3,int y4){
    bool condition = (x1-x2)!=0 && (x3-x4)!=0;
    if (condition){
        float slope1 = (y1-y2)/(x1-x2);
        float slope2 = (y3-y4)/(x3-x4);
        if(slope1==slope2){
            return false;
        }else{
            return true;
        }
    }else{
        return false;
    }
}

bool check_intersection(vector<vector<int>> list, vector<int> pair){
    if (list.size()<2)
        return false;
    int i = 0;
    while(i<list.size()){
        int x1 = pair[0];
        int y1 = pair[1];
        int x2 = list[list.size()-1][0];
        int y2 = list[list.size()-1][1];

        int x3 = list[i][0];
        int y3 = list[i][1];
        int x4 = list[i+1][0];
        int y4 = list[i+1][1];

        int xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
        int ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
        int den = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));
        if (den != 0){
            float xcoor =  xnum / den;
            float ycoor = ynum / den;
            bool x_evaluation = xcoor>=min(x1,x2) && xcoor<=max(x1,x2) && xcoor>=min(x3,x4) && xcoor<=max(x3,x4);
            bool y_evaluation = ycoor>=min(y1,y2) && ycoor<=max(y1,y2) && ycoor>=min(y3,y4) && ycoor<=max(y3,y4);
            if (x_evaluation && y_evaluation){
                bool overlap_case = check_slope(x1,x2,x3,x4,y1,y2,y3,y4);
                if (!overlap_case){
                    return true;
                }
                return false;
            }else{
                return false;
            }
        }else{
            return false;
        }
        i++;
    }
    return false;
}

bool check_street_coord_validity(vector<vector<int>> list, vector<int> pair){
    for(auto i = 0; i<list.size(); i++){
        if(list[i]==pair) return false;
    }
    if(!check_intersection(list, pair)){
        return true;
    }else{
        return false;
    }
}

int main(int argc, char **argv){
    street_names_pool.clear();
    generate_names();

    const int min_street_count=2, min_line_count=1, min_wait_time=5, min_coord_range=1;
    int max_street_count=10, max_line_count=5, max_wait_time=5, max_coord_range=20;

    vector<string> street_names;
    
    if(argc>1){
        string cmd_inputs[8];
        int temp_value;
        for (int i = 1; i < argc; i++) {
            cmd_inputs[i-1] = cmd_inputs[i-1] + argv[i];
        }
        int i = 0;
        while (i<8){
            if (cmd_inputs[i]=="-s"){
                temp_value = stoi(cmd_inputs[i+1]);
                bool ans = max_value_check(temp_value, min_street_count);
                if (ans){
                    max_street_count = temp_value;
                }else{
                    break;
                }
            }else if (cmd_inputs[i]=="-n"){
                temp_value = stoi(cmd_inputs[i+1]);
                bool ans = max_value_check(temp_value, min_line_count);
                if (ans){
                    max_line_count = temp_value;
                }else{
                    break;
                }
            }else if (cmd_inputs[i]=="-l"){
                temp_value = stoi(cmd_inputs[i+1]);
                bool ans = max_value_check(temp_value, min_wait_time);
                if (ans){
                    max_wait_time = temp_value;
                }else{
                    break;
                }
            }else if (cmd_inputs[i]=="-c"){
                temp_value = stoi(cmd_inputs[i+1]);
                bool ans = max_value_check(temp_value, min_coord_range);
                if (ans){
                    max_coord_range = temp_value;
                }else{
                    break;
                }
            }else{
                cerr << "Error: Invalid command" << endl;
                break;
            }
            i=i+2;
            if(i>=8){
                break;
            }else if (cmd_inputs[i]==""){
                break;
            }
        }
    }

    int const street_count = get_random_in_range(min_street_count, max_street_count);
    int const wait_time = get_random_in_range(min_wait_time, max_wait_time);
    int line_count = get_random_in_range(min_line_count, max_line_count);

    vector<vector<int>> temp_graph_coord_list;

    signal(SIGTERM, signalHandler);

    while(1){
        for (int sc = 0; sc < street_names.size(); sc++){
            cout << "rm \"" << street_names[sc] << "\"" << endl;
        }
        fflush(stdout);
        street_names.clear();

        for (int sc = 0; sc < street_count; sc++){
            bool street_flag = false;
            vector<vector<int>> temp_coord_list;
            string input_a1 = "add \"Street "+street_names_pool[sc]+"\"";
            for (int lc = 0; lc <= line_count; lc++){
                bool coord_flag = false;
                for (int iter_count = 0; iter_count < 25; iter_count++){
                    int x_coord = get_coordinate(max_coord_range);
                    int y_coord = get_coordinate(max_coord_range);
                    vector<int> temp_coord = {x_coord,y_coord};
                    bool ans = check_street_coord_validity(temp_coord_list, temp_coord);
                    if (ans){
                        coord_flag = true;
                        street_flag = true;
                        temp_coord_list.push_back(temp_coord);
                        input_a1 = input_a1 + " (" + to_string(x_coord) +","+ to_string(y_coord) +")";
                        break;
                    }                
                }
                if(!coord_flag){
                    cerr << "Error: Failed to generate valid input for 25 simultaneous attempts" << endl;
                }
            }
            if(!street_flag){
                cerr << "Error: Failed to generate a valid street. 0 coordinates generated after 25 simultaneous attempts per coordinate." << endl;
            }else{
                street_names.push_back("Street "+street_names_pool[sc]);
                cout << input_a1 << endl;
                fflush(stdout);
            }
        }
        cout << "gg" << endl;
        fflush(stdout);

        sleep(wait_time);
    }
    return 0;
}

