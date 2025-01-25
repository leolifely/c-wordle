# c-wordle
A very simple implementation of the popular game wordle written in C.
## Usage
`c-wordle /path/to/wordlist number_of_attempts`  
For example, on Arch Linux, with the words package (`sudo pacman -Sy words`), you can run:  
`c-wordle /usr/share/dict/words 5`
## Building
This project uses CMake, so requires the following packages (Arch Linux):  
`base-devel`  
`cmake`  
To build and install, run the following commands:  
`mkdir build`  
`cd build`  
`cmake ..`  
`make`  
`sudo make install`
