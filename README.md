cleanup - v2.1 [![Build Status](https://travis-ci.org/thfabian/cleanup.svg?branch=master)](https://travis-ci.org/thfabian/cleanup)
==============

Cleanup will recursively search your file system, starting at $HOME by default, for files ending with '~' (Linux backup files) and delete them.

### Installation
Cleanup is based on [Boost][boost] and Intel(R) [TBB][tbb].

```bash
cd where-you-want-cleanup-to-live
git clone https://github.com/thfabian/cleanup
cd cleanup
mkdir build
cd build
cmake -G "Unix Makefiles" ../
make
sudo make install
```

[boost]: http://www.boost.org/
[tbb]: https://www.threadingbuildingblocks.org/