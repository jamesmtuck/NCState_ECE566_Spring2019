#include "sqlite3.h"
#include <stdio.h>

int callback(void *, int, char **, char **);

int main(void) {
    
  sqlite3 *db;
  char *err_msg = 0;
    
  int rc = sqlite3_open("test.db", &db);
    
  if (rc != SQLITE_OK) {
        
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
        
    return 1;
  }
    
    char *sql = "DROP TABLE IF EXISTS Cars;" 
                "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);" 
                "INSERT INTO Cars VALUES(1, 'Audi', 52642);" 
                "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);" 
                "INSERT INTO Cars VALUES(3, 'Skoda', 9000);" 
                "INSERT INTO Cars VALUES(4, 'Volvo', 29000);" 
                "INSERT INTO Cars VALUES(5, 'Bentley', 350000);" 
                "INSERT INTO Cars VALUES(6, 'Citroen', 21000);" 
                "INSERT INTO Cars VALUES(7, 'Hummer', 41400);" 
      "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {        
      fprintf(stderr, "SQL error: %s\n", err_msg);      
      sqlite3_free(err_msg);        
    } 
    
    char *sql2 = 
      "DROP TABLE IF EXISTS Friends;" 
      "CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);"
      "INSERT INTO Friends(Name) VALUES ('Tom');"
      "INSERT INTO Friends(Name) VALUES ('Rebecca');"
      "INSERT INTO Friends(Name) VALUES ('Jim');"
      "INSERT INTO Friends(Name) VALUES ('Roger');"
      "INSERT INTO Friends(Name) VALUES ('Robert');";
    
    
    rc = sqlite3_exec(db, sql2, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
      
      fprintf(stderr, "Failed to create table\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);
      sqlite3_free(err_msg);
      
    } else {
      
      fprintf(stdout, "Table Friends created successfully\n");
    }
    
    char *sql3 = "SELECT * FROM Cars";
        
    rc = sqlite3_exec(db, sql3, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
    } 

    // int last_id = sqlite3_last_insert_rowid(db);
    //printf("The last Id of the inserted row is %d\n", last_id);
    

    sqlite3_close(db);
    
    return 0;
}

int callback(void *NotUsed, int argc, char **argv, 
	     char **azColName) {
    
  NotUsed = 0;
    
  for (int i = 0; i < argc; i++) {

    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
    
  printf("\n");
    
  return 0;
}
