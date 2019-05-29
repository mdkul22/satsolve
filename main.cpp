#include <fstream>
#include "satool.h"

using namespace std;
vector<short> get_clause(string);
vector<vector<short> > getEquation(vector<string>, int);

/*
Usage:
    This main function can be called by running the ./main <filename>
    which will read the variables and the clauses present and convert 
    the data into a vector of vectors whose data will be formulated 
    by a SAT algorithm of our liking. 
*/ 
int main( int argc, char *argv[]) {
    if (argc != 2){
        cout<<"Usage: Add file name as argument"<<endl;
        return 1;
    }
    
    string line;
    int variables, clauses;
    ifstream myfile(argv[1]);
    vector< vector<short> > pos_equation; 
    vector<string> string_arr;
    if(myfile.is_open()){
        myfile >> variables;
        cout<<"No of variables: "<<variables<<endl;
        myfile >> clauses;
        cout<<"No of clauses: "<<clauses<<endl;
        while (getline(myfile, line)) {
            if(line.length() > 0)
            {
                string_arr.push_back(line);
                cout<<string_arr.back()<<endl;
            }
        }
        myfile.close();
        pos_equation = getEquation(string_arr, clauses);
        for(int i=0; i<pos_equation.size(); i++){
            if(pos_equation[i].size() == 0){
                cout<<"erasing"<<endl;
                pos_equation.erase(pos_equation.begin()+i);
            }
        }
        
    }
    else {
        cout<<"ERROR: File specified either does not exist or incorrect file name specified!"<<endl;
        exit(1);
    }
    SATool calc(pos_equation);
 //   calc.calculate_iter();
    calc.calculate_greedy();
    cout<<"Everything seems right!"<<endl;
    return 0;
}

vector<short> get_clause(string var) {
    vector<short> clause;
    for(int i=0; i<var.length(); i++){
        if(!var.compare(i, 1, "0")){
            clause.push_back(-1);
        }
        else if(!var.compare(i, 1, "1")){
            clause.push_back(0);
        }
        else if(!var.compare(i, 1, "2")){
            clause.push_back(1);
        }
        else{
        cout<<"ERROR: INCORRECT INPUT IN FILE, NOT READABLE! value: "<<var[i]<<endl;
        exit(1);
        }
    }
    return clause;
}

vector<vector<short> > getEquation(vector<string> string_arr, int clauses) {
    vector<vector<short> > equation_list;
    for(int i=0; i<clauses; i++){
        equation_list.push_back(get_clause(string_arr[i]));
    }
    return equation_list;
}

