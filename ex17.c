#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// define constants through the pre-processors
#define MAX_DATA 512
#define MAX_ROWS 100

// the name is fixed size making it less efficient but easier to store and read
struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    struct Address rows[MAX_ROWS];
};

struct Connection {
    FILE *file;
    struct Database *db;
};

// kills the program with an error if anything is wrong
// when you have an error return from a function, it will usually set an "external" variable
// called errno to say exactly what error happened. These are just numbers so you can use perror to print the
// message.
void die(const char *message) {
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

void Address_print(struct Address *addr) {
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}
// rewind gets you back to the beginning of a file, you need to make sure to free errno
void Database_load(struct Connection *conn) {
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1) die("Failed to load db.");
}

struct Connection *Database_open(const char *filename, char mode) {
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn) die("Memory error");

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db) die("Memory error");

    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if (conn->file) {
            Database_load(conn);
        }
    }

    if (!conn->file) die("Failed to open the file");

    return conn;
}

void Database_close(struct Connection *conn) {
    if (conn) {
        if (conn->file) fclose(conn->file);
        if (conn->db) free(conn->db);
        free(conn);
    }
}

void Database_write(struct Connection *conn) {
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1) die("Failed to write database.");

    rc = fflush(conn->file);
    if (rc == -1) die("Cannot flush database.");
}

void Database_create(struct Connection *conn) {
    for (int i = 0; i < MAX_ROWS; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0};
        // then assign it
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
    struct Address *addr = &conn->db->rows[id];
    if (addr->set) die("Already set, delete it first");

    addr->set = 1;
    char *res = strncpy(addr->name, name, MAX_DATA);
    addr->name[MAX_DATA-1] = '\0';
    if (!res) die("Name copy failed");

    res = strncpy(addr->email, email, MAX_DATA);
    addr->email[MAX_DATA-1] = '\0';
    if (!res) die("Email copy failed");
}

void Database_get(struct Connection *conn, int id) {
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set");
    }
}

// Copying struct prototypes -- initialize a struct on the stack and then copy into another struct in order to
// delete the fields of a struct. Modern C allows you to simply assign one struct to another and it'll handle
// the copying for you.
void Database_delete(struct Connection *conn, int id) {
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
    struct Database *db = conn->db;

    for (int i = 0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];

    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    // converting string to an int
    if (argc > 3) id = atoi(argv[3]);
    if (id >= MAX_ROWS) die("There's not that many records.");

    switch (action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;
        case 'g':
            if (argc != 4) die("Need an id to get");
            Database_get(conn, id);
            break;
        case 's':
            if (argc != 6) die("Need id, name, email to set");
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;
        case 'd':
            if (argc != 4) die("Need id to delete");
            Database_delete(conn, id);
            Database_write(conn);
            break;
        case 'l':
            Database_list(conn);
            break;
        default:
            die("invalid action, only: c=create, g=get, s=set, d=del, l=list");
    }

    Database_close(conn);

    return 0;
}

//  Heap is all the remaining memory in your computer, and you access it with the function malloc to get more.
//  Each time you call malloc, the OS uses internal functions to register that piece of memory to you, and
//  then returns a pointer to it. You use free to return it to the OS so that it can be used by other
//  programs. Otherwise you'll leak memory, but Valgrind helps you track these down.
//
//  The Stack is a special region of memory that stores temporary variables each function creates as locals to
//  that function. Each argument to a function is "pushed" onto the stack, and then used inside the function.
//  It is really a stack data strcuture, so the last thing in is the first thing out. Advantage of using a
//  stack is that when the function exists, C compilier "pops" these variables off the stack to clean up.
//  Three primary problems to worry about:
//      1.) If you get a block of memory from malloc, and have the pointer on the stack, then when the
//      function exits, the pointer will get popped off and lost.
//      2.) If you put too much data on the stack (large structs and arrays) then you can cause a "stack
//      overflow" and the program will abort
//      3.) If you take a pointer to something on the stack, and then pass that or return it from your
//      function, the function receiving it will segfault because the actual data will get popped off and
//      disappear.
//  This is why we have a database open and close, so we can open with creating everything and then freeing
//  everything when we delete it.
