//
//  Project_4.cpp
//  Project_4
//
//  Created by Jodie Chen on 5/5/23.
//
#include <string>
#include <iostream>
#include <cassert>
using namespace std;
int countMatches(const string a[], int n, string target) {
    int count = 0;
    if(n < 0)
        return -1;
    for(size_t i = 0; i < n; i++) {
        if(a[i] == target)
            count++;
    }
    
    return count;
}

/*test cases
 checked all edge cases
 */
int detectMatch(const string a[], int n, string target) {
    if(n<0)
        return -1;
    
    for(int i = 0; i < n; i++) {
        if(a[i] == target)
            return i;
    }
    
    return -1;
}
/*
 CHECKED ALL EDGE CASES
 what if n is greater than the size of array --don't need to check this
 what if n is less than the size of array
 don't have to check for this since no way of knowing how many items are in an array
 */
bool detectSequence(const string a[], int n, string target, int& begin, int& end) { /* checked, not sure if there's edge cases */
    /*
     begin = position of first occurence of target
     end = last ocurence of target
     */
    /*
     for loop with an if statement inside that checks for earliest convenience
     */
    bool flag = false;
    
    if(n<0)
        return false;

    for(int i = 0; i<n; i++) {
        if(a[i] == target) {
            flag = true;
            begin = i;
            break;
        }
        // if no string in a is equal to target
    }
    
    if(flag == false)
        return false;
    
    int j = begin;
    
    while(j < n) { //set end to the last occurence of target in earliest consecutive sequence.
        if(a[j+1] == target)
            j++;
        else
            break;
    }
    
    end = j;
    
    return true;
}

int detectMin(const string a[], int n) { //"paul", "danny", "george", "amy", "june", "amy"
    string temp_min = a[0];
    int count = 0; //keep track of the index
    
    if(n<=0)
        return -1;
    
    for(int i = 1; i<n; i++) {
        if(a[i] == temp_min) //if there is more than one string, want smallest position, so skip if encounter repeats of temp_min
            continue;
        if(a[i]<temp_min) {
            temp_min = a[i];
            count = i;
        }
    }
    
    return count;
}

int moveToFront(string a[], int n, int pos) { //run with invalid inputs
    /*
     store a[pos] to a new variable b;
     remove a[pos] from the array a[]
     concatenate b to a[]
     */
    if(pos >= n || n < 0 || pos < 0) // will need this if statement for all functions that return int
        return -1;
    
    string temp = a[pos]; // "george"
    //use temp
    //switch george and danny (george is in second position), then switch george and paul (so paul is in first position)
    //danny paul
    //swap strings, still need to use temp
    for(size_t i = pos; i > 0; i--) {
        a[i] = a[i-1];
    }
    
    a[0] = temp;
    
    return pos;
}

/*checked*/
int detectDifference(const string a1[], int n1, const string a2[], int n2) {
    int size;
    
    //check should only go for as long as the shorter of the two array
    if(n1<0 || n2<0) //if number of items is negative for any array, return -1
        return -1;
    
    if(n1<n2)
        size = n1;
    else
        size = n2;
    
    for(int i = 0; i < size; i++) {
        if(a1[i] != a2[i])
            return i;
    }
    
    if(n1<=n2)
        return n1;
    
    return n2;
}

int deleteDups(string a[], int n) {// "danny", "june", "amy", "amy", "george", "george", "george", "amy", "amy"

    int j;
    int count = n; //keep track of number of retained items
    if(n<0)
        return -1;
    
    for(int i = 1;i < count; i++) {
        if(a[i] == a[i-1]) { //checks if there is a consecutive sequence
            count--;
            for(j = i+1; j<count; j++) {
                a[j-1] = a[j]; //shifts all items after consecutive sequence to the left
            }
            i--;
        }
    }
    
    return count;
    
}


int moveToBack(string a[], int n, int pos) {
    string temp = a[pos];
    if(pos >= n || n < 0 || pos < 0)
        return -1;
    
    for(size_t i = pos; i < n-1; i++) {
        a[i] = a[i+1];
    }
    a[n-1] = temp;
    
    return pos;
}

bool contains(const string a1[], int n1, const string a2[], int n2) {
    int counter = 0;
    int j = 0;
    int i;
    int curr_a1 = 0;
    
    for(i = 0; i<n2; i++) { // run through  {"danny", "amy", "june"} or a2
        for(j = curr_a1; j<n1; j++) {
            if(a2[i] == a1[j]) {//if "danny" in a2 is found in a1 "paul", "danny", "george", "amy", "june", "george"
                curr_a1 = j+1;
                counter++; //increment every time you find a match
                break; //break after you find matching element
            }
        }
    }
    
    //if a1 does not contain a2
    if(counter == n2)
        return true;
        
    return false;
    
}

int meld(const string a1[], int n1, const string a2[], int n2,
         string result[], int max) {
    if(n1<0 || n2 < 0 || max < 0)
        return -1;
    
    if((n1+n2)>max)
        return -1;

    for(int i = 1; i<n1; i++) {
        if(a1[i-1] > a1[i]) //check if a1 is not in nondecreasing order
            return -1;
    }
    
    for(int j = 1; j<n2; j++) {
        if(a2[j-1] > a2[j]) //check if a2 is not in nondecreasing order
            return -1;
    }
     
    
    if(n1+n2>max) {
        return -1;
    }
   
    int index_a1 = 0;
    int index_a2 = 0;
    
    for(int i = 0; i<n1+n2; i++) {
        //if index_a1 reaches the end of the first array, store the array items starting from the index after the last item in the first array
        //to result
        
        if(index_a1 >= n1) {
            //store array items starting from the index that the other array runs out of, and store the remaining items into tresult
            for(int j = index_a1; j<n2; j++) {
                result[i] = a2[j];
                i++;
            }
            
            return n1+n2;
        }
        if(index_a2 >= n2) { //same as above
            for(int j = index_a2; j<n1; j++) {
                result[i] = a1[j];
                i++;
            }
            
            return n1+n2;
        }
         
            //store to result
        if(a1[index_a1] < a2[index_a2]) { //store items from first array to result if it is smaller than corresponding item in second array
            result[i] = a1[index_a1];
            index_a1++;
        }
        else {
            result[i] = a2[index_a2];
            index_a2++;
        }
    }

    return n1+n2;
}

int separate(string a[], int n, string separator) {
    /* make two arrays--one that is smaller than "june" and another that is larger than "june"*/
    /*try swapping the first and last element and */
    
    int i = 0; //where we are in the array
    //k to make sure we only check n elements (4 elements)--counter
    
    if(n < 0) // will need this if statement for all functions that return int
        return -1;
    
    
    for(int k = 0; k<n; k++) {
        if(a[i] < separator) { //if item is smaller than separator, move the item to the beginning of the array
            moveToFront(a, n, i);
        }
        else if(a[i] > separator) { //if item is smaller than separator, move the item to the back of the array
            moveToBack(a, n, i);
            continue; //skip iteration
        }
        i++;
    }
    
    for(int j = 0; j<n; j++ ){
        if(a[j]>=separator)
            return j;
    }
    
    return n;
}


int main() {
    
    string h[7] = { "isaac", "paul", "danny", "june", "", "amy", "danny" };
            assert(countMatches(h, 7, "danny") == 2);
            assert(countMatches(h, 7, "") == 1);
            assert(countMatches(h, 7, "fumi") == 0);
            assert(countMatches(h, 0, "danny") == 0);
            assert(detectMatch(h, 7, "danny") == 2);
            assert(detectMatch(h, 2, "danny") == -1);
            int bg;
            int en;
            assert(detectSequence(h, 7, "amy", bg, en) && bg == 5 && en == 5);

            string g[4] = { "isaac", "paul", "amy", "danny" };
            assert(detectMin(g, 4) == 2);
            assert(detectDifference(h, 4, g, 4) == 2);
            assert(contains(h, 7, g, 4));
            assert(moveToBack(g, 4, 1) == 1 && g[1] == "amy" && g[3] == "paul");
        
            string r[4] = { "paul", "amy", "danny", "isaac" };
            assert(moveToFront(r, 4, 2) == 2 && r[0] == "danny" && r[2] == "amy");
        
            string e[5] = { "paul", "paul", "paul", "danny", "danny" };
            assert(deleteDups(e, 5) == 2 && e[1] == "danny");
        
            string u[4] = { "george", "george", "naomi", "paul" };
            string v[4] = { "amy", "danny", "george", "isaac" };
            string w[10];
            assert(meld(u, 4, v, 4, w, 10) == 8 && w[5] == "isaac");
            assert(separate(h, 7, "isaac") == 4);

            cout << "All tests succeeded" << endl;
            
            int result = meld(u, 5, v, 4, w, 8);
            cout << result << endl;
            
            int result2 = meld(u, 5, v, 4, w, -1);
            cout << "meld for bad param passed" << result2 << endl;
    
    string d[9] = {
        "george", "george", "george", "amy", "amy", "george"
    };

    
    int result3 = deleteDups(d, -1);
    cout << "delete dups" << result3 << endl;
            
        
            
}



/*
 removing elements from array, is there some kind of delete function
 
 */
