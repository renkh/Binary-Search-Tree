/******************************************************************************
  Title          : main.cpp
  Author         : Renat Khalikov
  Created on     : December 12, 2016
  Description    : The program implements a binary search tree, it enters an 
  					interactive mode,  repeatedly displays a prompt (list, 
                    insert, merge, delete, clear all, find, save, quit) and
                    waits for the user to enter a command. After a command
                    is entered and the program responds to it, the prompt
                    is displayed again, unless the command was quit.
  Purpose        : re-implement the ContactList class using a binary search
                    tree instead of a list
  Usage          : g++ -o proj4 main.o contactlist.o
  Build with     : g++ -c -Wall main.cpp
*/
#include <iostream>
#include <fstream> // file io
#include <cstdlib> // exit() call
#include <string>
#include <cctype>
#include "contactlist.h"
using namespace std;

/*************************************************************************/
/** removeNonAlphaCharacters( word )
 * Removes all non alphabetic characters from a given string
 *
 * @pre      word contains a mix of alphabetic and numeric characters
 * @post     deletes all non alphabetic characters from given word
 * @param [inout] word contains string of characters to be deleted
 * @returns  void passed by reference
 */
void removeNonAlphaCharacters(string& word);
/*************************************************************************/

/*************************************************************************/
/** removeNonNumericCharacters( number )
 * Removes all non numeric characters from a given string
 *
 * @pre      number contains a mix of alphabetic and numeric characters
 * @post     deletes all non numeric characters from given number
 * @param [inout] number contains string of characters to be deleted
 * @returns  void passed by reference
 */
void removeNonNumericCharacters(string& number);
/*************************************************************************/

int main(int argc, char const *argv[])
{
    // looks for a file named contactlist in the current working directory
	ifstream inputFile("contactlist");
    if (inputFile.fail()){
        cerr << "contactlist file could not be opened" << endl;
        exit(1); // 1 indicates an error occurred
    }
    // displays on the console however many records it found
    else {
        cout << endl << endl;
        cout << "file opened successfully, read in ";
    }

    // create class objects
    ContactList linkedList;
    Name fullname;
    int recordsInserted = 0;

    // load input file into memory
    while( inputFile.good() ){
        string first, last, telephone, email;
        // input file is a csv file 
        // get each word that ends with a ','
        // unless its the last word which ends with a new line
        getline( inputFile, first, ',' );
        getline( inputFile, last, ',' );
        getline( inputFile, telephone, ',' );
        getline( inputFile, email, '\n');

        // set the first and last names into Name class
        // set the fullname, telephone number, and email address into Contact class
        fullname.set_first(first);
        fullname.set_last(last);
        Contact record(fullname, telephone, email);

        // load the Contact object, record, into a linked list returning
        // number of insertions done
        recordsInserted = linkedList.insert(record);
    }
    // displays on the console however many records it found
    cout << recordsInserted << " records" << endl;
    // interactive mode, repeatedly displaying a prompt and waiting for the 
    // user to enter a command
    string c;
    bool done = false;
    while(!done){
        cout << endl << endl;
        cout<<"contact list menu "<<endl;
        cout<<"----------------- "<<endl;
        cout<<"1. list all records "<<endl;
        cout<<"2. insert new record "<<endl;
        cout<<"3. merge contact list "<<endl;
        cout<<"4. delete record "<<endl;
        cout<<"5. clear all "<<endl;
        cout<<"6. find record "<<endl;
        cout<<"7. save "<<endl;
        cout<<"8. quit "<<endl;
        cout<<"enter number: ";
        cin >> c;

        // list all records
        if("1" == c){
            cout << endl << endl;
            cout<<"displaying all records"<<endl;
            cout<<"----------------------"<<endl;
            // returns how many records it displayed
            int count = 0;
            count = linkedList.display(cout);
            cout << count << " records in total\n";
        }

        // insert new record
        else if ("2" == c){
            string lastname, firstname, telNum, emailAddr;
            cout << endl << endl;
            cout << "lastname: ";
            cin >> lastname;
            // removes all non alphabetic characters
            removeNonAlphaCharacters(lastname);

            cout << "firstname: ";
            cin >> firstname;
            // removes all non alphabetic characters
            removeNonAlphaCharacters(firstname);

            cout << "telephone number (optional): ";
            cin.ignore();
            getline(cin, telNum);
            // removes all non numeric characters
            removeNonNumericCharacters(telNum);

            cout << "email address (optional): ";
            cin.sync();
            getline(cin, emailAddr);

            fullname.set_first(firstname);
            fullname.set_last(lastname);
            Contact record(fullname, telNum, emailAddr);
            int count=1;
            count = linkedList.insert(record);
            cout<<endl;
            // if insert does not insert
            // it will set count to 0
            if (0 == count){
                cout << "record not inserted\n";
            }
            // record has been inserted
            else{
                cout << "record inserted successfully \n";
            }
        }

        // merge contact list
        else if ("3" == c){
            cout << endl << endl;
            // Prompt the user for the pathname of a file containing a contact list
            // in the same format as the contactlist file
            cout << "please enter the pathname of a file: ";
            string pathname;
            cin.ignore();
            getline(cin, pathname);

            // inputs user defined pathname
            ifstream newFile (pathname.c_str());
            if (newFile.fail()){
                cerr << "could not open: " << pathname << endl;
                exit(1); // 1 indicates an error occurred
            }
            int count = 0;
            int numberMerged = 0;
            // load input file into memory
            while( newFile.good() ){
                string first, last, telephone, email;
                // input file is comma seperated so get each word that ends with ','
                // unless its the last word which ends with a new line
                getline( newFile, first, ',' );
                getline( newFile, last, ',' );
                getline( newFile, telephone, ',' );
                getline( newFile, email, '\n');

                // set the first and last names into Name class, and set the fullname,
                // telephone number, and email address into Contact class
                fullname.set_first(first);
                fullname.set_last(last);
                Contact record(fullname, telephone, email);

                // load the Contact object, record, into a linked list returning
                // number of insertions done
                //recordsInserted += linkedList.insert(record);
                count = linkedList.insert(record);
                numberMerged++;
            }
            // displays how many records inserted
            cout << numberMerged << " records inserted\n";
        }

        // delete record
        else if ("4" == c){
            cout << endl << endl;
            string lastname, firstname, telNum, emailAddr;
            cout << "lastname: ";
            cin >> lastname;
            cout << "firstname: ";
            cin >> firstname;
            cout << "telephone number (optional): ";
            cin.ignore();
            getline(cin, telNum);
            cout << "email address (optional): ";
            cin.sync();
            getline(cin, emailAddr);
            fullname.set_first(firstname);
            fullname.set_last(lastname);
            Contact record(fullname, telNum, emailAddr);
            int count = 0;
            count = linkedList.remove(record);
            cout << endl << endl;
            cout << "deleted " << count << " records\n";
        }

        // clear all
        else if ("5" == c){
            cout << endl << endl;
            int count = 0;
            count = linkedList.make_empty();
            cout << count << " records deleted\n";
        }

        // find record
        else if ("6" == c){
            string lastname, firstname, telNum, emailAddr;
            // the user can leave any member blank
            cout << endl << endl;
            cout << "lastname (optional): ";
            cin.ignore();
            getline(cin, lastname);
            cout << "firstname (optional): ";
            cin.sync();
            getline(cin, firstname);
            cout << "telephone number (optional): ";
            cin.sync();
            getline(cin, telNum);
            cout << "email address (optional): ";
            cin.sync();
            getline(cin, emailAddr);
            cout << endl << endl;
            // set user info into class objects
            Name find_fullname;
            find_fullname.set_first(firstname);
            find_fullname.set_last(lastname);
            Contact find_record(find_fullname, telNum, emailAddr);

            int count = 0;
            count = linkedList.find(cout, find_record);
            if (count > 0){
                cout << count << " records found\n";
            }
        }

        // save
        else if ("7" == c){
            int count = 0;
            count = linkedList.save();
            cout << endl;
            cout << "saved " << count << " records to contactlist.bkp\n";
        }

        // quit
        else if("8" == c){
            done = true;
        }

        // invalid command
        else{
            cout << endl << endl;
            cout << "invalid number, please try again..." << endl;
        }
    }
	return 0;
}

/** removeNonAlphaCharacters( word )
 * Removes all non alphabetic characters from a given string
 *
 * @pre      word contains a mix of alphabetic and numeric characters
 * @post     deletes all non alphabetic characters from given word
 * @param [inout] word contains string of characters to be deleted
 * @returns  void passed by reference
 */
void removeNonAlphaCharacters(string& word)
{
    for(string::iterator i = word.begin(); i != word.end(); i++){
        if( !isalpha( word.at( i - word.begin() ) ) ){
            word.erase(i);
            i--;
        }
    }
}

/** removeNonNumericCharacters( number )
 * Removes all non numeric characters from a given string
 *
 * @pre      number contains a mix of alphabetic and numeric characters
 * @post     deletes all non numeric characters from given number
 * @param [inout] number contains string of characters to be deleted
 * @returns  void passed by reference
 */
void removeNonNumericCharacters(string& number)
{
    for(string::iterator i = number.begin(); i != number.end(); i++){
        if( !isdigit( number.at( i - number.begin() ) ) ){
            number.erase(i);
            i--;
        }
    }
}

