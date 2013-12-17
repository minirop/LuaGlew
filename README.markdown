Name
====

luaglew: use OpenGL in lua5.2

Version
=======

Currently, only OpenGL 2.1 is (partially) implemented.
3.x and 4.x will come later.

How to build
============

**requirements**: dynamic-build lua (Windows: like one from LuaBinaries) and glew

* Windows: you can just type Make if you have all requirements
* Linux: Compile gl.cpp with lua5.2 and glew

How to use
==========

* Move the .dll/.so near your lua file
* Use with `local gl = require('luaglew')`

Documentation
=============

See gl.cpp, but there is a few things to know:

* functions don't have the gl prefix. If importing like in the "How to use" section, you just do: *gl.FunctionName*
* defines are not modified: i.e. *gl.GL_MODELVIEW*
* most functions (mostly the ones without a pointer parameter) are 1-to-1 with the C API.
* suffixes are dropped
    * when there are several versions, only the double or the float is used: no more *glColor3\*()*, only *gl.Color3(double, double, double)* exists.
    * when there is only one version, remove the suffox: glGetQueryiv() become gl.GetQuery()
* vectors versions are not available: no *glColor3\*v()*
* functions filling an array, instead return a lua table: *void glGenTextures(GLsizei, GLuint\*)* --> *table gl.GenTextures(int)*
* all glGet(Integer/Boolean/Float/Double)v are merged into: *value/table gl.Get(type, pname)*.
    * *type* is *GL\_FLOAT*, *GL\_DOUBLE*, *GL\_BOOL* or *GL\_INT*.
    * *pname* is the same as the C param (i.e. *GL_DEPTH_RANGE*, *GL_CURRENT_COLOR*, etc.).
* Most function taking a pointer are not implemented. They are available in lua, but will raise an exception if used.

TODO
====

* finish to implement OpenGL 2.1
* finish to implement glu
* implement glX
* check parameter count
* some functions like *glIndexPointer* can have parameters with multiple meanings(?), i.e.:
```
	If a non-zero named buffer object is bound while a color index array is specified, *pointer* is treated as a byte offset into the buffer object's data store
```
how to manage those things??? (i.e. *I need help*)

Licence
=======

Copyright © 2013 Alexander Roper

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.