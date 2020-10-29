// -*- Mode: C++ -*-

//         command.cpp
//
// Copyright (c) 1996, Regents of the University of California
// $Header: /usr/local/devel/GiST/libGiST/RSTree/command.cpp,v 1.1.1.1 1996/08/06 23:47:23 jmh Exp $

#include <stdio.h>
#include <fstream.h>
#ifdef UNIX
#include <unistd.h>
#include <ctype.h>
#endif

#include "RT.h"
#include "command.h"

struct Table
{
    char *name;
    GiST *gist;
};

#define MAX_TABLES 10
#define NOT_FOUND (-1)

storage db("testrstree.odb");

REGISTER(Record);

Table tables[MAX_TABLES];
int numTables = 0;
int debug = 0;

int 
GetTable(const char *name)
{
    int i;

    for (i=0; i<numTables; i++)
	if (!strcmp(tables[i].name, name))
	    break;
    return i == numTables ? NOT_FOUND : i;
}

void 
CommandCreate(const char *method,
	      const char *table)
{
    if (numTables == MAX_TABLES) {
	cout << "This progam can only handle "<<MAX_TABLES<<" open tables.\n";
	return;
    }

    if (GetTable(table) != NOT_FOUND) {
	cerr << "Table already open!\n";
	return;
    }

    if (strcmp(method, "rtree")) {
	cerr << "The only supported method is rtree.\n";
	return;
    }

    GiST *gist = new RT(db);
    GiST_POST_Root* root = (GiST_POST_Root*)db.get_root_object();
    if (root == NULL) { 
	root = new_in(db, GiST_POST_Root)(db);
	db.set_root_object(root);
    }

    gist->Create(table);
    if (!gist->IsOpen()) {
	cout << "Error opening table.\n";
	delete gist;
	return;
    }

    cout << "Table " << table << " created as type " << method << ".\n";

    tables[numTables].name = strdup(table);
    tables[numTables].gist = gist;
    numTables++;
}

void 
CommandDrop(const char *table)
{
    if (GetTable(table) != NOT_FOUND) {
	cerr << "Cannot drop an open table.\n";
	return;
    }
    cout << "Table " << table << " dropped.\n";
}

void 
CommandOpen(const char *method, const char *table)
{
    GiST *gist;
    if (numTables == MAX_TABLES) {
	cout << "This progam can only handle "<<MAX_TABLES<<" open tables.\n";
	return;
    }

    if (GetTable(table) != NOT_FOUND) {
	cout << "Table already open!\n";
	return;
    }

    if (!strcmp(method, "rtree"))
      gist = new RT(db);
    else {
      cerr << "The only supported method is rtree.\n";
      return;
    }

    gist->Open(table);

    if (!gist->IsOpen()) {
	delete gist;
	cout << "Error opening table.\n";
	return;
    }

    cout << "Table " << table << " opened.\n";

    tables[numTables].gist = gist;
    tables[numTables].name = strdup(table);
    numTables++;
}

void 
CommandClose(const char *table)
{
    int i;

    i = GetTable(table);
    if (i == NOT_FOUND) {
	cerr << "Table is not open.\n";
	return;
    }

    cout << "Table " << table << " closed.\n";
    delete tables[i].gist;

    for ( ; i < numTables-1; i++)
	tables[i] = tables[i+1];
    numTables--;
}

void 
CommandSelect(const char *table,
		   const GiSTpredicate& pred)
{
    int i;

    i = GetTable(table);
    if (i == NOT_FOUND) {
	cerr << "Table is not open!\n";
	return;
    }

    GiST *gist = tables[i].gist;

    GiSTcursor *c = gist->Search(pred);
    GiSTentry *e;
    while ((e = c->Next()) != NULL) {
	Record* rec = (Record*)e->Ptr();
	cout << rec->value << "\n";
	delete e;
    }
    delete c;
}

void 
CommandDelete(const char *table,
	      const GiSTpredicate& pred)
{
    int i;

    if ((i = GetTable(table)) == NOT_FOUND) {
	cerr << "Table not open!\n";
	return;
    }

    if (i != NOT_FOUND)
	tables[i].gist->Delete(pred);
}

void 
CommandInsert(const char *table,
	      const RTkey& key,
	      int value)
{
    int i;

    if ((i = GetTable(table)) == NOT_FOUND) {
	cerr << "Table not open!\n";
	return;
    }

    Record* rec = new_in(db, Record);
    rec->value = value;
    GiST *gist = tables[i].gist;
    gist->Insert(RTentry(key, (GiSTpage)rec));

    cout << "(" << key << ", " << value << ") inserted into " << table << ".\n";
}

void 
CommandQuit()
{
    for (int i=0; i<numTables; i++)
	delete tables[i].gist;
    cout << "Goodbye.\n";
    db.close();
    exit(0);
}

void 
CommandPrompt()
{
    cout << "RSTree> ";
	cout.flush();
}

void 
CommandDebug()
{
    debug = !debug;
    cout << "Debugging Output ";
    cout << (debug ? "ON\n" : "OFF\n");
}

void 
CommandHelp()
{
    ifstream is("RTree.help");
    char c;

    while (is.get(c)) cout << c;
}

void 
CommandDump(const char *table, GiSTpage page)
{
    int i;

    if ((i = GetTable(table)) == NOT_FOUND) {
	cout << "No such table is open.\n";
	return;
    }

    GiSTpath path;
    path.MakeRoot();
#ifdef PRINTING_OBJECTS
    tables[i].gist->DumpNode(cout, path);
#endif
}

int yyparse();

int 
main()
{
    cout << "**  RSTree: An R*-Tree based on Generalized Search Trees\n";
    cout << "Type \"help\" for a list of commands.\n\n";
    if (db.open()) { 
	CommandPrompt();
	yyparse();
	return 0;
    } else { 
	return 1;
    }
}
