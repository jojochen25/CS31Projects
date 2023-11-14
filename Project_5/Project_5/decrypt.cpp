//
//  decrypt.cpp
//  Project_5
//
//  Created by Jodie Chen on 5/17/23.
//
#include <cstring>
#include <cctype>
#include <iostream>
using namespace std;

void cleantext(const char crib[], char cleanCrib[], bool cipher) {
    
    int i;
    int j = 0;
    bool prev_nonLetter = false;
    
    for(i = 0; crib[i] != '\0'; i++) { // does doing crib[i]
        
        char currentChar = tolower(crib[i]);
        if(isalpha(currentChar)) { //if it is a letter, add to copycrib
            //if previous is not letter, add whitespace
            if(prev_nonLetter && j != 0) { //if not letter, add whitespace and not beginning of output array
                cleanCrib[j] = ' '; //add a space for any special characters except '\n' if cipher
                j++;
            }
            prev_nonLetter = false;
            cleanCrib[j] = currentChar;
            j++;
            
        } else if (cipher && currentChar == '\n') { //leave the \n in cipher only
            prev_nonLetter = false;
            cleanCrib[j] = currentChar;
            j++;
        } else { //if not letter
            prev_nonLetter = true;
        }
        
    }
    
    cleanCrib[i] = '\0';
     //RETURN POINTER
}

//SHOULD WORK
//split into 2d array for both cipher and crib;

//last_match: index of last character in matching cipher message --should be a ' ' or '\n' or '\0' at this point
//check subset of cipher matching in length if it matches in mapping
bool checkMap(int last_match, char crib[], char cipher[], char key[]) {
    int start_match = last_match - strlen(crib);
    
    for(int start = start_match; start < last_match; start++){
        cerr << cipher[start];
    }
    cerr << endl;
    for(int i =0; i<26; i++) { //key contains only ? initially
        key[i] = '?';
    }
    
    /*
     run loop through cipher starting at start_match and ending at last_match
     
     if no match, return false at the end
     */
    
    int cribIndex = 0;
    
    //whileloop? outside that checks if cipher[start_match] and crib[cribIndex] at corresponding index is a space
    cerr << "///////match length passed: checking mapped characters//////" << endl;
    
    for(int i = start_match; i< last_match; i++) {
        if(isalpha(cipher[i]) && isalpha(crib[cribIndex])) { //if character in cipher at index i is a letter, do the mapping
            if(key[cipher[i] - 'a'] == '?') { //first iteration
                key[cipher[i] - 'a'] = crib[cribIndex];
                cribIndex+=1;
            } else { //already a letter there at index cipher[i] - 'a' (!= '?')
                if(crib[cribIndex] == key[cipher[i] - 'a']) {//if repeat of a letter in cipher matches the existing mapping in key ('ala' in cipher = 'tut' in crib)
                    cribIndex+=1;
                } else { //repeat doesn't match
                    cerr << "checkMap not passed: current crib letter is " << crib[cribIndex] << " when " << cipher[i] << " in cipher should map to " << key[cipher[i] - 'a'] << ": NOT A MATCH" << endl;
                    return false;
                    
                }
            }
            
        } else if(!isalpha(cipher[i]) && isalpha(crib[cribIndex])) { // example [w'wt qeirtk] and [alan turing\0]
            cerr << "No match bc space in cipher when there is an alaphabet in crib" << endl;
            return false;
        } else if (isalpha(cipher[i]) && !isalpha(crib[cribIndex])){
            cerr << "No match bc alphabet in cipher when there is a space in crib" << endl;
            return false;
        } else { // when cipher[i] and crib[cribIndex] are both not alphabet --> match so continue continue checking
            cribIndex++;
        }
    }
    
    cerr << "KEY IS NOW:" << endl;
    for(int i = 0; i < 26; i++) {
        cerr << key[i] << " ";
    }
    
    cerr << endl;
    
    //loop to check if there are repeats in the key array. if there is --> return false
    for(int i = 1; i < 26; i++) {
        for(int j = 0; j < i; j++) {
            if(key[i] != '?' && key[j] != '?') {
                if(key[i] == key[j]) {
                    cerr << "checkMap not passed: not a match because " << key[i] << " appears in " << "index " << i << " and " << j << endl;
                    return false; //if there is at least in key array, no match so return false
                }
            }
        }
        //while
    }
    
    return true; //if crib matches cipher subset
}


bool findMatchLength(char crib[], char cipher[], char key[]) {
    //counts the num of ' '  and \n in cipher which is to be used as the length of the ind array
    int num_words=0;
    for(int i = 0; cipher[i] != '\0'; i++) {
        if(i == 0 && isalpha(cipher[i])) { //if first letter in cipher is an alphabet--> word
            num_words++;
        } else if(!isalpha(cipher[i-1]) && isalpha(cipher[i])) {
            num_words++;
        }
    } //this is correct
    
    cerr << "number of words " << num_words << endl;
    
    //before was num_words
    int ind[7600]; //array containing indices of where the first letter of each word is
    int c = 0;

    for(int count_w = 0; cipher[count_w]!='\0'; count_w++) {
        if(count_w == 0 && isalpha(cipher[count_w])) {
            ind[c] = count_w;
            c+=1;
        } else if(!isalpha(cipher[count_w-1]) && isalpha(cipher[count_w])) {
            ind[c] = count_w;
            c+=1;
        }
    }
    
    cerr << "indices of first letter in each word: " << endl;
    for(int i = 0; i < num_words; i++) {
        cerr << ind[i] << "(" << cipher[ind[i]] << ") ";
    }
    cerr << endl;
    
   
    int last_match;
    int start_match;
    //bool found_match;
    
    //keeps running into trailing null bytes
    cerr << "from findMatchLength function: " << endl;
    for(int k = 0; k < num_words; k++) { //running through array containing indices of first letter in each word
        cerr << strlen(crib) + (ind[k]) << " [" << cipher[strlen(crib) + (ind[k])] << "]"<< endl;
         // to account for a match at the very end, LINE MIGHT NOT BE NECESSARY
            //if last character is ' ', '\n', or '\0', it is a match
        if(strlen(cipher) - ind[k] < strlen(crib)) {
            cerr << "last potential match is not same length as crib! for index " << cipher[ind[k]] << endl;
            return false;
        }
            if(cipher[strlen(crib) + (ind[k])] == ' ' || cipher[strlen(crib) + (ind[k])] == '\n' || cipher[strlen(crib) + (ind[k])] == '\0') {
                last_match = strlen(crib) + (ind[k]);
                start_match = ind[k];
                
                bool check_match = true;
                for(int start = start_match; start < last_match; start++) { //account for if there is a \n separating the matching cipher--> not a match
                    if(cipher[start] == '\n') {
                        cerr << "findMatchLength not passed: not a match because there is a new line in the middle for indices " << start_match << "to " << last_match << endl;
                        check_match = false; //no match
                    }
                }
                //call mapping function
                //found_match = true;
                if(check_match) { //at this point, no \n in the cipher substring
                    cerr << "Matching length passed--index of last character in match: " << last_match << endl;
                    if(checkMap(last_match, crib, cipher, key)) {//if crib matches cipher in mapping, ultmate match!, no need to check for other potential matches
                        return true;
                    }
                }
            }
        }
    
    
    cerr << "no match found at all " << endl;
    return false; //no match found in length and mapping
}

void translate(const char cipher[], char key[]) {
    
    for(int i = 0; i < 26; i++) {
        key[i] = toupper(key[i]);
    }
    
    cerr << "//////MATCH FOUND: here is the translated text///////////" << endl;
    
    for(int i = 0; cipher[i] != '\0'; i++) {
        char c = tolower(cipher[i]);
        if(isalpha(c)) { // if it is a letter
            char new_c = key[c - 'a'];
            if(new_c != '?')
                cout << new_c;
            else
                cout << c;
        } else {
            cout << c;
        }
    }
}

bool decrypt(const char ciphertext[], const char crib[]) {
    
    //cipher max 7500
    //crib: max 100
    
    if(strlen(crib) > 100) {
        cerr << "error: crib is longer than 100!" << endl;
        return false;
    } else if (strlen(crib) == 0){ // if less than 100
        cerr << "error: crib is empty!" << endl;
        return false;
    }
    
    bool flag = true;
    for(int i = 0; crib[i] != '\0'; i++) {
        if(isalpha(crib[i]))
            flag = false;
    }
    
    if(flag) {// if all non-alphabet in crib, meaning crib has no words so return false;
        cerr << "error: crib contains all nonalphabets!" << endl;
        return false;
    }
    
    //at this point crib is valid
    if(strlen(ciphertext) == 0)
        return false;
    
    char cleanCrib[101]; //+1 to account for null byte at the end
    cleantext(crib, cleanCrib, false);
    
    char key[26];
    
    char cleanCipher[7600];
    cleantext(ciphertext, cleanCipher, true);
    
    bool found_match = findMatchLength(cleanCrib, cleanCipher, key); //inside found_match, should call checkMap if findMatchLength passes
    
    if(found_match) { //if match is found in cipher, translate the text and return true
        
        translate(ciphertext, key);
        return true;
        
    }
    
    return false;
}
//caught\0 --> cauht\0

/*
void deleteG(char *chr) {
    
    char *chr2;
     //set the pointer chr2 to where pointer chr is.
    while(*chr != 0) {
        if(*chr == 'G' || *chr == 'g') {
            chr2 = chr;
            while(*chr2 != 0) {
                *chr2 = *(chr2 + 1);
                chr2++;
            }
           
        }
        
        chr++;
    }
}
 */


const char* findTheChar2(char *ptr, char chr) {
    while(*ptr != 0) {
        if(*ptr == chr)
            return ptr;
        ptr++;
    }
    return nullptr;

}
const char* findTheChar(const char str[], char chr)
{
        for (int k = 0; str[k] != 0; k++)
            if (str[k] == chr)
                return &str[k];

        return nullptr;
}


int main() {
    
    pd.print("Hello C++");

    char str[] = {'h', 'e', 'l', 'l', 'o'};
    cout << "pointer: " << findTheChar(str, 'l') << endl;
    cout << "pointer2: " << findTheChar2(str, 'l') << endl;


    
    
    char msg[100] = "I recall the glass gate next to Gus in Lagos, near the gold bridge.";
           // deleteG(msg);
           // cout << msg;
    
    int arr[3] = { 5, 10, 15 };
            int* ptr = arr; //behind the scenes: int* ptr = &a[0]

            *ptr = 10;          // set arr[0] to 10; now arr[0] is 10
            *(ptr + 1) = 20;      // set arr[1] to 20; now arr[1] = 20 (de-references pointer and just increments the content
            ptr += 2; //arr[3] =
            ptr[0] = 30;        // same as *ptr + 0 =30;


         while (ptr >= arr) //comparing memory address (ptr stores a memory address, find something to follow memory address in memory to see whatever that is stored there
            {

                cout << ' ' << *ptr;    // print values
        ptr--;
            }
        cout << endl;

    
    char cipher[] = "Rswjo qgx Tmeuo sgjsy jds vqgf vo jds vqzf xbby\nUdbyjo iqcju cg wybgj cg jds esjqiqo zqy\nXbg'j rsj jds jsrrsy jycn jds ucrsgj qrqyt.\nZU 31 cu zdqrrsgecge!";
     
    /*
    char cipher[] = "Fh lgr rntoy rkwndyk ahna'y\n phklk ahk mgtks dfsfy.\n";
     */
    char crib[] =        "silent-----alarm";
    
    cout << decrypt(cipher, crib);
    
}



