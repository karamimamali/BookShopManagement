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
void initializeDatabase(sqlite3 *db);
void addBookToDatabase(sqlite3 *db);
void updateBookDetails(sqlite3 *db);
void showAllBooks(sqlite3 *db);
void searchByBookTitle(sqlite3 *db);
void searchByBookAuthor(sqlite3 *db);
void sellBook(sqlite3 *db);
void rentBook(sqlite3 *db);
void showSalesReport(sqlite3 *db);
void showRentalReport(sqlite3 *db);


#endif 
