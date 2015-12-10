# Pallache
A small and simple header only math parser library (written in C++14):
```
/* By BC van Zuiden -- Leiden, November 2015 */
/* Probably very buggy USE AT OWN RISK this will brick everything you own */
/* NOBODY but YOU is liable for anything that happened in result of using this */
/* WARNING: DON'T RUN THIS PROGRAM THIS WILL DESTROY YOUR COMPUTER AND/OR HOUSE */
/* Any copyrighted piece of code within this code is NOT mine */
/* Inclusion of that code is forced upon me by a scary anonymous guy with a gun*/
```
Feel free to reuse and contribute, pull requests are very welcome!
This code is (and forever will be) a work in progress.

### Todo
* Implement function definitions 
* Add more special functions and operators
* Add (psuedo) random number generator(s)
* Add support for complex numbers (maybe quaternions?)
* Add support for automatic differentiation of functions (using dual numbers)
* Add support for vectors and matrices (do we really want to go there?)
* Improvements / Bugfixes / Stability / Maintenance (the usual)

### Building
To build run something like:
```
make
```

To debug:
```
make debug
```
This will print out some debug messages while parsing.

To test:
```
make test
```
This should print a bunch of ones and exit (otherwise file a bug report).

To clean run:
```
make clean
```
This will remove the compiled binary.

You can install by copying the compiled binary to your path.

### Running
After compilation run something like:
```
./pallache 
```
Should get you started.  

If you are lazy -- like me -- try:
```
rlwrap ./pallache
```

### Acknowledgements
* [Samuel Pallache](https://en.wikipedia.org/wiki/Samuel_Pallache) yarr!
* [Edsger W. Dijkstra](https://en.wikipedia.org/wiki/Edsger_W._Dijkstra)
* [neovim](https://neovim.io)
