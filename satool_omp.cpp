#include "satool.h"
#include<omp.h>

using namespace std;

SATool::SATool(vector<vector<short> > equ){
    equation = equ;
    init_();
}

void SATool::init_(){
    cout<<"SATool initializing" <<endl;
    no_clauses = equation.size();
    no_variables = equation[1].size();
    cout<<no_clauses<<" "<<no_variables<<endl;
    for(int i=0; i<no_variables; i++){
        priority_list.push_back(make_pair(i, 0));
    }
    assgn = equation;
    for(int i=0; i<no_clauses; i++)
        for(int j=0; j<no_variables; j++)
            assgn[i][j] = abs(equation[i][j]);
}

void SATool::assign_priority() {
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
void SATool::calculate_iter(){
    cout<<"C++ OpenMP version"<<endl;
    long comb = 1;
    comb = comb << no_variables;
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
    long i;
    bool valid;
# pragma omp parallel
{   
    vector<vector<bool> > private_sol;
    #pragma omp for nowait
    for(i=0; i<comb; i++) {
        valid = check_solution(solution_space[i]);
        if(valid){
            private_sol.push_back(solution_space[i]);
        }
    }
    #pragma omp critical
    solution_set.insert(solution_set.end(), private_sol.begin(), private_sol.end());
}
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    dif = chrono::duration_cast<chrono::microseconds>(elapsed).count();
    cout<<"printing solutions!"<<endl;
    print_solutions(solution_set);
}

// Checks if a particular combination of variables is valid for the equation
// or not.
bool SATool::check_solution(vector<bool> solution){
    bool is_zero = true;
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

void SATool::print_solutions(vector<vector<bool> > solution_set){
     cout<<"solution has "<<solution_set[1].size()<<" variables"<<endl;
     for(int i=0; i<solution_set.size(); i++) {
        cout<<"solution "<<i<<" : " ;
        for(int j=0; j<solution_set[j].size(); j++) {
            cout<<solution_set[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"Time elapsed is "<<dif<<" microseconds"<<endl;
}

// greedy SAT solver that starts with a fixed variable set and starts giving values
// to each variable and backtracks in case any of the clauses give a 0. If all 
// combinations tried return a 0 we claim there is no solution.
void SATool::calculate_greedy() {
    // vector of pairs variable solution consists of all variables
    // and their assignments
    volatile bool flag = false;
    #pragma omp parallel shared(flag)
{
    vector<pair<int, bool> > solution;
    // var_list consists of available variables that can be manipulated
    vector<int> var_list(no_variables);
    for(int i=0; i<no_variables; i++){
        var_list[i] = i;
    }
    // this vector contains all the elements that have been assigned values
    vector<int> sol_var;
    srand(time(NULL));
    // shuffling contents of vectors and beginning parallelizing the code
    random_shuffle(var_list.begin(), var_list.end(), myrandom);
    bool not_solved = true;
    bool attempted_once = false;
    bool backtrack = false;
    // starting greedy algorithm!
    auto start = chrono::high_resolution_clock::now();
    while(not_solved){
	if(flag){
	    break;
	}
        int chosen_var = var_list.back();
        bool satisfied = check_pair_sat(chosen_var, attempted_once, backtrack);
        if(satisfied) {
            if(attempted_once){
                solution.push_back(make_pair(chosen_var,false));   
            }
            else{
                solution.push_back(make_pair(chosen_var,true));
            }
            if(var_list.size()==0){
                cout<<"Equation is solved!"<<endl;
                not_solved = false;
	 	flag = true;
                break;
            }
            sol_var.push_back(chosen_var);
            var_list.pop_back();
        }
        else {
            if(!attempted_once){
                attempted_once = true;
                
            }
            else{
                if(sol_var.size()==0){
                    cout<<"No solution exists as variable "<<var_list.back()<<" is unsatisfiable!"<<endl;
                    break;
                }
                // backtrack step here
                if(solution.back().second == false){
                    backtrack = true;
		             cout<<"backtracking!"<<endl;
		            // ISSUE! NEED TO BACKTRACK ASSGN VARIABLE WHEN DOING BACKTRACK!
                    backtracker(solution.back().first);
		            cout<<"variable: "<<solution.back().first<<endl;
                    var_list.push_back(solution.back().first);
		            solution.pop_back();
                    sol_var.pop_back();
		        }
                else {
                    attempted_once = true;
                    backtrack = false;
                    backtracker(solution.back().first);
		            cout<<"variable to be checked for a zero assgn: "<<solution.back().first<<endl;
                    var_list.push_back(solution.back().first);
		            solution.pop_back();
                    sol_var.pop_back();

                }
            }
        }
    }

    if(!not_solved){
    	auto elapsed = std::chrono::high_resolution_clock::now() - start;
    	dif = chrono::duration_cast<chrono::microseconds>(elapsed).count();
    	int tid = omp_get_thread_num();
	cout<<"printing solutions!"<<endl;
    	cout<<"Thread ID no. "<<tid<<" solution is:"<<endl;
    	for(int i=0; i<no_variables; i++){
        	cout<<"variable "<<solution[i].first<<":\t"<<solution[i].second<<endl;
    	}
	
    	cout<<"Time taken for greedy algo: "<<dif<<" microseconds"<<endl;
    }
}
}

bool SATool::check_pair_sat(int rand_var, bool attempt, bool backtrack){
    short assignment;
    if(backtrack){
        return false;
    }
    else{
        if(!attempt){
        // run with rand_var = 1
            assignment = 1;   
        }
        else{
        // run with rand_var = 0
            assignment = 0;
        }
        // check if each clause is giving a sureshot zero or not
        // if no sureshot zeroes in any of the clauses return true
        // else return false
        for(int i=0; i < no_clauses; i++){
            if(equation[i][rand_var]==-1 && assignment==0){
                assgn[i][rand_var] = 1; 
            }
            else if(equation[i][rand_var]==1 && assignment==1){
                assgn[i][rand_var] = 1;
            }
            else{
                assgn[i][rand_var] = 0;
            }
       }  
        for(int i=0; i<no_clauses; i++){
            long sum = accumulate(assgn[i].begin(), assgn[i].end(), 0);
            if(sum==0)
                return false;
        } 
        return true;
    }

}

void SATool::backtracker(int var){
    for(int i=0; i<no_clauses; i++){
        if(equation[i][var] != 0){
            assgn[i][var] = 1;
        }
    }
}
int myrandom(int i) { return rand() % i; }


