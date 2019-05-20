#/usr/bin/python3

import random
import sys

""" 
---------------------------------------------------------------------
Usage: This py file helps generate sample test cases for SAT solving

python vargen.py <no of variables> <no of clauses> <filename to write in>
---------------------------------------------------------------------
"""
if sys.argv[1] == "--help":
    print("Usage: This py file helps generate sample test cases for SAT solving\n python vargen.py <no of variables> <no of clauses> <filename to write in>")
    exit()

args = sys.argv

var_no = args[1]

clause_no = args[2]

filename = args[3]

print("Generating random variable equation")

f_l = open(filename, "w+")
f_l.write(str(var_no))
f_l.write("\n")
f_l.write(str(clause_no))
f_l.write("\n")

for i in range(int(clause_no)):
    x = ""
    for j in range(int(var_no)):
        x += str(random.randint(0,2))
    f_l.write(x)
    f_l.write("\n")

f_l.close()

