
# CWE-89 Code Example

## Description of the code

### authenticate.cpp

This is the file that we are trying to exploit. This file does the following:

1. In main, the application opens an existing MySQL DB file that contains an ID, username, password, and a secret for each entry. It then asks the user to specify a username and a password.

3. The application then calls the authenticate function which looks for an existing entry with the inputted username and password within the DB file.

4. If an entry was found, the application retrieves the secret through the get_secret function. It then displays the secret to the user then terminates.

5. If an entry was not found, the application displays an error message then terminates.

### exploit.sh

This file is for exploiting authenticate.cpp. What it does is it provides authenticate.cpp with the string “' OR id = $i —“”. This effectively injects SQL code within the query that the authenticate function uses to verify the user’s input. This particular SQL injection causes the authenticate function to always return true as long as there is an entry with id equaling $i.  

Once the user is authenticated, authenticate.cpp will then call get_secret which utilizes the username in its query to get the secret. Using the same malicious input as the username will cause the get_secret function to return the secret of the entry with id equalling $i.

### run.sh

This file runs the exploit.sh script in a for loop to get all possible secrets from the database.

### sqlite3.c & sqlite3.h

These two files are for the authenticate.cpp function to access the sqlite cpp interface as that currently does not exist on the linux machines.

### users.db

This is the database that stores the IDs, usernames, passwords, and secrets of users.

### MakeFile

Compiles authenticate.cpp

## How and where to compile/run your code

Compiling and running the code is very similar to the assignments. Simply compile the code using the MakeFile by running the following: 

```
$ make
```

This should result in an executable called “authenticate” which you can run. This can be done on the UCalgary CS Linux machines.

## How and where to compile/run your exploit

Assuming your are in the directory where the executable and the exploit are, you can run the exploit by running the following:

```
$ sh run.sh
```

This can also be done on the UCalgary CS Linux machines.
