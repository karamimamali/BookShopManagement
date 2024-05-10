#ifndef BOOKSHOP_H
#define BOOKSHOP_H

#include <sqlite3.h>

#define MAX_BOOKS 100

struct Book {
    char title[100];
    char author[100];
    char genre[50];
    float price;
    int available_for_sale;
    int available_for_rent;
    int sold_count;
    int rent_count;
};

struct Rent {
    char title[100];
    int quantity;
    int duration;
};

void displayMenu();
int initializeDatabase(sqlite3 *db);
int addBookToDatabase(sqlite3 *db);
int updateBookDetails(sqlite3 *db);
int showAllBooks(sqlite3 *db);
int searchByBookTitle(sqlite3 *db);
int searchByBookAuthor(sqlite3 *db);
int sellBook(sqlite3 *db);
int rentBook(sqlite3 *db);
int showSalesReport(sqlite3 *db);
int showRentalReport(sqlite3 *db);


#endif 
