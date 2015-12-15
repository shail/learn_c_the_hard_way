#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// the name is fixed size making it less efficient but easier to store and read
// This defines global constants that the preprocessor creates

// Address struct then uses these constants to create a piece of data that is fixed in size making it less
// efficient, but easier to store and read.
struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    int max_data;
    int max_rows;
    struct Address *rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

// Function kills the program with an error if there's anything wrong
// When you have an error return from a function, it will usually set an "external" variable called errno to
// say exactly what error happened. These are just numbers, so you can use perror to print the error message.
//
void Database_close(struct Connection *conn) {
    if (conn) {
        if (conn->file) fclose(conn->file);
        if (conn->db) free(conn->db);
        free(conn);
    }
}

void die(const char *message, struct Connection *conn) {
    if (conn) {
      Database_close(conn);
    }

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
    struct Database *db = conn->db;
    struct Address *addr;
    db->rows = malloc(db->max_rows * sizeof(struct Address));
    for (addr = db->rows; addr < db->rows + db->max_rows; ++addr) {
      addr->name = malloc(db->max_data);
      addr->email = malloc(db->max_data);
    }
}

struct Connection *Database_open(const char *filename, char mode) {
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn) die("Memory error", NULL);

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db) die("Memory error", NULL);

    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if (conn->file) {
            Database_load(conn);
        }
    }

    if (!conn->file) die("Failed to open the file", NULL);

    return conn;
}

void Database_write(struct Connection *conn) {
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1) die("Failed to write database.", NULL);

    rc = fflush(conn->file);
    if (rc == -1) die("Cannot flush database.", NULL);
}

void Database_create(struct Connection *conn, int max_data, int max_rows) {
    struct Address *addr;
    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;
    conn->db->rows = malloc(conn->db->max_rows * sizeof(struct Address));
    for (int i = 0; i < max_rows; ++i) {
        addr = &conn->db->rows[i];
        addr->id = i;
        addr->set = 0;

        addr->name = malloc(conn->db->max_data);
        addr->name[0] = '\0';

        addr->email = malloc(conn->db->max_data);
        addr->email[0] = '\0';
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
    struct Address *addr = &conn->db->rows[id];
    if (addr->set) die("Already set, delete it first", NULL);

    addr->set = 1;
    char *res = strncpy(addr->name, name, conn->db->max_data);
    addr->name[conn->db->max_data-1] = '\0';
    if (!res) die("Name copy failed", NULL);

    res = strncpy(addr->email, email, conn->db->max_data);
    addr->email[conn->db->max_data-1] = '\0';
    if (!res) die("Email copy failed", NULL);
}

void Database_get(struct Connection *conn, int id) {
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", NULL);
    }
}
void Database_find(struct Connection *conn, const char *email) {
  struct Database *db = conn->db;
  for (int i = 0; i < conn->db->max_rows; i++) {
    struct Address *cur = &db->rows[i];
    if (strstr(cur->email, email)) {
      Address_print(cur);
    }
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
    printf("Rows: %d", conn->db->max_rows);
    struct Database *db = conn->db;
    for (int i = 0; i < conn->db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1];
    char action = argv[2][0];

    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    int max_data, max_rows;
    switch (action) {
        case 'c':
            max_data = (argc >= 4) ? atoi(argv[3]) : 512;
            max_rows = (argc >= 5) ? atoi(argv[4]) : 100;
            Database_create(conn, max_data, max_rows);
            Database_write(conn);
            break;
        case 'g':
            if (argc > 3) id = atoi(argv[3]);
            if (id >= conn->db->max_rows) die("There's not that many records.", NULL);
            if (argc != 4) die("Need an id to get", NULL);
            Database_get(conn, id);
            break;
        case 'f':
            if (argc != 4) die("Need an email to find", NULL);
            char *email = argv[3];
            Database_find(conn, email);
            break;
        case 's':
            if (argc > 3) id = atoi(argv[3]);
            if (id >= conn->db->max_rows) die("There's not that many records.", NULL);
            if (argc != 6) die("Need id, name, email to set", NULL);
            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;
        case 'd':
            if (argc > 3) id = atoi(argv[3]);
            if (id >= conn->db->max_rows) die("There's not that many records.", NULL);
            if (argc != 4) die("Need id to delete", NULL);
            Database_delete(conn, id);
            Database_write(conn);
            break;
        case 'l':
            Database_list(conn);
            break;
        default:
            die("invalid action, only: c=create, f=find, g=get, s=set, d=del, l=list", NULL);
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
