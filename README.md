# node-libtorrent-ng

#### node-libtorrent New Generation

[![Build Status](https://travis-ci.org/lyssdod/node-libtorrent.svg?branch=master)](https://travis-ci.org/lyssdod/node-libtorrent) [![Build Status](https://ci.appveyor.com/api/projects/status/github/lyssdod/node-libtorrent?branch=master&svg=true)](https://ci.appveyor.com/project/lyssdod/node-libtorrent)

This package provides native bindings to [libtorrent rastebar](http://www.rasterbar.com/products/libtorrent/) as a [Node.js addon](http://nodejs.org/docs/latest/api/addons.html).

It's a fork of https://github.com/fanatid/node-libtorrent which incorporates few fixes and enhancements from various
contributors and which was ported to [NAN²](https://github.com/nodejs/nan) to allow usage across all versions of node.js
(NAN claims to support all major versions).

Please report any issues you find while running this code. Feel free to open a PR as well.

# Getting started
Execute in command line:
```
$ npm install node-libtorrent-ng
```
or copy repository and build bindings manually
```
$ git clone --recursive git://github.com/lyssdod/node-libtorrent
$ cd node-libtorrent
$ npm install
```
