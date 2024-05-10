#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "test_bookshop.h"
#include "../bookshop.h" 

// we will se if our all functions are running correctly with an in-memory database
int main() {
    sqlite3 *db = createDatabase();
    assert(addBookToDatabase(db)==0);
    assert(updateBookDetails(db)==0);
    assert(showAllBooks(db)==0);
    assert(searchByBookTitle(db)==0);
    assert(searchByBookAuthor(db)==0);
    assert(sellBook(db)==0);
    assert(rentBook(db)==0);
    assert(showSalesReport(db)==0);
    assert(showRentalReport(db)==0);

    
    printf("All tests passed!\n");
    sqlite3_close(db); 
    return 0;
}


sqlite3 *createDatabase() {
    sqlite3 *db;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        fprintf(stderr, "Can't open in-memory database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS books ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "title TEXT,"
                                 "author TEXT,"
                                 "genre TEXT,"
                                 "price REAL,"
                                 "available_for_sale INTEGER,"
                                 "available_for_rent INTEGER,"
                                 "sold_count INTEGER,"
                                 "rent_count INTEGER"
                                 ");"
                                 "CREATE TABLE IF NOT EXISTS rents ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "book_id INTEGER,"
                                "duration INTEGER,"
                                "quantity INTEGER"
                                ");";

    if (sqlite3_exec(db, createTableSQL, NULL, NULL, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to create 'books' table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    return db;
}
