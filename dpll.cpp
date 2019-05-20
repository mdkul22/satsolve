#include "dpll.h"

using namespace std;

DPLL::DPLL(vector<vector<short> > equ){
    equation = equ;
    init_();
}

void DPLL::init_(){
    cout<<"DPLL initializing" <<endl;
    no_clauses = equation.size();
    no_variables = equation[1].size();
    cout<<no_clauses<<" "<<no_variables<<endl;
    for(int i=0; i<no_variables; i++){
        priority_list.push_back(make_pair(i, 0));
    }
}

void DPLL::assign_priority() {
    cout<<"beginning priority assignment!"<<endl;
    for(int i=0; i<equation.size(); i++) {
        for(int j=0; j<equation[j].size(); j++){
            priority_list[i].second += abs(equation[i][j]); 
           // cout<<"equation ij = "<<equation[i][j]<<endl; 
        }
    }
    sort(priority_list.begin(), priority_list.end(), sortbysec); 
    cout<<"size: "<<priority_list.size()<<endl;
    for(int i=0; i<priority_list.size(); i++){
       cout<<priority_list[i].first<<" :"<<priority_list[i].second<<endl;
    }
    cout<<"done"<<endl;
}

bool sortbysec(const pair<int, int> &a, const pair<int, int> &b) {
    return (a.second > b.second);
}
// Will do it the old school way by running all possible combinations 
// in an iterative manner to get a number of solutions. This would only
// work for a solution space of a maximum of 32 variables
void DPLL::calculate_iter(){
    long comb = 1;
    comb = comb<<no_variables;
    vector<vector<bool> > solution_space(comb, vector<bool>(no_variables));
    cout<<"combinations: "<<comb<<endl;
    for(long i=0; i<comb; i++){
        long mask=1;
        for(int j=0; j<no_variables; j++){
            if((i&mask)>>j==1){
                solution_space[i][j] = true;
            }
            else{
                solution_space[i][j] = false;
            }
            mask = mask << 1;
        }
    }
    //cout<<"begin calculations.."<<endl;
    // Begin calculations
    auto start = chrono::high_resolution_clock::now();
    vector<vector<bool> > solution_set;
    for(long i=0; i<comb; i++) {
        bool valid = check_solution(solution_space[i]);
        if(valid){
            solution_set.push_back(solution_space[i]);
        }
    }
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    dif = chrono::duration_cast<chrono::microseconds>(elapsed).count();
    cout<<"printing solutions!"<<endl;
    print_solutions(solution_set);
}

// Checks if a particular combination of variables is valid for the equation
// or not.
bool DPLL::check_solution(vector<bool> solution){
    bool is_zero;
    for(int i=0; i<no_clauses; i++){
        for(int j=0; j<no_variables; j++) {
            if(((equation[i][j]==1) && solution[j]) || ((equation[i][j]==-1) && !solution[j])){
                is_zero=false;
                break;
            }
            else if(equation[i][j]==0){
                is_zero=true;
            }
            else{
                is_zero=true;
            }
        }
        if(is_zero){
            break;
        }
    }
    return !is_zero;
}

void DPLL::print_solutions(vector<vector<bool> > solution_set){
    for(int i=0; i<solution_set.size(); i++) {
        cout<<"solution "<<i<<" :" ;
        for(int j=0; j<solution_set[j].size(); j++) {
            if(solution_set[i][j])
                cout<<1<<" ";            
            else
                cout<<0<<" ";
        }
        cout<<endl;
    }
    cout<<"Time elapsed is "<<dif<<" microseconds"<<endl;
}
