#ifndef EXTENSION_SQLITE_H
#define EXTENSION_SQLITE_H

#include <sqlite3.h>
#include <stdbool.h>
#include <map>

#include "functions.h"
#include "numbers.h"
#include "utils.h"
#include "list.h"
#include "storage.h"
#include "log.h"
#include "server.h"
#include "map.h"

#define SQLITE_MOO_VERSION      "2.0"
#define SQLITE_MAX_HANDLES      20  /* Maximum number of SQLite databases that can be open
                                     * at a single time. Can be overridden with an INT in
                                     * $server_options.sqlite_max_handles */

#define SQLITE_PARSE_TYPES      2   // Return all strings if unset
#define SQLITE_PARSE_OBJECTS    4   // Turn "#100" into OBJ
#define SQLITE_SANITIZE_STRINGS 8   // Strip newlines from returned strings.

typedef struct sqlite_conn
{
    sqlite3 *id;
    char *path;
    unsigned char options;
} sqlite_conn;

// Map of open connections
static std::map <int, sqlite_conn> sqlite_connections;
// Next database handle. This will get reset to 1 when all connections get closed.
static int next_sqlite_handle = 1;

/* The result of our last query from the callback
 * so the MOO can copy it into a Var from the builtin function. */
Var last_result = new_list(0);

// Forward declarations
extern const char *file_resolve_path(const char *);             // from fileio.cc
extern int parse_number(const char *, int *, int);              // from numbers.cc
extern int parse_float(const char *, double *);                 // from numbers.cc

// Other helper functions
bool valid_handle(int handle);
int next_handle();
int allocate_handle();
void deallocate_handle(int handle);
int database_already_open(const char *path);
int callback(void *, int, char **, char **);
void sanitize_string_for_moo(char *);
Var string_to_moo_type(char *, bool, bool);
Stream* object_to_string(Var *);

#endif /* EXTENSION_SQLITE_H */
