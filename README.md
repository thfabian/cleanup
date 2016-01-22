## cleanup - (3.0)

### DESCRIPTION

If you are using text editors like Emacs or Gedit your filesystem will be polluted with backup/swap files `~`. Cleanup will scan your `home/` directory recursivly and remove all of them in a few milliseconds.

### BUILD

Cleanup needs a working C++ toolchain and [CMake][cmake], in addition it depends on the following libraries:

* [Boost][boost]
* [TBB][tbb]

#### Ubuntu (14.04)
Install the prerequisite:

```bash
sudo apt-get install cmake libtbb-dev libboost-filesystem-dev libboost-serialization-dev
```

To compile and install the program to `/usr/local/bin`:

```bash
git clone https://github.com/thfabian/cleanup
cd cleanup
mkdir build && cd build
cmake ../
make
sudo make install 
```

### LICENSE

#####  The MIT License (MIT)
Copyright (c) 2016 Fabian Thuering

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[boost]: http://www.boost.org/
[tbb]: https://www.threadingbuildingblocks.org/
[cmake]: https://cmake.org/