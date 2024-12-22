#!/bin/bash
#delete all the files in the folder that ends with .txt
rm -f *.txt
rm -f data_generated/*.txt  
rm -f output/*.txt
# Compile the project
#!/bin/bash

g++ -std=c++17 -O2 -Wall -I. -I./utils -o final_project main.cpp
# Notify user
echo "Compilation finished. Executable file: final_project"

./final_project