# Station manager

Part of coursework of my Operating Systems class at my university.
This program simulates a bus station, in which the station manager handles bus traffic according to the rules in a file given by the user.  

## Getting Started

These instructions will get you a copy of the project up and running on your local machine.

### Prerequisites

- gcc : open source C\C++ compiler
- make : instalation tool

````
$ sudo apt update && sudo apt upgrade
$ sudo apt-get install gcc make
````

### Installing

Obtain the repository either by dowloading it or by cloning it from GitHub.

To clone it:

````bash
git clone https://github.com/nathanasia/station_manager.git
````

### Running

Use the Makefile to compile the code

````
make
````

Execute it

````c
./mystation -l confile.txt
````

**NOTE:** To terminate the program the user should press a key

**NOTE:** You can create your own confile.txt

#### Configuration file (confile.txt)

This file includes all the parameters that this program needs to run, such as the number of buses that will be generated, the time a bus is allowed to park, etc. The structure of the configuration file (confile.txt) is as follows:

Each parameter has its own line. The value of each parameter is located at the beginning of the paremeters' line.

#### Available parameters:
| Line number   | Description  |
| :-----: |:-------------|
| 1 | number of buses |
| 2 | time a bus is allowed to parking* |
| 3 | time limit that a bus can wait until an empty parking space is available* |
| 4 | the frequency that the controller prints the situation of the bus stations* |
| 5 | the frequency that the controller prints the statistics of the bus stations* |
| 6 | number of medians on bus station |
| 7 | capacity for VOR's median (North Greece) |
| 8 | capacity for ASK's median (Attica, Central Greece) |
| 9 | capacity for PEL's median (Pelloponisos) |

*: sleeping time of a process in seconds (1 <= recomended time <= 5)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

