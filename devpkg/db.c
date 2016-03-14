#include <unistd.h>
#include <apr_errno.h>
#include <apr_file_io.h>

#include "db.h"
#include "bstrlib.h"
#include "dbg.h"

// static functions are only visible to other functions in same file
// const variable declaration means that you won't change the variable
// FILE is a macro defined in stdio.h, fopen returns a FILE pointer
static FILE *DB_open(const char *path, const char *mode) {
  return fopen(path, mode);
}

// closes the stream, all buffers are flushed
static void DB_close(FILE *db) {
  fclose(db);
}

// bstring is a better string type that provides some added performance and usability over regular C strings
// bread() is a function that takes a pointer to a function as an argument, bNread type is used to specify the
// type of function, fread() function is close enough to pass muster when cast, second argument is the value
// to be passed to the function pointed to by bNRead
static bstring DB_load() {
  FILE *db = NULL;
  bstring data = NULL;

  db = DB_open(DB_FILE, "r");
  check(db, "Failed to open database: %s", DB_FILE);

  data = bread((bNread)fread, db);
  check(data, "Failed to read from db file: %s", DB_FILE);

  DB_close(db);
  return data;

error:
  if (db) DB_close(db);
  if (data) bdestroy(data);
  return NULL;
}

int DB_update(const char *url) {
  if (DB_find(url)) {
    log_info("Already recorded as installed: %s", url)
  }

  FILE *db = DB_open(DB_FILE, "a+");
  check(db, "Failed to open DB file: %s", DB_FILE);

  // bfromcstr takes a standard C library '\0' terminated char buffer and generate a bstring with the same
  // contents
  bstring line = bfromcstr(url);
  // concat second arg to first arg
  bconchar(line, '\n');
  int rc = fwrite(line->data, blength(line), 1, db);
  check(rc == 1, "Failed to append to the db.");

  return 0;
error:
  if (db) DB_close(db);
  return -1;
}

int DB_find(const char *url) {
  bstring data = NULL;
  bstring line = bfromcstr(url);
  int res = -1;

  data = DB_load();
  check(data, "Failed to load: %s", DB_FILE);

  // binstr checks for bstring line in data starting at position 0
  if (binstr(data, 0, line) == BSTR_ERR) {
    res = 0;
  } else {
    res = 1;
  }

error: //fallthrough
  if (data) bdestroy(data);
  if (line) bdestroy(line);

  return res;
}

int DB_init() {
  // apr_pool_t is the fundamental pool type
  // memory pool allows you to easily manage a set of memory chunks, this way you allocate one memory pool and
  // you can allocate multiple memory chunks from the pool
  apr_pool_t *p = NULL;
  apr_pool_initialize();
  // pool just created, if parent pool is NULL, the new pool is a root pool
  apr_pool_create(&p, NULL);

  // access() function check the file named by the pathname is accessible via the mode provide, value of amode
  // is the bitwise-inclusive OR of the access permissions to be checked
  // apr_dir_make_recursive creates a new directoy on the file system
  if (access(DB_DIR, W_OK | X_OK) == -1) {
    apr_status_t rc = apr_dir_make_recursive(DB_DIR,
          APR_UREAD | APR_UWRITE | APR_UEXECUTE |
          APR_GREAD | APR_GWRITE | APR_GEXECUTE, p);
    check(rc == APR_SUCCESS, "Failed to make database dir: %s", DB_DIR);
  }

  if (access(DB_FILE, W_OK) == -1) {
    FILE *db = DB_open(DB_FILE, "w");
    check(db, "Cannot open database: %s", DB_FILE);
    DB_close(db);
  }

  apr_pool_destroy(p);
  return 0;

error:
  apr_pool_destroy(p);
  return -1;
}

int DB_list() {
  bstring data = DB_load();
  check(data, "Failed to read load: %s", DB_FILE);

  printf("%s", bdata(data));
  bdestroy(data);
  return 0;

error:
  return -1;
}
