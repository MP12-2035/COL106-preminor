# COL106-preminor
For the assignment to build a version tracker like git for files.

.................................................................
[Time Traveling File Version System]
-----------------------------------------

This project implements a file version control system similar to Git, supporting file creation, content insertion/update, snapshots, rollback to previous versions, and version tree visualization. A customized "Art Mode" enhances console output. (OPTIONAL)

---

[] How to Compile and Run:

# Open your terminal and navigate to the project folder containing the source files and the script.

(For example, on my laptop it is: *cd Desktop/TIME_TRAVELLER*)

# Run the compilation and execution script with the  following commands:

   *chmod +x compile_and_run.sh*

   *./compile_and_run.sh*

   This will compile the program, and if successful, launch it for interactive commands.

# When the program starts, it will prompt:

   [*] Do you want to enable Art Mode? (Enter ON/OFF)

   Please enter *ON* or *OFF* depending on your preference.

   (Art Mode, though optional, offers better visual support and illustrations)

---

[] Basic Usage

* After choosing Art Mode functionality, you can enter commands such as:

  *CREATE <filename>*           : Create a new file (or "Untitled#" if no name given)

  *INSERT <filename> <text>*   : Insert text into a file

  *UPDATE <filename> <text>*   : Overwrite file contents with new text

  *SNAPSHOT <filename> <msg>*  : Save a version snapshot with a message

  *ROLLBACK <filename> [id]*   : Revert to a previous version by ID

  *READ <filename>*             : Display contents of a file

  *RENAME <old_filename> <new_filename>* : Rename a file

  *TREE <filename>*             : Show version tree visually

  *HELP*                       : Display all commands and their descriptions

  *EXIT*                       : Exit the program

* Commands are case-insensitive; Filenames though, are case-sensitive.

* Use the *HELP* command to see the full list of supported commands.

* A sample input file has been provided in the folder. 

* A sample log of input and output has also been provided.

---

[] Notes

* There are seven header files in the folder, namely:

  * art.hpp

  * commands.hpp

  * file_system.hpp

  * file.hpp

  * hash_map.hpp

  * heap.hpp

  * tree_node.hpp
..........................

* Requires a C++17 compatible compiler (e.g., g++).

* The program reads commands from standard input interactively; it also works with bulk text input, albeit not perfectly.

* Use the supplied shell script on Unix-like systems (Linux/macOS or Windows with Git Bash/WSL).

* The Art Mode can be toggled ON or OFF anytime using the *ARTMODE ON* / *ARTMODE OFF* command.

---

[] Contact / Support

For any issues or questions, please contact the author:

Samridhi Roy

2024MT10673

<mt1240673@iitd.ac.in>
........................................................

