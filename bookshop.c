#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sqlite3.h>
#include "bookshop.h"


void displayMenu() {
    printf("\n");
    printf("|-----------------------------------------|\n");
    printf("|          Bookshop Management System     |\n");
    printf("|-----------------------------------------|\n");
    printf("|  1. Add Book                            |\n");
    printf("|  2. Update Book                         |\n");
    printf("|  3. Display Books                       |\n");
    printf("|  4. Search By Title                     |\n");
    printf("|  5. Search By Author                    |\n");
    printf("|  6. Sell Book                           |\n");
    printf("|  7. Rent Book                           |\n");
    printf("|  8. Generate Sales Report               |\n");
    printf("|  9. Generate Rental Report              |\n");
    printf("|  10. Exit                                |\n");
    printf("|-----------------------------------------|\n\n");
    printf("Enter your choice: ");
}

int initializeDatabase(sqlite3 *db) {
    char *errMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS books ("
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
    
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    }
}

int addBookToDatabase(sqlite3 *db) {
    struct Book new_book;
    printf("Enter book title: ");
    scanf(" %[^\n]", new_book.title);
    printf("Enter author: ");
    scanf(" %[^\n]", new_book.author);
    printf("Enter genre: ");
    scanf(" %[^\n]", new_book.genre);
    printf("Enter price: ");
    scanf("%f", &new_book.price);
    printf("Enter quantity available for sale: ");
    scanf("%d", &new_book.available_for_sale);
    printf("Enter quantity available for rent: ");
    scanf("%d", &new_book.available_for_rent);

    new_book.sold_count = 0;
    new_book.rent_count = 0;

    char sql[500];
    sqlite3_stmt *stmt;
    const char *tail;
    
    
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    sprintf(sql, "INSERT INTO books (title, author, genre, price, available_for_sale, available_for_rent, sold_count, rent_count) VALUES (?, ?, ?, ?, ?, ?, ?, ?);");

    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, &tail) == SQLITE_OK) {
        
        sqlite3_bind_text(stmt, 1, new_book.title, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, new_book.author, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, new_book.genre, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 4, new_book.price);
        sqlite3_bind_int(stmt, 5, new_book.available_for_sale);
        sqlite3_bind_int(stmt, 6, new_book.available_for_rent);
        sqlite3_bind_int(stmt, 7, new_book.sold_count);
        sqlite3_bind_int(stmt, 8, new_book.rent_count);

        
        int result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0); 
            return 1;
        }
    } else {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0); 
        return 1;
    }

    sqlite3_finalize(stmt);

    sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    
    printf("\nBook added successfully!\n\n");
    return 0;
}



int updateBookDetails(sqlite3 *db) {
    char title[100];
    printf("Enter the title of the book to update: ");
    scanf(" %[^\n]", title);

    char sql_check[200];
    sprintf(sql_check, "SELECT * FROM books WHERE title = ?;");

    sqlite3_stmt *stmt_check;
    if (sqlite3_prepare_v2(db, sql_check, -1, &stmt_check, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(stmt_check, 1, title, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt_check) != SQLITE_ROW) {
        printf("Book not found in inventory!\n\n");
        sqlite3_finalize(stmt_check);
        return 1;
    }

    sqlite3_finalize(stmt_check);

    struct Book updated_book;
    printf("Enter updated title: ");
    scanf(" %[^\n]", updated_book.title);
    printf("Enter updated author: ");
    scanf(" %[^\n]", updated_book.author);
    printf("Enter updated genre: ");
    scanf(" %[^\n]", updated_book.genre);
    printf("Enter updated price: ");
    scanf("%f", &updated_book.price);
    printf("Enter updated quantity available for sale: ");
    scanf("%d", &updated_book.available_for_sale);
    printf("Enter updated quantity available for rent: ");
    scanf("%d", &updated_book.available_for_rent);

    char sql_update[500];
    sqlite3_stmt *stmt_update;
    const char *tail;
    
    
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    sprintf(sql_update, "UPDATE books SET title = ?, author = ?, genre = ?, price = ?, available_for_sale = ?, available_for_rent = ? WHERE title = ?;");

    
    if (sqlite3_prepare_v2(db, sql_update, -1, &stmt_update, &tail) == SQLITE_OK) {
        
        sqlite3_bind_text(stmt_update, 1, updated_book.title, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt_update, 2, updated_book.author, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt_update, 3, updated_book.genre, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt_update, 4, updated_book.price);
        sqlite3_bind_int(stmt_update, 5, updated_book.available_for_sale);
        sqlite3_bind_int(stmt_update, 6, updated_book.available_for_rent);
        sqlite3_bind_text(stmt_update, 7, title, -1, SQLITE_STATIC);

        
        int result = sqlite3_step(stmt_update);
        if (result != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_update);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0); 
            return 1;
        }
    } else {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0); 
        return 1;
    }

    
    sqlite3_finalize(stmt_update);

    
    sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    
    printf("\nBook details updated successfully!\n\n");
    return 0;
}


int showAllBooks(sqlite3 *db) {
    printf("\n------------------------------------------\n");
    printf("       Books in Inventory\n");
    printf("------------------------------------------\n\n");

    char *sql = "SELECT * FROM books;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Title: %s | Author: %s | Genre: %s | Price: $%.2f | Sale Qty: %d | Rent Qty: %d\n",
               sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 3),
               sqlite3_column_double(stmt, 4), sqlite3_column_int(stmt, 5), sqlite3_column_int(stmt, 6));
    }

    sqlite3_finalize(stmt);
    printf("\n");
    return 0;
}



int searchByBookTitle(sqlite3 *db) {
    char title[100];
    printf("Enter the title of the book: ");
    scanf(" %[^\n]", title);

    printf("\n------------------------------------------\n");
    printf("             Search Result\n");
    printf("------------------------------------------\n\n");

    char *sql = "SELECT * FROM books WHERE title = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);

    int isPresent = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        isPresent++;
        printf("Title: %s | Author: %s | Genre: %s | Price: $%.2f | Sale Qty: %d | Rent Qty: %d\n\n",
               sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 3),
               sqlite3_column_double(stmt, 4), sqlite3_column_int(stmt, 5), sqlite3_column_int(stmt, 6));
    }
    if (!isPresent) {
        printf("There is no book called %s in inventory\n\n", title);
    }

    sqlite3_finalize(stmt);
    return 0;
}

int searchByBookAuthor(sqlite3 *db) {
    char author[100];
    printf("Enter the author of the book: ");
    scanf(" %[^\n]", author);

    printf("\n------------------------------------------\n");
    printf("             Search Result\n");
    printf("------------------------------------------\n\n");

    char *sql = "SELECT * FROM books WHERE author = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(stmt, 1, author, -1, SQLITE_TRANSIENT);

    int isPresent = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        isPresent++;
        printf("Title: %s | Author: %s | Genre: %s | Price: $%.2f | Sale Qty: %d | Rent Qty: %d\n\n",
               sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 3),
               sqlite3_column_double(stmt, 4), sqlite3_column_int(stmt, 5), sqlite3_column_int(stmt, 6));
    }
    if (!isPresent) {
        printf("There is no book with author %s in inventory\n\n", author);
    }

    sqlite3_finalize(stmt);

    return 0;
}

int sellBook(sqlite3 *db) {
    char bookTitle[100];
    int quantity;
    
    printf("Enter the title of the book to sell: ");
    scanf(" %[^\n]", bookTitle);

    char *sql_check = "SELECT * FROM books WHERE title = ?;";
    sqlite3_stmt *stmt_check;

    if (sqlite3_prepare_v2(db, sql_check, -1, &stmt_check, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(stmt_check, 1, bookTitle, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt_check) != SQLITE_ROW) {
        printf("Book not found in inventory!\n\n");
        sqlite3_finalize(stmt_check);
        return 1;
    }

    int availableQuantity = sqlite3_column_int(stmt_check, 5);
    double pricePerUnit = sqlite3_column_double(stmt_check, 4);
    sqlite3_finalize(stmt_check);

    printf("Enter the quantity to sell: ");
    scanf("%d", &quantity);

    if (quantity <= 0) {
        printf("\nPlease enter a positive quantity!\n\n");
        return 1;
    }

    if (availableQuantity >= quantity) {
        char *sql_update_sale_qty = "UPDATE books SET available_for_sale = available_for_sale - ?, sold_count = sold_count + ? WHERE title = ?;";
        sqlite3_stmt *stmt_update_sale_qty;

        
        sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

        if (sqlite3_prepare_v2(db, sql_update_sale_qty, -1, &stmt_update_sale_qty, 0) != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0); 
            return 1;
        }

        sqlite3_bind_int(stmt_update_sale_qty, 1, quantity);
        sqlite3_bind_int(stmt_update_sale_qty, 2, quantity);
        sqlite3_bind_text(stmt_update_sale_qty, 3, bookTitle, -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt_update_sale_qty) != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_update_sale_qty);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        sqlite3_finalize(stmt_update_sale_qty);

        sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);

        double totalPrice = pricePerUnit * quantity;
        printf("\nTotal amount: $%.2lf\n", totalPrice);
        printf("Book sold successfully!\n\n");
    } else {
        printf("\nInsufficient quantity available for sale!\n\n");
    }

    return 0;
}



int rentBook(sqlite3 *db) {
    char bookTitle[100];
    int quantity, durationMonths;
    
    printf("Enter the title of the book to rent: ");
    scanf(" %[^\n]", bookTitle);

    char *sql_check = "SELECT * FROM books WHERE title = ?;";
    sqlite3_stmt *stmt_check;

    if (sqlite3_prepare_v2(db, sql_check, -1, &stmt_check, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    sqlite3_bind_text(stmt_check, 1, bookTitle, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt_check) != SQLITE_ROW) {
        printf("Book not found in inventory!\n\n");
        sqlite3_finalize(stmt_check);
        return 1;
    }

    int availableQuantity = sqlite3_column_int(stmt_check, 6);
    double pricePerUnit = sqlite3_column_double(stmt_check, 4);
    sqlite3_finalize(stmt_check);

    printf("Enter the quantity to rent: ");
    scanf("%d", &quantity);
    printf("Enter the duration in months to rent: ");
    scanf("%d", &durationMonths);

    if (quantity <= 0 || durationMonths <= 0) {
        printf("\nPlease enter a positive quantity and duration!\n\n");
        return 1;
    }

    if (availableQuantity >= quantity) {
        char *sql_update_rent_qty = "UPDATE books SET available_for_rent = available_for_rent - ?, rent_count = rent_count + ? WHERE title = ?;";
        sqlite3_stmt *stmt_update_rent_qty;

        
        sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

        if (sqlite3_prepare_v2(db, sql_update_rent_qty, -1, &stmt_update_rent_qty, 0) != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0); 
            return 1;
        }

        sqlite3_bind_int(stmt_update_rent_qty, 1, quantity);
        sqlite3_bind_int(stmt_update_rent_qty, 2, quantity);
        sqlite3_bind_text(stmt_update_rent_qty, 3, bookTitle, -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt_update_rent_qty) != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_update_rent_qty);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        sqlite3_finalize(stmt_update_rent_qty);

        
        sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);

        char *sql_insert_rent = "INSERT INTO rents (book_id, duration, quantity) VALUES ((SELECT id FROM books WHERE title = ?), ?, ?);";
        sqlite3_stmt *stmt_insert_rent;

        
        sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

        if (sqlite3_prepare_v2(db, sql_insert_rent, -1, &stmt_insert_rent, 0) != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        sqlite3_bind_text(stmt_insert_rent, 1, bookTitle, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt_insert_rent, 2, durationMonths);
        sqlite3_bind_int(stmt_insert_rent, 3, quantity);

        if (sqlite3_step(stmt_insert_rent) != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_insert_rent);
            sqlite3_exec(db, "ROLLBACK TRANSACTION", 0, 0, 0);
            return 1;
        }

        sqlite3_finalize(stmt_insert_rent);

        
        sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);

        double totalCost = (pricePerUnit * quantity * durationMonths) / 12.0;
        printf("\nTotal amount: $%.2lf\n", totalCost);
        printf("Book rented successfully!\n\n");
    } else {
        printf("\nInsufficient quantity available for rent!\n\n");
    }

    return 0;
}




int showSalesReport(sqlite3 *db) {
    printf("\n------------------------------------------\n");
    printf("           Sales Report\n");
    printf("------------------------------------------\n");

    char *query = "SELECT * FROM books;";
    sqlite3_stmt *statement;

    if (sqlite3_prepare_v2(db, query, -1, &statement, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    double totalSalesRevenue = 0;
    struct Book bestSellingBook;
    bestSellingBook.sold_count = -1;

    while (sqlite3_step(statement) == SQLITE_ROW) {
        double price = sqlite3_column_double(statement, 4);
        int soldCount = sqlite3_column_int(statement, 7);
        totalSalesRevenue += price * soldCount;

        if (soldCount > bestSellingBook.sold_count) {
            bestSellingBook.sold_count = soldCount;
            const unsigned char *title = sqlite3_column_text(statement, 1);
            const unsigned char *author = sqlite3_column_text(statement, 2);
            strcpy(bestSellingBook.title, (const char *)title);
            strcpy(bestSellingBook.author, (const char *)author);
        }
    }

    printf("\nTotal Sales Revenue: $%.2lf\n", totalSalesRevenue);
    printf("The best selling book is \"%s\" by %s\n", bestSellingBook.title, bestSellingBook.author);

    sqlite3_finalize(statement);
    printf("\n");
    return 0;
}


int showRentalReport(sqlite3 *db) {
    printf("\n------------------------------------------\n");
    printf("           Rental Report\n");
    printf("------------------------------------------\n");

    char *query = "SELECT b.title, b.price, r.quantity, r.duration FROM rents r JOIN books b ON r.book_id = b.id;";
    sqlite3_stmt *statement;

    if (sqlite3_prepare_v2(db, query, -1, &statement, 0) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    double totalRentalRevenue = 0;
    int totalRentalDuration = 0;
    int totalRentalQuantity = 0;
    int totalRentals = 0;

    while (sqlite3_step(statement) == SQLITE_ROW) {
        double price = sqlite3_column_double(statement, 1);
        int quantity = sqlite3_column_int(statement, 2);
        int duration = sqlite3_column_int(statement, 3);

        double rentalRevenue = (price * quantity * duration) / 12.0;
        totalRentalRevenue += rentalRevenue;

        totalRentalDuration += duration;
        totalRentalQuantity += quantity;

        totalRentals++;
    }

    double averageRentalDuration = (totalRentals > 0) ? (double)totalRentalDuration / totalRentals : 0;

    printf("\nTotal Rental Revenue: $%.2lf\n", totalRentalRevenue);
    printf("Average Rental Duration: %.2lf months\n", averageRentalDuration);
    printf("Total Rental Quantity: %d\n", totalRentalQuantity);

    sqlite3_finalize(statement);
    printf("\n");

    return 0;
}
