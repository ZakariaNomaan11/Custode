Custode
The Italian word for "keeper" depends on context: custode (caretaker) that's why we used this name and you can keep it here. It's your Stock and Inventory Management System
It is a basic Stock and Inventory Management System in C language made by a group of university called "Level Dodger'. It is a simple CMD use system that can help your inventory manageable.

Custode is a small console-based inventory system in C with login, role-based menus, and stock in/out using files.​

Features
Sign up / login for Admin and User using admins.txt and users.txt.​

Role menus:

Admin: add items, list items, stock in, stock out.

User: list items, stock in, stock out.

Inventory and transactions stored in binary files (items.dat, transactions.dat).​

Build & Run
bash

git clone https://github.com/ZakariaNomaan11/Custode
cd custode
gcc main.c -o custode
./custode
On Windows (MinGW):

bash
gcc main.c -o custode.exe
custode.exe
Basic Flow
Sign up an admin (option 1 → As Admin).


Admin login (option 3) → add items and manage stock.

Sign up users (option 1 → As User) → user login (option 2) to manage stock.​

