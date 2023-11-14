//
//  music.cpp
//  Project_3
//
//  Created by Jodie Chen on 4/26/23.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

//checks if each note in a beat is valid
//return true if note is valid
bool isNote(string note) {
    
    if(note == "") {
        return true;
    }
    if(note.at(0) >= 'A' && note.at(0) <= 'G') { //starts with letter A-G
        if(note.length() == 1) { // just the letter A-G if it's just one character
            return true;
        } else {
            if(note.length() == 2) { //if note is 2 characters long
                if(note.at(1) == '#' || note.at(1) == 'b') //first char is A-G, followed by sharp or flat
                    return true;
                else if(note.at(1) >= '0' && note.at(1) <= '9') //first char is A-G, followed by 0-9
                    return true;
            } else if(note.length() == 3) { // if note is 3 characters long
                // second note has to be # or b, then followed by 0-9
                if((note.at(1) == '#' || note.at(1) == 'b') && note.at(2) >= '0' && note.at(2) <= '9' ) {
                    return true;
                }
            }
        }
        
    }
    
    return false;
}

// input is a beat: e.g. A3C#E/ from A3C#E//E//F#3A3D4/A3C#E/
// extract each potential note from the subpiece A3C#E (A3, C#, E) and pass each to isNote to check if it is valid note
// if a beat has at least one invalid note, return false
bool isBeat(string piece) {
    size_t start = 0;
    int last_index = piece.length()-1;
    
    
    if(piece.at(last_index) != '/') //if it doesn't end with /, not a beat
        return false;
    
    piece = piece.substr(0, last_index); // get everything except the last character
    
    if(piece == "") { // an empty beat is proper
        return true;
    }
    
    //if beat is not empty, for loop to extract potential notes
    for(size_t i=0; i< piece.length(); i++) {
        // not this--piece.at(i) == '/'
        if(piece.at(i) >= 'A' && piece.at(i) <= 'G') { // extract characters before a new letter using substring; if it reaches the end
            string note = piece.substr(start, i-start); // second character is len
            start = i;
            
            //check if the note is valid
            if(!isNote(note)) { //at least one invalid note >> not a beat
                return false;
            }
        }
    }
    
    //extract characters before the last index
    string last_note = piece.substr(start); //extracts everything before the last index
    
    if(!isNote(last_note)) { //at least one invalid note >> not a beat
        return false;
    }
    
    return true;
}

/*
 checks if entire user inputted piece is proper (may not necessarily be playable)
 */
bool isProperPiece(string piece) {
    
    size_t start = 0;
    //write another for loop that runs through piece and an if statement inside that checks if it reaches /
    //and passes each substring to isBeat() which calls isNote
    // last character is a slash
    
    //if user entered an empty piece, piece is proper
    if(piece == "")
        return true;
    if(piece.at(piece.size()-1) != '/') //if it doesn't end with /, not a beat--> improper
        return false;
    
    //extracts everything in between slashes or each beat and checks if it is valid in isBeat
    for (size_t i = 0; i< piece.size(); i++) {
        if(piece.at(i) == '/') {
            string bef_slash = piece.substr(start, i-start+1);
            
            start = i+1;
            //extract everything before slash = one beat
            if(!isBeat(bef_slash))
                return false;
        }
    }
    
    return true;
}

//professor provided function on transform each note based on component---octave, noteLetter, and accidental
char transformNote(int octave, char noteLetter, char accidentalSign)
{
      // This check is here solely to report a common CS 31 student error.
    if (octave > 9)
    {
        cerr << "********** transformNote was called with first argument = "
             << octave << endl;
    }

      // Convert Cb, C, C#/Db, D, D#/Eb, ..., B, B#
      //      to -1, 0,   1,   2,   3, ...,  11, 12

    int note;
    switch (noteLetter)
    {
      case 'C':  note =  0; break;
      case 'D':  note =  2; break;
      case 'E':  note =  4; break;
      case 'F':  note =  5; break;
      case 'G':  note =  7; break;
      case 'A':  note =  9; break;
      case 'B':  note = 11; break;
      default:   return ' ';
    }
    switch (accidentalSign)
    {
      case '#':  note++; break;
      case 'b':  note--; break;
      case ' ':  break;
      default:   return ' ';
    }

      // Convert ..., A#1, B1, C2, C#2, D2, ...
      //      to ..., -2,  -1, 0,   1,  2, ...

    int sequenceNumber = 12 * (octave - 2) + note;

    string keymap = "1!2@34$5%6^78*9(0qQwWeErtTyYuiIoOpPasSdDfgGhHjJklLzZxcCvVbBnm";
    if (sequenceNumber < 0  ||  sequenceNumber >= keymap.size())
        return ' ';
    return keymap.at(sequenceNumber);
}

/*
pass a beat (e.g. F#3A3D4/) and want to extract notes one at a time; for example F#3, A3, D4, etc
and transform respectively in transformNote; if encounters an invalid note, return ""?
 return value is a transformed subpiece for example, if input is F#3A3D4/ from A3C#E///E//F#3A3D4/A3C#E/
 F#3A3D4, parseNote returns is Qey
*/
string parseNote(string bef_slash) {
    string result = "";
    size_t start = 0;
    string note;
    
    
    if(bef_slash == "/") // if bef_slash is just a slash, just a space character
        result += ' ';
    
    for(size_t i = 0; i< bef_slash.size(); i++) { // iterating through C0C0DC0DD/, to get C0, C0, D, C0, D, D
        //substring(index of letter to index right before another letter)
        if((bef_slash.at(i) >= 'A' && bef_slash.at(i) <= 'G')|| bef_slash.at(i) == '/') { // extract characters before a new letter using substring; if it reaches the end
            note = bef_slash.substr(start, i-start); // second character is len
            start = i;
                        
            if(note.size() > 0) { //accounting for when note is not equal to bef_slash(0, 0) or empty string
                char noteLetter = note.at(0);
                int octave = 4;
                char accidentalSign = ' ';
                
                if(note.size() > 2) { //note is three characters long
                    accidentalSign = note.at(1);
                    octave = note.at(2) - '0';
                } else if(note.size() == 2) { // if note is two characters long, the first digit is letter, last character is either # or b
                    // checks if there is a number at all
                    if(note.at(1) == '#' || note.at(1) == 'b' ) {
                        accidentalSign = note.at(1);
                        octave = 4;
                    }
                    else
                        octave = note.at(1) - '0'; //account for when octave is set to char (note.at() returns char) and we want an int
                }
                
               
                if(transformNote(octave, noteLetter, accidentalSign) == ' ') //if note translates to ' ', then this is an unplayable note
                    return "";
                    
                result += transformNote(octave, noteLetter, accidentalSign);
                
                }
                    
            }
            
        }
    
    
    if(result.size()>1) { //if beat contains more than one note, it is a chord --> put brackets around
        result = '['+ result +']';
    }

    
    return result;
}

/*
 transforms user inputted piece if playable, otherwise instructions remains unchanged
 if proper but not playable, set badBeat = to the beat with the first unplayable note
 
 if proper, extract everything in between slashes in the for loop >> C0C0DC0D/, E#FbB#Cb/, B#9/ and pass
 to parseNote to check if playable
 return: an int (0, 1, or 2)
 */
int transformPiece(string piece, string& instructions, int& badBeat) {
    
    int start = 0;
    int beat = 1;
    string transformedNote;
    string transformedpiece = "";
    
    //isProperPiece checks isBeat and isNote
    
    //empty string is playable
    if(piece == "") {
        instructions = "";
        return 0;
    }
        
    if(!isProperPiece(piece)) //if not proper piece, nothing after executes (badbeats remains unchanged)
        return 1;
    
    
    //run if playable
    //if proper, extract everything in between slashes in the for loop >> C0C0DC0D/, E#FbB#Cb/, B#9/ and pass eaach beat
    //to parseNote
    for (size_t i = 0; i< piece.size(); i++) {
        if(piece.at(i) == '/') {
            string bef_slash = piece.substr(start, i-start+1);
           
            start = i+1;
            transformedNote = parseNote(bef_slash); // pass C0C0DC0DD/ to parseNote --> transformedNote = [  y yy]
            
            if(transformedNote == "") { //if unplayable note--> parseNote returns "" which also indicates unplayable piece
                badBeat = beat;
                return 2;
            }
            
            beat++; // keep track of the number of the beat for badBeat
            
            transformedpiece += transformedNote; //concatenate all the transformed notes to get the completely transformed piece
        }
    }
    
    //at this point it is playable
    instructions = transformedpiece;

    return 0; // if it is playable
}

int main()
{
    string instrs;
    int badb;
    
    cout << "isNote is " << isNote("Cb2") << endl; //isNote("Cb2") returns TRUE
    cout << "isBeat is " << isBeat("Cb2C3/") << endl; //isBeat("Cb2C3/") returns TRUE
    cout << "isProper is " << isProperPiece("Cb2C3/") << endl;
    cout << "isProper for A3B/Cb2/C7#/ is " << isProperPiece("A3B/Cb2/C#7/") << endl;
    
    cout << transformPiece("A3B/Cb2/C7#/", instrs, badb) << endl;
    
    instrs = "???"; badb = -999; // so we can detect whether these get change
    if(isProperPiece("A3B/Cb2/C7#/"))
        cout << "Passed test 0" << endl;
    if (isProperPiece("D5//D/"))
        cout << "Passed test 1: isProperPiece(\"D5//D/\")" << endl;
    if (!isProperPiece("D5//Z/"))
        cout << "Passed test 2: !isProperPiece(\"D5//Z/\")" << endl;
    if (transformPiece("D5//D/", instrs, badb) == 0  &&  instrs == "d y"  &&  badb == -999)
        cout << "Passed test 3: transformPiece(\"D5//D/\", instrs, badb)" << endl;
            instrs = "???"; badb = -999; // so we can detect whether these get changed
    if (transformPiece("D5//Z/", instrs, badb) == 1  &&  instrs == "???"  &&  badb == -999)
        cout << "Passed test 4: transformPiece(\"D5//Z/\", instrs, badb)" << endl;
    if (transformPiece("D5//D8/", instrs, badb) == 2  &&  instrs == "???"  &&  badb == 3)
        cout << "Passed test 5: transformPiece(\"D5//D8/\", instrs, badb)" << endl;
    if (transformPiece("///", instrs, badb) == 0   &&  instrs == "   "  &&  badb == 3)
        cout << "Passed test 6: empty string" << endl;
    cout << "return for A3B/Cb2/C7#/ is" << transformPiece("A3B/Cb2/C7#/", instrs, badb) << endl;
    cout << "return for A3B/Cb2/C#7/ is" << transformPiece("A3B/Cb2/C#7/", instrs, badb) << endl;
    cout << "return for C7#/ is" << transformPiece("C7#/", instrs, badb) << endl;
    cout << "return for A3B/Cb2/C#7/ is" << transformPiece("A3B/Cb2/C#7/", instrs, badb) << endl;
   
     
}

