#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<utility>
#include<algorithm>
#include<cmath>
#include<chrono>

bool sortbysec(const std::pair<int, int> &, const std::pair<int, int> &); 
class DPLL
{
    public:
        DPLL(std::vector<std::vector<short> >);
        void calculate_iter();
        void calculate_greedy();
    private:
        long long dif;
        std::vector<std::pair<int, int> > priority_list;
        std::vector<std::vector<short> > equation;
        int no_clauses;
        int no_variables;
        void init_();
        void assign_priority();
        bool check_solution(std::vector<bool>);
        void print_solutions(std::vector<std::vector<bool> >);
};
