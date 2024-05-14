# Bookshop Management System Report

## How to Run

First compile it from root directory:
For main app `gcc -o main main.c bookshop.c -lsqlite3`
For testing `gcc -o test_bookshop tests/test_bookshop.c bookshop.c -lsqlite3 -Wall`

Then run it `./main` or `./test_bookshop`


## 1. Introduction

### Project Overview:
The Bookshop Management System aims to provide a comprehensive solution for managing a bookshop's inventory, sales, and rentals. This system enables users to efficiently add, update, and search for books, as well as process sales and generate reports. 

### Importance:
Managing a bookshop involves handling a vast inventory of books, tracking sales, and maintaining customer records. A robust management system streamlines these operations, improving efficiency, accuracy, and customer satisfaction. It also facilitates data-driven decision-making by providing insights into sales trends and inventory management.

## 2. System Design

### Architecture:
The system is designed using a modular approach, with separate functions for each feature such as adding books, updating details, searching, selling, and generating reports. The core functionality is implemented using the SQLite database to store book information and transactional data.

### Data Structure:
The primary data structure used is a relational database, represented by the 'books' and 'rents' tables. These tables store information about books, including title, author, genre, price, availability for sale or rent, and sales/rental counts.

## 3. Functionality

### Adding new books to the inventory:
Users can add new books to the inventory by providing details such as title, author, genre, price, and quantities available for sale or rent. The system ensures data integrity by validating inputs and executing SQL transactions.

### Displaying all books in the inventory:
The system retrieves and displays all books stored in the database, including their details such as title, author, genre, price, and availability for sale or rent.

### Searching for books by title, author, or genre:
Users can search for books by title or author to retrieve specific book details from the database. The system performs SQL queries to match search criteria and displays the results accordingly.

### Updating book details:
Users can update existing book details such as title, author, genre, price, and availability. The system executes SQL UPDATE statements to modify book records in the database.

### Selling books to customers:
The system enables users to sell books to customers by deducting the sold quantity from the available stock and updating sales counts. It calculates the total amount based on the price per unit and sold quantity.

### Renting books to customers:
Users can rent books to customers for a specified duration by deducting the rented quantity from the available stock and updating rental counts. The system calculates the total rental cost based on the price per unit, rented quantity, and duration.

### Viewing sales reports:
The system generates sales reports, including total sales revenue and the best-selling book based on the number of units sold.

### Viewing rental reports:
Users can generate rental reports, including total rental revenue, average rental duration, and total rented quantity.

## 4. Implementation Details

### Programming Languages and Tools:
The system is implemented in C programming language using SQLite as the database management system. It utilizes SQLite's C API for database operations and standard I/O functions for user interaction.

### Challenges Faced:
- Ensuring data integrity and transactional consistency while performing database operations.
- Handling user input validation and error handling to prevent runtime errors and SQL injection vulnerabilities.

## 5. Sample Usage

### User Interaction:
Users interact with the system through a command-line interface (CLI), where they can select options from a menu to perform various operations.

```plaintext
|-----------------------------------------|
|          Bookshop Management System     |
|-----------------------------------------|
|  1. Add Book                            |
|  2. Update Book                         |
|  3. Display Books                       |
|  4. Search By Title                     |
|  5. Search By Author                    |
|  6. Sell Book                           |
|  7. Rent Book                           |
|  8. Generate Sales Report               |
|  9. Generate Rental Report              |
|  10. Exit                                |
|-----------------------------------------|

Enter your choice: 
```

## 6. Testing

### Testing Approach:
The system is tested using an in-memory SQLite database to ensure functionality and reliability. Test cases cover all implemented features, including adding, updating, searching, selling, renting, and generating reports.

### Test Results:
All test cases passed successfully, demonstrating the correctness and robustness of the system's implementation.

## 7. Future Enhancements

### Ideas for Improvement:
- Implement user authentication and access control for secure system usage.
- Enhance reporting capabilities with graphical visualization for better data analysis.
- Integrate online payment gateways for seamless transactions.
- Extend functionality to support e-book management and digital rentals.