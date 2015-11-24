# Pallache
A small and simple header only math parser library:
```
/* By BC van Zuiden -- Leiden, November 2015 */
/* Probably very buggy USE AT OWN RISK this will brick everything you own */
/* NOBODY but YOU is liable for anything that happened in result of using this */
/* WARNING: DON'T RUN THIS PROGRAM THIS WILL DESTROY YOUR COMPUTER AND/OR KILOBOTS */
/* Any copyrighted piece of code within this code is NOT mine */
/* Inclusion of that code is forced upon me by a scary anonymous guy with a gun*/
```
Feel free to reuse and contribute, pull requests are very welcome!
This program is (and forever will be) a work in progress.

### This program acknowledges:
* [Samuel Pallache](https://en.wikipedia.org/wiki/Samuel_Pallache) yarr!
* [Edsger W. Dijkstra](https://en.wikipedia.org/wiki/Edsger_W._Dijkstra)
* [neovim](https://neovim.io)

### Todo
* Implement function definitions 
* Support for automatic differentiation of functions 
* Support for complex numbers
* Improvements / Stability maintenance 

### Building
To build run something like:
```
make
```
To clean run:
```
make clean
```
To debug:
```
make debug
```
You can install by copying the compiled binary to your path.

### Running
After compilation run something like:
```
./pallache 
```
Should get you started.  
If you are lazy try:
```
rlwrap ./pallache
```
Or even more conviently:
```
rlwrap -c ./pallache
```
