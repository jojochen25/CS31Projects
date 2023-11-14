//
//  duty.cpp
//  Project_2
//
//  Created by Jodie Chen on 4/16/23.
//
// Purpose: prints import duty for cheese based on the value and in some cases, cheese type

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

int main() {
    
    

    string cheese_ty;
    double val;
    double duty = 0.0;
    string imp;
    double after_1000;
    
    cout << "Cheese type: ";
    getline(cin, cheese_ty);
    
    // checks if input for cheese type is empty
    if (cheese_ty == "") {
        cout << "---" << endl;
        cout << "You must enter a cheese type" << endl;
        return 1;
    }
    
    cout << "Value: ";
    cin >> val; //[3273\n] so buffer is [\n] after 3273 gets assigned to val
    cin.ignore(); // removes \n from the buffer

    if (val <= 0) {
        cout << "---" << endl;
        cout << "The value must be positive" << endl;
        return 1;
    }
    
    cout << "Importer: ";
    getline(cin, imp);
    
    if (imp == "") {
        cout << "---" << endl;
        cout << "You must enter an importer" << endl;
        return 1;
    }
    
        cout << "---" << endl;
    
    // if the program passes all the checks above, at this point in the program, value must be positive and all other inputs are valid
        if (val >= 1000) {
            duty = 0.013 * 1000;
            after_1000 = val - 1000;
            
            // if there is remaining value after first 1000 and it is less than 11000
            if (after_1000 <= 11000 && after_1000 > 0) {
                if(cheese_ty == "cheshire" || cheese_ty == "stilton") {
                    duty = duty + 0.012 * after_1000;
                } else {
                    duty = duty + 0.02 * after_1000;
                }
            } else { // if the remaining value after the first 1000 in the value exceeds 12000
                if(cheese_ty == "cheshire" || cheese_ty == "stilton") {
                    duty = duty + 0.012 * 11000;
                } else {
                    duty = duty + 0.02 * 11000;
                }
                val -= 12000;
                duty += 0.027*val;
                
            }
            
        } else if(val < 1000) { //if val is less than 1000 but positive still
            duty = val * 0.013;
        }
       
        // round duty to two decimal places
        cout.setf(ios::fixed);
        cout.precision(2);
        
        cout << "The import duty for " << imp << " is $" << duty << endl;
    
}

