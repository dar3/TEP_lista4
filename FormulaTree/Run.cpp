#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <sstream>
#include <set>
#include "CNode.h"
#include "CTree.h"

using namespace std;


void readValues(stringstream& ss, vector<double>& values) {
    double value;
    while (ss >> value) {
        values.push_back(value);
    }
}

int main() {
    CTree tree;

    while (true) {
        cout << message_enter;
        string command;
        getline(cin, command);

        stringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == command_comp) {
           

            vector<double> values;
            readValues(ss, values);

            
            if (values.size() != tree.numberOfVariablesInTree()) {
                cout << message_err << endl;
            }
            else {
                
                map<string, double> variableValues;
                for (size_t i = 0; i < values.size(); ++i) {
                    variableValues[tree.getVariableNameAtIndex(i)] = values[i];
                }

                
                double result = tree.evaluate(
                    tree.getRoot(), variableValues);

                cout << message_result << result << endl;
            }
        } 
        else if (cmd == message_print) {
            
            cout << message_tree;
            if (!tree.getRoot()) {
                cout << message_no_tree << endl;
            }
            else {
                tree.printTree(tree.getRoot());
                cout << endl;
            }
        }
        else if (cmd == message_vars) {
            
            set<string> variables;
            tree.collectVariables(tree.getRoot(), variables);
            cout << message_variables;
            for (const auto& variable : variables) {
                cout << variable << message_space;
            }
            cout << endl;
        }
        else if (cmd == command_enter) {
            string formula;
            getline(ss, formula);
            tree.parseExpression(formula);
            cout << message_parsed << endl;
        } 
        else if (cmd == command_del) {
            tree.deleteTree(tree.getRoot());
            cout << message_deleted_tree << endl;
        }
        else if (cmd == command_join) {
            CTree newTree;
            string formula;
            getline(ss, formula);
            newTree.parseExpression(formula);
            tree += newTree;
            cout << message_merged_tree << endl;
        }
        else if (cmd == command_exit) {
            cout << command_exit << endl;
            return 0;
        }

        else if (cmd == command_levels) {
            
            if (!tree.getRoot()) {
                cout << message_no_tree << endl;
            }
            else {
                tree.printTreeRows(tree.getRoot());
                cout << endl;
            }
        }
    }


  
  

    return 0;
}
