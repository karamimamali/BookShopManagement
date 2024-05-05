#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "bookshop.h"



int main() {
    sqlite3 *db;
    if (sqlite3_open("bookshop.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    
    initializeDatabase(db);
    
    int choice;
    int scanfResult;

    do {
        displayMenu();

        scanfResult = scanf("%d", &choice);

        if (scanfResult != 1) {
            while (getchar() != '\n');
            printf("\nInvalid input! Please enter a number.\n\n");
            continue;
        }

        switch (choice) {
            case 1:
                addBookToDatabase(db);
                break;
            case 2:
                updateBookDetails(db);
                break;
            case 3:
                showAllBooks(db);
                break;
            case 4:
                searchByBookTitle(db);
                break;
            case 5:
                searchByBookAuthor(db);
                break;
            case 6:
                sellBook(db);
                break;
            case 7:
                rentBook(db);
                break;
            case 8:
                showSalesReport(db);
                break;
            case 9:
                showRentalReport(db);
                break;
            case 10:
                printf("\nExiting the program...\n\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n\n");
        }
    } while (choice != 10);

    sqlite3_close(db);
    return 0;
}
