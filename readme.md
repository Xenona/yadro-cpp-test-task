# Introduction

The test task I solved is dedicated to the problem of managing clients in some gaming club. Basically, there are some events that can happen, such as 'ClientCame', or 'ClientLeft', or 'ClientChoseTable', and the program introduced keeps track of those to provide the end user with the following info (example for the test given in the task): 

| TableID | Income from the table | Time the table was taken |
| ------- | --------------------- | ------------------------ |
| 1       | 70                    | 05:58                    |
| 2       | 30                    | 02:18                    |
| 3       | 90                    | 08:01                    |


## Building the program and the tests

I used CMake for the task, so to build the code, you need to execute the following commands: 

```sh
cmake -DCMAKE_BUILD_TYPE=Release -S ./ -B ./build
cmake --build ./build
```

## Running the program

After building, you can run the tests with the following command:

```sh
./build/test_task ./Tests/test1.txt
```

In `/Tests` folder you can find 3 sample files for testing. There I also provided correct output for each test. F.e., `testout1.txt` is the correct output for the `test1.txt` input.


## How it works

The hardest part is hidden in the input checking. I should stop the program whenever it encounters wrong line, so for that a lot of utility functions were written. Important to note that I wasn't using implicit type coercions (such as redirecting `cin` to `int` variable), because that would lead to a lot of headache with white spaces. 

Instead, program reads line by line and parses number of tables, working hours and cost per hour separately. Then it collects all the events, parses each and stored in vector of `std::variant`s. The way I parsed you can see in `EventVisitor::parseEvents` function. 

That's where all the magic is hidden, to be fair. As later I just apply the visitor on all the events, and it changes the statistics, current clients, queue of clients, etc. 


# Some test cases provided 

Some brief explanation why I left these tests in `/Tests` directory.

## Test Case 1

This test case uses the example directly provided in the task instructions.


## Test Case 2

This test case verifies how the system automatically kicks out clients after the working day ends.
 
## Test Case 3

This test checks that clients in the queue actually take the tables that just got free. 

## Google Unit Test Library

I used the library for writing (really) small unit tests for some utility functions (mostly for checking the correctness of the input). To run the tests, you should build the project first (check the article above) and run:

```sh
./build/tests
```
 
To add more tests, check the `./Tests/tests.cc` file.
