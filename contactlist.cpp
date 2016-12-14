/*****************************************************************************
  Title          : contactlist.h
  Author         : Renat Khalikov
  Created on     : December 12, 2016
  Description    : implementation of header file contactlist.h
  Purpose        : 
  Usage          : 
  Build with     : g++ -c -Wall contactlist.cpp
*/

#include "contactlist.h"
#include <string>
#include <iostream>
#include <cstdlib> // exit() call
#include <fstream>
using namespace std;

// Name class defined below
Name::Name(){ }

// returns firstname
string Name::first()
{
    return fname;
}

// returns lastname
string Name::last()
{
    return lname;
}

// sets firstname
void Name::set_first(string f_name)
{
    fname = f_name;
}

// sets lastname
void Name::set_last(string l_name)
{
    lname = l_name;
}

//  The Contact class defined below is needed by the ContactList ADT. This class is not documented either because the methods are simple accessors and mutators.
Contact::Contact (){ }

// class constructor with 3 parameters
Contact::Contact (Name person, string tel_num, string email_addr)
{
    name = person;
    telephone = tel_num;
    email = email_addr;
}
void Contact::set (string fname, string lname, string tel_num, string email_addr)
{
    Name aName;
    aName.set_first(fname);
    aName.set_last(lname);
    telephone = tel_num;
    email = email_addr;
}
void Contact::get_name  (Name& fullname)
{
    fullname = name;
}
void Contact::get_tel   (string& tel_num)
{
    tel_num = telephone;
}
void Contact::get_email (string& email_addr)
{
    email_addr = email;
}

void Contact::set_name  (string fullname) { }
void Contact::set_tel   (string tel_num) 
{
    telephone = tel_num;
}
void Contact::set_email (string email_addr) 
{
    email = email_addr;
}

//  CONTACTLIST CLASS PUBLIC MEMBERS
/** Constructor: 
 * Creates an empty contact list.
 * @pre  None
 * @post The object is empty.
*/
ContactList::ContactList()
{
    root = NULL;    // root of the tree
    insertSize = 0; // size of the tree
    count = 0;      // general count used for find and delete
}

/** Destructor 
 * Deletes all memory used by the contact list.
 * @pre None
 * @post The list is empty
 * Note that this is not called by any code.
 */
ContactList::~ContactList()
{
    destroy(root);  // recursively calls destroy to delete all nodes of the tree
    root = NULL;    // set root to null
    insertSize = 0; // set tree size to zero
}

/** display(output)
 * Outputs the contact list in sorted order by last name, with the first 
 * name as the secondary key. The data is spaced on the line so that each 
 * data field is  aligned with the one above. The implementation is free 
 * to choose the specific field widths.
 *
 * @pre      The ostream has been opened.
 * @post     The contacts in the contact list are appended to the ostream 
 *           in sorted order, by last name, and then by first name in case 
 *           last names are identical. If there are two records with 
 *           identical primary and secondary key, the telephone number is 
 *           used as a tertiary key, and if need be the email address is 
 *           the quaternary key.
 * @param [inout] ostream output The stream for outputting the contact 
 *          list.
 * @returns  int The number of records written
 */
int ContactList::display(ostream& output)
{
    in_order(root, output);
    return insertSize;
}

/** in_order(link current, ostream& output)
 * Traverses binary search tree in alphabetical order by recursively
 * calling the left most node, displaying the contents of that node, 
 * and recursively calling the right node and displaying the contents
 * This will display the contents of the left child, its parent, and 
 * the contents of the right child
 *
 * @pre      link current exists and points to the root of the tree
 * @pre      The ostream has been opened.
 * @post     The contacts in the contact list are appended to the ostream 
 *           in sorted order, by last name, and then by first name in case 
 *           last names are identical. If there are two records with 
 *           identical primary and secondary key, the telephone number is 
 *           used as a tertiary key, and if need be the email address is 
 *           the quaternary key.
 * @param [in] link current pointer that points to a node in tree 
 * @param [inout] ostream output The stream for outputting the contact 
 *          list.
 * @returns none, void function
 */
void ContactList::in_order(link current, ostream& output)
{
    if(current != NULL)
    {
        in_order(current->left, output);
        display_tree(current->record, output);
        in_order(current->right, output);
    }
}

/** display_tree(Contact record, ostream& output)
 * displays the contents of record to given output stream
 *
 * @pre      Contact record holds a valid record to be displayed
 * @pre      The ostream has been opened
 * @post     The contacts in the given record are appended to the
 *           output stream
 * @param [in] Contact record contains valid records to be printed
 * @param [inout] ostream output The stream for outputting the contact 
 *          list.
 * @returns none, void function
 */
void ContactList::display_tree(Contact record, ostream& output)
{
    Name fullname_in_record;
    string lastnameInRecord, firstnameInRecord, telephone, email;
    record.get_name(fullname_in_record);
    lastnameInRecord = fullname_in_record.last();
    firstnameInRecord = fullname_in_record.first();
    record.get_tel(telephone);
    record.get_email(email);
    output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
}


/** insert(record_to_insert)
 * Inserts a given record into the contact list. If the record is an exact
 * duplicate of an existing record, it will not be added.
 * 
 * @pre     record_to_insert is a valid Contact. If there is not an exact 
 *          copy of record_to_insert already in contact list, then 
 *          record_to_insert is inserted into the list at an unspecified 
 *          position.
 * @returns int The total number of contacts successfully inserted into 
 *          the list.
 */
int ContactList::insert(Contact record_to_insert)
{
    // seed root of tree, record_to_insert and count into recursive function
    count=0;
    insertIntoBinaryTree(root, record_to_insert, count);
    insertSize += count;
    // if record inserted successfully return number of records inserted
    if (0!=count){
        return insertSize;
    }
    // if record not inserted return count which is zero
    else{
        return count;
    }
}

/** insertIntoBinaryTree(current, record_to_insert,  count)
 * Inserts a given record into the contact list. If the record is an exact
 * duplicate of an existing record, it will not be added.
 * 
 * @pre     current points to a contact in the tree starting from the root
 *          it may point to null means it reached the end of the tree
 * @pre     record_to_insert is a valid Contact. If there is not an exact 
 *          copy of record_to_insert already in contact list, then 
 *          record_to_insert is inserted into the list at an unspecified 
 *          position.
 * @returns void no return
 */
void ContactList::insertIntoBinaryTree(link & current, Contact record_to_insert, int& count)
{
    // If list is empty or if recursion reach the leaf of the tree
    if (NULL == current){
        // create a new node and set temp_ptr to point to it
        link temp_ptr = new tree_node;
        // check if new node creation is unsuccessful
        if ( NULL == temp_ptr ){
            count=0;
            return;
        }
        else{
            // if tree_node successfully created, set record_to_insert into the node
            temp_ptr->record = record_to_insert;
            temp_ptr->left = NULL;
            temp_ptr->right = NULL;
            // link the newly created node to the tree
            // recursive call, current represents current->left, or current->right this
            // will link the previous node to the newly created one
            current = temp_ptr;
            // update the size of the list
            count=1;
            return;
        }
    }

    // if the list is not empty
    // sort by comparing last name as primary key, comparing first name as secondary
    // key, comparing telephone number as tertiary key, and comparing email address
    // as quaternary key.
    // create an object of Name class of record that is being inserted
    Name fullname_to_insert;
    // set object to the fullname of record_to_insert
    record_to_insert.get_name(fullname_to_insert);
    // get the lastname out of fullname_to_insert
    string lastnameToInsert;
    lastnameToInsert = fullname_to_insert.last();

    // create an object of Name class of record that exists in the list
    Name fullname_in_record;
    // set object to the fullname of fullname_in_record
    current->record.get_name(fullname_in_record);
    // get the lastname out of fullname_in_record
    string lastnameInRecord;
    lastnameInRecord = fullname_in_record.last();
    // compare the last names
    // if lastname to insert is less than lastname in record
    if (lastnameToInsert < lastnameInRecord){
        // insert to the left side of the tree
        insertIntoBinaryTree(current->left, record_to_insert, count);
    }

    // if lastname to insert is greater than lastname in record
    else if (lastnameToInsert > lastnameInRecord){
        // insert to the right side of the tree
        insertIntoBinaryTree(current->right, record_to_insert, count);
    }

    // found the lastname
    // search by firstname
    else{
        // get the firstname that exists in record and 
        // firstname that we want to insert
        string firstnameInRecord, firstnameToInsert;
        firstnameInRecord = fullname_in_record.first();
        firstnameToInsert = fullname_to_insert.first();

        // if firstname to insert is less than firstname in record
        if (firstnameToInsert < firstnameInRecord) {
            // insert on the left hand side
            insertIntoBinaryTree(current->left, record_to_insert, count);
        }

        // if firstname to insert is greater than firstname in record
        else if (firstnameToInsert > firstnameInRecord){
            // insert on the right hand side
            insertIntoBinaryTree(current->right, record_to_insert, count);
        }

        // found the firstname
        // found the lastname
        // search by the telephone
        else{
            // get the telephone number that exists in record and 
            // telephone number that we want to insert
            string telephoneInRecord, telephoneToInsert;
            current->record.get_tel(telephoneInRecord);
            record_to_insert.get_tel(telephoneToInsert);

            // get the email address that exists in record and 
            // email address that we want to insert
            string emailInRecord, emailToInsert;
            current->record.get_email(emailInRecord);
            record_to_insert.get_email(emailToInsert);

            // if telephone number is provided
            if (!telephoneToInsert.empty()){
                // if telephone number to insert is less than 
                // telephone number in record
                if (telephoneToInsert < telephoneInRecord) {
                    // insert on the left hand side
                    insertIntoBinaryTree(current->left, record_to_insert, count);
                }

                // if telephone number to insert is greater than 
                // telephone number in record
                else if (telephoneToInsert > telephoneInRecord){
                    // insert on the right hand side
                    insertIntoBinaryTree(current->right, record_to_insert, count);
                }

                // found the telephone number
                // found the firstname
                // found the lastname
                // search the email address
                else{
                    // if email address is provided
                    if (!emailToInsert.empty()){
                        // if email address is less than the one in record, insert
                        // before the record
                        if ( emailToInsert < emailInRecord ) {
                            // insert on the left hand side
                            insertIntoBinaryTree(current->left, record_to_insert, count);
                        }
                        // if email address to insert is greater than 
                        // email address in record
                        else if (emailToInsert > emailInRecord){
                            // insert on the right hand side
                            insertIntoBinaryTree(current->right, record_to_insert, count);
                        }
                        // if the email address match, then the contact to insert
                        // matches the contact that exists in record, so break out
                        // of this recursive call, no need to insert a duplicate
                        else{
                            count=0;
                            return;
                        }
                    }
                    // email address is not provided
                    else{
                        // found a duplicate
                        // return from this call
                        count=0;
                        return;
                    }
                }
            }
            // telephone number is not provided
            // found the firstname
            // found the lastname
            else{
                // found duplicate
                count=0;
                return;
            }
        }
    }
}

/** insert( contact_list)
 * Inserts all contacts in contact_list into the current contact list.
 * If any of the contacts in contact_list are duplicates of an existing 
 * contact, they will not be inserted. The contacts are inserted at 
 * unspecified positions.
 * 
 * @pre      A contact list consisting of only valid Contacts. 
 * @post     The contact list contains all previously existing contacts 
 *           plus all contacts from contact_list that are not exact copies 
 *           of records already in the existing contact list.
 * @returns int The total number of contacts sucessfully inserted into the 
 *          list.
 */
int ContactList::insert(ContactList contact_list)
{
    Contact record;
    int count=0;
    count += contact_list.insert(record);
    return count;
}

/** remove(record_to_delete)
 * Removes all contacts which match the non-null fields of 
 * record_to_delete. 
 * Every contact in the contact list whose members match every non-null 
 * member of record_to_delete is removed from the list.
 * 
 * @pre     record_to_delete is a contact containing at least a non-null 
 *          last name and a non-null first name. 
 * @post    The contact list will contain no contacts which match the 
 *          non-null fields of record_to_delete.
 * @returns int The total number of contacts successfully removed from the 
 *          list.
 */
int ContactList::remove(Contact record_to_delete)
{
    count = 0;
    deleteNodeFromTree(root, record_to_delete);
    insertSize -= count;
    return count;
}

/** deleteNodeFromTree (link & current, Contact record_to_delete)
 * Removes all contacts which match the non-null fields of 
 * record_to_delete. 
 * Every contact in the contact list whose members match every non-null 
 * member of record_to_delete is removed from the list.
 * 
 * @pre     current points to a contact in tree
 * @pre     record_to_delete is a contact containing at least a non-null 
 *          last name and a non-null first name. 
 * @post    The contact list will contain no contacts which match the 
 *          non-null fields of record_to_delete.
 * @returns int The total number of contacts successfully removed from the 
 *          list.
 */
void ContactList::deleteNodeFromTree (link & current, Contact record_to_delete)
{
    if ( NULL == current ){
        return;  // the item was not found
    }

    // delete by comparing last name as primary key, comparing first name as secondary
    // key, comparing telephone number as tertiary key, and comparing email address
    // as quaternary key.
    // create an object of Name class of record that is being deleted
    Name fullname_to_delete;
    // set object to the fullname of record_to_delete
    record_to_delete.get_name(fullname_to_delete);
    // get the lastname out of fullname_to_delete
    string lastnameToDelete;
    lastnameToDelete = fullname_to_delete.last();

    // create an object of Name class of record that exists in the list
    Name fullname_in_record;
    // set object to the fullname of fullname_in_record
    current->record.get_name(fullname_in_record);
    // get the lastname out of fullname_in_record
    string lastnameInRecord;
    lastnameInRecord = fullname_in_record.last();

    // compare the lastnames
    // if lastname to delete is less than lastname in record
    if ( lastnameToDelete < lastnameInRecord ){
        // recursively delete on the left hand side
        deleteNodeFromTree( current->left, record_to_delete );
    }

    // if lastname to delete is greater than lastname in record
    else if ( lastnameToDelete > lastnameInRecord ){
        // recursively delete on the left hand side
        deleteNodeFromTree( current->right, record_to_delete );
    }

    // found the lastname
    // search firstname
    else {
        // get the firstname that exists in record and 
        // firstname that we want to delete
        string firstnameInRecord, firstnameToDelete;
        firstnameInRecord = fullname_in_record.first();
        firstnameToDelete = fullname_to_delete.first();

        // if firstname to delete is less than firstname in record
        if (firstnameToDelete < firstnameInRecord) {
            // recursively delete on the left hand side
            deleteNodeFromTree( current->left, record_to_delete );
        }

        // if firstname to delete is greater than firstname in record
        else if (firstnameToDelete > firstnameInRecord){
            // recursively delete on the right hand side
            deleteNodeFromTree( current->right, record_to_delete );
        }

        // found the firstname
        // found the lastname
        else{
            // get the telephone number that exists in record and 
            // telephone number that we want to delete
            string telephoneInRecord, telephoneToDelete;
            current->record.get_tel(telephoneInRecord);
            record_to_delete.get_tel(telephoneToDelete);

            // telephone number is provided
            if (!telephoneToDelete.empty()){
                // if telephone number to delete is less than 
                // telephone number in record
                if (telephoneToDelete < telephoneInRecord) {
                    // recursively delete on the left hand side
                    deleteNodeFromTree( current->left, record_to_delete );
                }

                // if telephone number to delete is greater than 
                // telephone number in record
                else if (telephoneToDelete > telephoneInRecord){
                    // recursively delete on the right hand side
                    deleteNodeFromTree( current->right, record_to_delete );
                }

                // found the telephone number
                // found the firstname
                // found the lastname
                // search by email address
                else{
                    // get the email address that exists in record and 
                    // email address that we want to deleted
                    string emailInRecord, emailToDelete;
                    current->record.get_email(emailInRecord);
                    record_to_delete.get_email(emailToDelete);

                    // email address is provided
                    if (!emailToDelete.empty()){
                        // if email address is less than the one in record, delete
                        // before the record
                        if ( emailToDelete < emailInRecord ) {
                            // recursively delete on the left hand side
                            deleteNodeFromTree(current->left, record_to_delete);
                        }

                        // if email address to delete is greater than 
                        // email address in record
                        else if (emailToDelete > emailInRecord){
                            // recursively delete on the right hand side
                            deleteNodeFromTree(current->right, record_to_delete);
                        }

                        // found the email address
                        // found the telephone number
                        // found the firstname
                        // found the lastname
                        else{
                            // item is equal to the item in the node; it is found
                            // Check how many children it has
                            // if it has two children
                            if ( current->left != NULL && current->right != NULL ) {
                                // It has two children. We need to replace it by the
                                // smallest item in the right subtree. Assume there
                                // is a function, find_min() that returns a pointer
                                // to the smallest item in a tree.
                                // get the pointer to the smallest item in right subtree
                                link temp_ptr = findMin( current->right );

                                // Copy the item into the current node
                                current->record = temp_ptr->record;

                                // Recursively call delete to delete the item that was just
                                // copied. It is in the right subtree.
                                deleteNodeFromTree( current->right, current->record );
                            }
                            
                            // if it has at most one child
                            else {
                                // The current node has at most one child. Copy the value of
                                // current temporarily
                                link old_node = current;

                                // If the left child is not empty, then make the left child the
                                // child of the parent of current. By assigning to current this
                                // achieves that.
                                // If the left child is empty, then either the right is empty or it is not
                                // In either case we can set current to point to its right child.
                                if ( current->left != NULL ){
                                    current = current->left;
                                }

                                else{
                                    current = current->right;
                                }
                                // Delete the node that current used to point to
                                delete old_node;
                                count++;
                            }
                        }
                    }

                    // email address is not provided
                    // found the telephone number
                    // found the firstname
                    // found the lastname
                    else{
                        // item is equal to the item in the node; it is found
                        // Check how many children it has
                        // if it has two children
                        if ( current->left != NULL && current->right != NULL ) {
                            // It has two children. We need to replace it by the
                            // smallest item in the right subtree. Assume there
                            // is a function, find_min() that returns a pointer
                            // to the smallest item in a tree.
                            // get the pointer to the smallest item in right subtree
                            link temp_ptr = findMin( current->right );

                            // Copy the item into the current node
                            current->record = temp_ptr->record;

                            // Recursively call delete to delete the item that was just
                            // copied. It is in the right subtree.
                            deleteNodeFromTree( current->right, current->record );
                        }
                        
                        // if it has at most one child
                        else {
                            // The current node has at most one child. Copy the value of
                            // current temporarily
                            link old_node = current;

                            // If the left child is not empty, then make the left child the
                            // child of the parent of current. By assigning to current this
                            // achieves that.
                            // If the left child is empty, then either the right is empty or it is not
                            // In either case we can set current to point to its right child.
                            if ( current->left != NULL ){
                                current = current->left;
                            }

                            else{
                                current = current->right;
                            }
                            // Delete the node that current used to point to
                            delete old_node;
                            count++;
                        }
                        // deleted just one node that matches the lastname, firstname
                        // and telephone number, call recursive function again to delete
                        // all nodes that match given lastname, firstname, and telephone
                        // number
                        deleteNodeFromTree( root, record_to_delete );
                    }
                }
            }

            // user left telephone number field blank
            else{
                // check if user provided email address to delete
                string emailInRecord, emailToDelete;
                current->record.get_email(emailInRecord);
                record_to_delete.get_email(emailToDelete);

                // if user provided email address to delete
                if (!emailToDelete.empty()){
                    // if email address is less than the one in record, delete
                    // before the record
                    if ( emailToDelete < emailInRecord ) {
                        // recursively delete on the left hand side
                        deleteNodeFromTree(current->left, record_to_delete);
                    }

                    // if email address to delete is greater than 
                    // email address in record
                    else if (emailToDelete > emailInRecord){
                        // recursively delete on the right hand side
                        deleteNodeFromTree(current->right, record_to_delete);
                    }

                    // found the email address
                    // found the firstname
                    // found the lastname
                    else{
                        // item is equal to the item in the node; it is found
                        // Check how many children it has
                        // if it has two children
                        if ( current->left != NULL && current->right != NULL ) {
                            // It has two children. We need to replace it by the
                            // smallest item in the right subtree. Assume there
                            // is a function, find_min() that returns a pointer
                            // to the smallest item in a tree.
                            // get the pointer to the smallest item in right subtree
                            link temp_ptr = findMin( current->right );

                            // Copy the item into the current node
                            current->record = temp_ptr->record;

                            // Recursively call delete to delete the item that was just
                            // copied. It is in the right subtree.
                            deleteNodeFromTree( current->right, current->record );
                        }
                        
                        // if it has at most one child
                        else {
                            // The current node has at most one child. Copy the value of
                            // current temporarily
                            link old_node = current;

                            // If the left child is not empty, then make the left child the
                            // child of the parent of current. By assigning to current this
                            // achieves that.
                            // If the left child is empty, then either the right is empty or it is not
                            // In either case we can set current to point to its right child.
                            if ( current->left != NULL ){
                                current = current->left;
                            }

                            else{
                                current = current->right;
                            }
                            // Delete the node that current used to point to
                            old_node->left = NULL;
                            old_node->right = NULL;
                            delete old_node;
                            count++;
                        }

                        // deleted just one node that matches the lastname, firstname
                        // and email address, call recursive function again to delete
                        // all nodes that match given lastname, firstname, and email
                        // address
                        deleteNodeFromTree( root, record_to_delete );

                    }
                }

                // user left email address field empty
                // delete all nodes that match lastname and firstname
                else{
                    // item is equal to the item in the node; it is found
                    // Check how many children it has
                    // if it has two children
                    if ( current->left != NULL && current->right != NULL ) {
                        // It has two children. We need to replace it by the
                        // smallest item in the right subtree. Assume there
                        // is a function, find_min() that returns a pointer
                        // to the smallest item in a tree.
                        // get the pointer to the smallest item in right subtree
                        link temp_ptr = findMin( current->right );

                        // Copy the item into the current node
                        current->record = temp_ptr->record;

                        // Recursively call delete to delete the item that was just
                        // copied. It is in the right subtree.
                        deleteNodeFromTree( current->right, current->record );
                    }
                    
                    // if it has at most one child
                    else {
                        // The current node has at most one child. Copy the value of
                        // current temporarily
                        link old_node = current;

                        // If the left child is not empty, then make the left child the
                        // child of the parent of current. By assigning to current this
                        // achieves that.
                        // If the left child is empty, then either the right is empty or it is not
                        // In either case we can set current to point to its right child.
                        if ( current->left != NULL ){
                            current = current->left;
                        }

                        else{
                            current = current->right;
                        }
                        // Delete the node that current used to point to
                        delete old_node;
                        count++;
                    }
                    // deleted just one node that matches the lastname and firstname
                    // call recursive function again to delete all nodes that match
                    // given lastname and firstname
                    deleteNodeFromTree( root, record_to_delete );
                }
            }
        }
    }
}

/** findMin( current )
 * Returns the smallest contact in the tree, recursively calls on the
 * leftmost node in the tree
 * 
 * @pre     current points to a contact in tree
 * @post    Sets current to point to the smallest node in the tree
 * @returns link a pointer to the smallest node in the tree
 */
link ContactList::findMin( link current )
{
    // reaached the end of the tree
    if ( NULL == current ){
        return NULL;
    }
    // reached the smallest node in the tree
    if ( current->left == NULL ){
        return current;
    }

    return findMin( current->left );
}

/** size()
 * Returns the total number of contacts in the contact list.
 *
 * @pre         None.
 * @post        None.
 * @returns int The total number of contacts in the contact list.
 */
int ContactList::size()
{
    return insertSize;
}

/** save()
 * This saves a copy of the current contactlist by writing it to a file 
 * named contactlist.bkp in the current working directory, overwriting any 
 * such file if it already exists. It must have write permission in the 
 * working directory.
 *
 * @pre     None. 
 * @post    The contactlist.bkp file in the current working directory 
 *          contains the contents of the current copy of the in-memory 
 *          contact list. If the file existed before, it is replaced. 
 * @returns int The number of contacts written to the file, or -1 if the 
 *          write was not allowed.
 */
int ContactList::save()
{
    // open the outout file
    ofstream outFile;
    outFile.open("contactlist.bkp");
    if (outFile.fail()){
        cerr << "Could not open file to save" << endl;
        exit(1); // 1 indicates an error occurred
    }
    int count;
    // save nodes in alphabetical order
    // call in-order function to accomplish this
    saveUsingInOrderTraversal(outFile, root, count);
    return count;
}

/** saveUsingInOrderTraversal( outFile, current, count)
 * Traverses the tree using in-order algorithm
 *
 * @pre     ofstream has the means to create a file
 * @pre     link current points to a contact in the tree
 * @pre     count is initialized 
 * @post    The contactlist.bkp file in the current working directory 
 *          contains the contents of the current copy of the in-memory 
 *          contact list. If the file existed before, it is replaced. 
 * @returns void no return
 */
void ContactList::saveUsingInOrderTraversal(ofstream& outFile, link current, int& count)
{
    // in-order traversal
    if (NULL != current)
    {
        saveUsingInOrderTraversal(outFile, current->left, count);
        saveCurrentNode(outFile, current->record, count);
        saveUsingInOrderTraversal(outFile, current->right, count);
    }
}

/** saveCurrentNode( outFile, record_to_save, count )
 * This saves a copy of the current contactlist by writing it to a file 
 * named contactlist.bkp in the current working directory, overwriting any 
 * such file if it already exists. It must have write permission in the 
 * working directory.
 *
 * @pre     outfile is opened for writing
 * @pre     record_to_save is a valid record in sorted order 
 * @post    Writes contents of record_to_save to outfile stream
 * @returns void no return
 */
void ContactList::saveCurrentNode(ofstream& outFile, Contact record_to_save, int& count)
{
    Name fullname_in_record;
    string lastnameInRecord, firstnameInRecord, telephone, email;
    // collect the first and last name of the record
    record_to_save.get_name(fullname_in_record);
    lastnameInRecord = fullname_in_record.last();
    firstnameInRecord = fullname_in_record.first();

    // collect the telephone number and email address
    record_to_save.get_tel(telephone);
    record_to_save.get_email(email);

    // output the record into output file one record per line
    outFile << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
    count++;
}


/** find(output, lastname, firstname)
 * Writes onto the output stream all contacts whose last and first names 
 * match the given names.
 *
 * @pre      lastname is a valid name and firstname is a valid name and 
 *           output is an open iostream.
 * @post     Any contacts whose first and last names match the given first 
 *           and last names are appended to the ostream.
 * @returns int The number of records written to the stream.
 */
int ContactList::find(ostream& output, string lname, string fname)
{
    count = 0;
    findLastnameFirstnameInTree(output, root, lname, fname);
    return count;
}

/** findLastnameFirstnameInTree( output, current, lname, fname )
 * Writes onto the output stream all contacts whose last and first names 
 * match the given names.
 *
 * @pre      lastname is a valid name and firstname is a valid name and 
 *           output is an open iostream.
 * @post     Any contacts whose first and last names match the given first 
 *           and last names are appended to the ostream.
 * @returns void no return
 */
void ContactList::findLastnameFirstnameInTree(ostream& output, link current, string lname, string fname)
{
    // current is null and/or
    // recursive call reached the leaf of the tree
    if (NULL == current){
        // end of the tree, contact is not found
        return;
    }
    // begin search
    else{
        // get contact from the record in the list
        Name fullnameInRecord, fullname;
        current->record.get_name(fullnameInRecord);
        string lastnameInRecord, firstnameInRecord;
        lastnameInRecord = fullnameInRecord.last();
        firstnameInRecord = fullnameInRecord.first();

        // compare the last name in record with the last name to find
        if (lname < lastnameInRecord){
            // last name to find is less so recursively search on the left
            findLastnameFirstnameInTree(output, current->left, lname, fname);
        }
        // compare the last name in record with the last name to find
        else if (lname > lastnameInRecord){
            // last name to find is greater so recursively search on the right
            findLastnameFirstnameInTree(output, current->right, lname, fname);
        }
        // found the lastname
        else {
            // compare the first name in record with the first name to find
            if (fname < firstnameInRecord){
                // last name to find is less so recursively search on the left
                findLastnameFirstnameInTree(output, current->left, lname, fname);
            }
            // compare the first name in record with the first name to find
            else if (fname > firstnameInRecord){
                // first name to find is greater so recursively search on the right
                findLastnameFirstnameInTree(output, current->right, lname, fname);
            }
            // found the lastname
            // found the firstname
            else {
                // output the items
                string telephone;
                current->record.get_tel(telephone);
                string email;
                current->record.get_email(email);
                output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
                count++;
            }
        }
    }
}

/** find(output, record_to_find)
 * Calls findNodeInTree function to write onto the output stream all 
 * contacts that match the non-null fields of record_to_find. 
 *
 * @pre     record_to_find is a valid contact and output is an open 
 *          ostream.
 * @post    Any contacts whose non-null members match the non-null members 
 *          of the record_to_find are appended to the ostream, sorted by 
 *          last name, and then first name in case of ties.
 * @returns  int The number of records written to the stream.
 */
int ContactList::find(ostream& output, Contact record_to_find)
{
    count = 0;
    findNodeInTree(output, root, record_to_find);
    return count;
}

/** findNodeInTree( output, current, record_to_find )
 * Writes onto the output stream all contacts that match the first and 
 * last name fields of record_to_find, if lastname field is null, calls
 * findNodeUsingInOrderTraversal function
 *
 * @pre     record_to_find is a valid contact, current points to a contact
 *          in the tree, and output is an open ostream.
 * @post    Any contacts whose non-null members match the non-null members 
 *          of the record_to_find are appended to the ostream, sorted by 
 *          last name, and then first name in case of ties.
 * @returns  void no return
 */
void ContactList::findNodeInTree(ostream& output, link current, Contact record_to_find)
{
    // current is null and/or
    // recursive call reached the leaf of the tree
    if (NULL == current){
        // end of the tree, contact is not found
        return;
    }
    // begin search
    else{
        // get contact from the record in the list
        Name fullnameInRecord, fullname;
        current->record.get_name(fullnameInRecord);
        string lastnameInRecord, firstnameInRecord;
        lastnameInRecord = fullnameInRecord.last();
        firstnameInRecord = fullnameInRecord.first();

        // get the contacts to be found
        record_to_find.get_name(fullname);
        string firstnameToFind, lastnameToFind, telephoneToFind, emailToFind;
        firstnameToFind = fullname.first();
        lastnameToFind = fullname.last();
        record_to_find.get_tel(telephoneToFind);
        record_to_find.get_email(emailToFind);

        // user can leave any field blank, program must account for 
        // all possible combinations

        // lastname is provided
        // firstname is provided
        if (!lastnameToFind.empty() && !firstnameToFind.empty()){
            // compare the last name in record with the last name to find
            if (lastnameToFind < lastnameInRecord){
                // last name to find is less so recursively search on the left
                findNodeInTree(output, current->left, record_to_find);
            }
            // compare the last name in record with the last name to find
            else if (lastnameToFind > lastnameInRecord){
                // last name to find is greater so recursively search on the right
                findNodeInTree(output, current->right, record_to_find);
            }
            // found the lastname
            else {
                // compare the first name in record with the first name to find
                if (firstnameToFind < firstnameInRecord){
                    // last name to find is less so recursively search on the left
                    findNodeInTree(output, current->left, record_to_find);
                }
                // compare the first name in record with the first name to find
                else if (firstnameToFind > firstnameInRecord){
                    // first name to find is greater so recursively search on the right
                    findNodeInTree(output, current->right, record_to_find);
                }
                // found the lastname
                // found the firstname
                else {
                    // telephone number is provided
                    if (!telephoneToFind.empty()){
                        string telephoneInRecord;
                        current->record.get_tel(telephoneInRecord);

                        // if the telephone number is less than the one in record
                        if (telephoneToFind < telephoneInRecord){
                            // recursively search on the left
                            findNodeInTree(output, current->left, record_to_find);
                        }

                        // if the telephone number is larger than the one in the record
                        else if (telephoneToFind > telephoneInRecord){
                            // recursively search on the right
                            findNodeInTree(output, current->right, record_to_find);
                        }

                        // found the lastname
                        // found the firstname
                        // found the telephone number
                        // if the email address is provided we need to find it too
                        else if (!emailToFind.empty()){
                            // get the email address from record
                            string emailInRecord;
                            current->record.get_email(emailInRecord);

                            // if the email address is less than the record
                            if (emailToFind < emailInRecord){
                                // recursively search on the left
                                findNodeInTree(output, current->left, record_to_find);
                            }

                            // if the email address is larger than the record
                            else if (emailToFind > emailInRecord){
                                // recursively search on the right
                                findNodeInTree(output, current->right, record_to_find);
                            }

                            // found the lastname
                            // found the fistname
                            // found the telephone number
                            // found the email address
                            else{
                                // output the items
                                string telephone;
                                current->record.get_tel(telephone);
                                string email;
                                current->record.get_email(email);
                                output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
                                count++;
                                
                                // there might be more nodes with the same lastname,
                                // firstname, telephone number and email address
                                // recursively call on the left and right tree to see
                                // if they match as well
                                findNodeInTree(output, current->left, record_to_find);
                                findNodeInTree(output, current->right, record_to_find);
                            }
                        }
                        // email address is not provided
                        // found the lastname
                        // found the fistname
                        // found the telephone number
                        else{
                            // output the items
                            string telephone;
                            current->record.get_tel(telephone);
                            string email;
                            current->record.get_email(email);
                            output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
                            count++;
                            
                            // there might be more nodes with the same lastname,
                            // firstname and telephone number
                            // recursively call on the left and right tree to see
                            // if they match as well
                            findNodeInTree(output, current->left, record_to_find);
                            findNodeInTree(output, current->right, record_to_find);
                        }
                    }
                    // telephone number is not provided
                    else {
                        // email address is provided
                        if (!emailToFind.empty()){
                            // get the email address from record
                            string emailInRecord;
                            current->record.get_email(emailInRecord);

                            // if the email address is less than the record
                            if (emailToFind < emailInRecord){
                                // recursively search on the left
                                findNodeInTree(output, current->left, record_to_find);
                            }
                            // if the email address is larger than the record
                            else if (emailToFind > emailInRecord){
                                // recursively search on the right
                                findNodeInTree(output, current->right, record_to_find);
                            }
                            // found the email address
                            // found the lastname
                            // found the firstname
                            else{
                                // output this record
                                string telephone;
                                current->record.get_tel(telephone);
                                string email;
                                current->record.get_email(email);
                                output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
                                count++;

                                // there might be more nodes with matching record
                                // recursively call on the left and right tree
                                findNodeInTree(output, current->left, record_to_find);
                                findNodeInTree(output, current->right, record_to_find);
                            }
                        }
                        // email address is not provided
                        // telephone number is not provided
                        // found the lastname
                        // found the firstname
                        else{
                            // output this record
                            string telephone;
                            current->record.get_tel(telephone);
                            string email;
                            current->record.get_email(email);
                            output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
                            count++;

                            // there might be more nodes with the same lastname
                            // and firstname
                            // recursively call on the left and right tree to see
                            // if the last names match as well
                            findNodeInTree(output, current->left, record_to_find);
                            findNodeInTree(output, current->right, record_to_find);
                        }
                    }
                }
            }
        }

        // lastname is provided, 
        // firstname is not provided
        if (!lastnameToFind.empty() && firstnameToFind.empty()){
            // compare the last name in record with the last name to find
            if (lastnameToFind < lastnameInRecord){
                // last name to find is less so recursively search on the left
                findNodeInTree(output, current->left, record_to_find);
            }
            // compare the last name in record with the last name to find
            else if (lastnameToFind > lastnameInRecord){
                // last name to find is greater so recursively search on the right
                findNodeInTree(output, current->right, record_to_find);
            }
            // found the lastname
            else {
                // if the telephone number also needs to be found
                if (!telephoneToFind.empty()){
                    // get the telephone number from the record
                    string telephoneInRecord;
                    current->record.get_tel(telephoneInRecord);

                    // if the telephone number is less than the one in record
                    if (telephoneToFind < telephoneInRecord){
                        // recursively search on the left
                        findNodeInTree(output, current->left, record_to_find);
                    }

                    // if the telephone number is larger than the one in the record
                    else if (telephoneToFind > telephoneInRecord){
                        // recursively search on the right
                        findNodeInTree(output, current->right, record_to_find);
                    }

                    // found the lastname
                    // found the telephone number but
                    // if the email address is provided we need to find it too
                    else if (!emailToFind.empty()){
                        // get the email address from record
                        string emailInRecord;
                        current->record.get_email(emailInRecord);

                        // if the email address is less than the record
                        if (emailToFind < emailInRecord){
                            // recursively search on the left
                            findNodeInTree(output, current->left, record_to_find);
                        }

                        // if the email address is larger than the record
                        else if (emailToFind > emailInRecord){
                            // recursively search on the right
                            findNodeInTree(output, current->right, record_to_find);
                        }

                        // found the lastname
                        // found the telephone number
                        // found the email address
                        else{
                            // output the items
                            string telephone;
                            current->record.get_tel(telephone);
                            string email;
                            current->record.get_email(email);
                            output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
                            count++;
                            
                            // there might be more nodes with the same last name
                            // telephone number and email address
                            // recursively call on the left and right tree to see
                            // if they match as well
                            findNodeInTree(output, current->left, record_to_find);
                            findNodeInTree(output, current->right, record_to_find);
                        }
                    }

                    // email address is not provided
                    // found the lastname
                    // found the telephone number
                    else{
                        // output the record
                        string telephone;
                        current->record.get_tel(telephone);
                        string email;
                        current->record.get_email(email);
                        output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
                        count++;
                        
                        // there might be more nodes with the same last name
                        // and telephone number
                        // recursively call on the left and right tree to see
                        // if they match as well
                        findNodeInTree(output, current->left, record_to_find);
                        findNodeInTree(output, current->right, record_to_find);
                    }
                }

                // telephone number is not provided
                else {
                    // if email address is provided
                    if (!emailToFind.empty()){
                        // get the email address
                        string emailInRecord;
                        current->record.get_email(emailInRecord);

                        // if email is less than the email in record
                        if (emailToFind < emailInRecord){
                            // recursively search on the left
                            findNodeInTree(output, current->left, record_to_find);
                        }

                        // if the email address is larger than the record
                        else if (emailToFind > emailInRecord){
                            // recursively search on the right
                            findNodeInTree(output, current->right, record_to_find);
                        }

                        // found the lastname
                        // found the email address
                        else{
                            // output the items
                            string telephone;
                            current->record.get_tel(telephone);
                            string email;
                            current->record.get_email(email);
                            output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
                            count++;

                            // there might be more nodes with the same last name
                            // and email address
                            // recursively call on the left and right tree to see
                            // if the last names and email address match as well
                            findNodeInTree(output, current->left, record_to_find);
                            findNodeInTree(output, current->right, record_to_find);
                        }
                    }
                    // email address is not provided
                    // found the lastname
                    else{
                        // output the record
                        string telephone;
                        current->record.get_tel(telephone);
                        string email;
                        current->record.get_email(email);
                        output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephone << "\t" << email << endl;
                        count++;

                        // there might be more nodes with the same last name
                        // recursively call on the left and right tree to see
                        // if the last names match as well
                        findNodeInTree(output, current->left, record_to_find);
                        findNodeInTree(output, current->right, record_to_find);
                    } 
                } // if the email address also needs to be found
            } // end if last name to find matches with last name in record
        } // end if last name is provided, but not the first name

        // lastname is not provided
        // do an in-order traversal
        // binary search tree is sorted by lastname as primary key 
        // must visit every single node to search
        else if (lastnameToFind.empty()){
            findNodeUsingInOrderTraversal(output, root, record_to_find);    
        }//end if lastname not provided
    } // end else
}

/** findNodeUsingInOrderTraversal( output, current, record_to_find )
 * Traverses tree using in-order algorithm, calls displayFoundNode to
 * output contacts that match record_to_find
 *
 * @pre     record_to_find is a valid contact, current points to a contact
 *          in the tree, and output is an open ostream.
 * @post    Any contacts whose non-null members match the non-null members 
 *          of the record_to_find are appended to the ostream, sorted by 
 *          last name, and then first name in case of ties.
 * @returns  void no return
 */
void ContactList::findNodeUsingInOrderTraversal(ostream& output, link current, Contact record_to_find)
{
    // in-order traversal
    if (NULL != current)
    {
        findNodeUsingInOrderTraversal(output, current->left, record_to_find);
        displayFoundNode(current->record, output, record_to_find);
        findNodeUsingInOrderTraversal(output, current->right, record_to_find);
    }
}

/** displayFoundNode( record, output, record_to_find )
 * Writes onto the output stream all contacts that match the first and 
 * last name fields of record_to_find
 *
 * @pre     record_to_find and record are valid contacts, current points 
 *          to a contact in the tree, and output is an open ostream.
 * @post    Any contacts whose non-null members match the non-null members 
 *          of the record_to_find are appended to the ostream, sorted by 
 *          last name, and then first name in case of ties.
 * @returns  void no return
 */
void ContactList::displayFoundNode(Contact record, ostream& output, Contact record_to_find)
{
    // get records of contact that exists in the record
    Name fullname_in_record;
    string lastnameInRecord, firstnameInRecord, telephoneInRecord, emailInRecord;
    record.get_name(fullname_in_record);
    lastnameInRecord = fullname_in_record.last();
    firstnameInRecord = fullname_in_record.first();
    record.get_tel(telephoneInRecord);
    record.get_email(emailInRecord);

    // get the records of contact that needs to be found
    Name fullname_to_find;
    record_to_find.get_name(fullname_to_find);
    string firstnameToFind, lastnameToFind, telephoneToFind, emailToFind;
    firstnameToFind = fullname_to_find.first();
    lastnameToFind = fullname_to_find.last();
    record_to_find.get_tel(telephoneToFind);
    record_to_find.get_email(emailToFind);

    // if firstname is not provided
    if (firstnameToFind.empty()){
        // if telephone number is not provided
        if (telephoneToFind.empty()){
            // if email address is not provided
            if (emailToFind.empty()){
                // all fields are null
                return;
            }
            // email address is provided
            else{
                // only the email address is provided
                // if email address matches current record
                if (emailToFind == emailInRecord){
                    // found a match!
                    // email address matches current record
                    // output the record to the output stream
                    output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephoneInRecord << "\t" << emailInRecord << endl;
                    count++;
                }
                // this record does not match the email address
                // return from this call
                return;
            }
        }
        // telephone number is provided
        else{
            // if email address is not provided
            if (emailToFind.empty()){
                // firstname is not provided
                // lastname is not provided
                // email address is not provided
                // telephone number is provided
                // if telephone number matches this record
                if (telephoneToFind == telephoneInRecord){
                    // found a match!
                    // telephone number matches current record
                    // output the record to the output stream
                    output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephoneInRecord << "\t" << emailInRecord << endl;
                    count++;
                }
                // telephone number does not match
                // return from this call
                return;
            }
            // if email address is provided
            else{
                // firstname is not provided
                // lastname is not provided
                // email address is provided
                // telephone number is provided
                // if email and telephone match this record
                if (emailToFind == emailInRecord && telephoneToFind == telephoneInRecord){
                    // email address and telephone number match!
                    // output this record to stream
                    output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephoneInRecord << "\t" << emailInRecord << endl;
                    count++;
                }
                // either email address or telephone number does not match
                // this record is not meant to be
                return;
            }
        }
    }
    // firstname is provided
    else{
        // if telephone is not provided
        if (telephoneToFind.empty()){
            // if email is not provided
            if (emailToFind.empty()){
                // lastname is empty
                // telephone number is empty
                // email address is empty
                // if firstname matches
                if (firstnameToFind == firstnameInRecord){
                    // found a match!
                    // firstname to find matches the record
                    // output record to stream
                    output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephoneInRecord << "\t" << emailInRecord << endl;
                    count++;
                }
                // firstname does not match
                // this record is not meant to be
                return;
            }
            // email address is provided
            // firstname is provided
            // telephone is not provided
            else{
                // if email address match
                // and if firstname match
                if (firstnameToFind == firstnameInRecord && emailToFind == emailInRecord){
                    // found a match!
                    // output to stream
                    output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephoneInRecord << "\t" << emailInRecord << endl;
                    count++;
                }
                // either email address or firstname does not match
                // this record is not meant to be
                return;
            }
        }
        // telephone number is provided
        // firstname is provided
        else{
            // if email is not provided
            if (emailToFind.empty()){
                // if firstname match and
                // if telephone match
                if (firstnameToFind == firstnameInRecord && telephoneToFind == telephoneInRecord){
                    // found a match!
                    // output to stream
                    output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephoneInRecord << "\t" << emailInRecord << endl;
                    count++;
                }
                // either firstname or telephone number does not match
                return;
            }
            // email address is provided
            // telephone number is provided
            // firstname is provided
            else{
                // if firstname and telephone number and email address match to record
                if (firstnameToFind == firstnameInRecord && telephoneToFind == telephoneInRecord && emailToFind == emailInRecord){
                    // found a match!
                    // output to stream
                    output << lastnameInRecord << "\t" << firstnameInRecord << "\t" << telephoneInRecord << "\t" << emailInRecord << endl;
                    count++;
                }
                // either firstname or telephone or email doesn't match
                // return from this call
                return;
            }
        }
    }
}

/** make_empty()
 * Deletes all of the contacts in the contact list. Calls destroy() to 
 * delete nodes in tree
 * 
 * @pre     None. 
 * @post    The contact list becomes an empty list.
 * @returns int The number of records deleted.
 */
int ContactList::make_empty()
{
    count=0;
    destroy(root);
    root = NULL;
    insertSize = 0;
    return count;
}

/** destroy( tree_ptr )
 * Deletes all of the contacts in the contact list.
 * 
 * @pre     link tree_ptr points to root of the tree 
 * @post    The contact list becomes an empty list.
 * @returns void no return
 */
void ContactList::destroy( link tree_ptr )
{
    // postorder traversal
    if (tree_ptr != NULL){
        destroy (tree_ptr->left);
        destroy (tree_ptr->right);
        delete tree_ptr;
        count++;
        tree_ptr = NULL;
    }
}

