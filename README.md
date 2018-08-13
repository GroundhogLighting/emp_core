# Emp_core: an embeddable lighting calculation engine

Emp_core is a  [Radiance](https://www.radiance-online.org/)-based lighting calculation engine that
is meant to be embedded in other tools. The difference between using Emp_core and
plain Radiance is that it provides:

* Efficient out-of-the-box multicore processing across programs (i.e. call several RTRACE or RCONTRIB
threads at the same time)
* Out of the box script optimization, eliminating redundant tasks (i.e. do not create two
octrees for calculating the Daylight Factor and the Annual Illuminance... one is enough...
but please reuse the ambient files when possible)
* Read and write several file formats (i.e. allow me to draw my models in some modern
3D modelling tool)
* Simple automation of those tasks required on a daily basis, so I can work faster
and avoid errors (i.e. I do not want to write a script every time I want to perform a
Climate Based Daylight Simulation)
* Cross platform consistency (i.e. should I write `rcalc -e "$1 = $1+$2"` or
`rcalc -e '$1 = $1+$2'` ?)
* Workplane interpretation as geometry, not a grid of points (i.e. if I want to know
the Spatial Daylight Autonomy of a workplane, I can probably describe a polygon that
encolses it... but I do not want to write every point where the illuminance is measured)
* Post-processing capabilities (i.e. my workplane contains 4,528 sensors... I do not want
to know, nor write down, the illuminance of each of them on each of the 8,760 hours
of the year. Just return the CBD metric I asked for)
* Do not create 3,125 files, please (i.e. there are several files I am not interested in, which
are just intermediate results... please delete them afterwards)

In summary, Emp_core can be seen as a C++ library that can read 3D models in several
formats, that calls several Radiance programs in an efficient way, and knows how to
post-process the information accoring to the users/client needs.

## A bit of history

*FULL DISCLAIMER: I am the developer of [Groundhog](http://www.groundhoglighting.com)*

After years of development, [Groundhog](http://www.groundhoglighting.com) has
become stable and intuitive enough to be used by industry, students and
academics. However, the more intuitive and stable a tool is, the bigger
the projects it is used for. Indeed, what was once a tool used only for solving
models with small rooms and few windows has become an excelent tool for
teaching students, who use it for much larger models with really complex
requirements (or so I have seen in my students exams).

Solving larger models force a tool to be much more time efficient (because no
one wants to wait 35 minutes for the results to come back). Even though
[Radiance](http://www.radiance-online.org) programs do their jobs as fast as they can
(and improving them is something this project is not yet focused in), an important fraction
of the time required to process a model was spent in reading and parsing files,
analizing numerical data, and performing, in series, several tasks that could be
easily paralellized. In other words, there was a lot of space for optimization.

Even if this project was initially meant to become the calculation engine behind
[Groundhog](http://www.groundhoglighting.com), it took me very little time to realize that
it had to be divided into [Emp_core](https://github.com/GroundhogLighting/emp_core); and
several other wrappers meant to provide different users with the capabilities of Emp_core.
An example of a Wrapper is [Emp](https://github.com/GroundhogLighting/emp), a [Lua](https://www.lua.org/)
based implementation of Emp_core that tries to be as close to Radiance as it is possible
to it.


## Origin of the name

Emp is not an acronym, but the short version of 'Empedocles'; who, according to Wikipedia:

>> ... is credited with the first comprehensive theory of light and vision. He put forward the idea that we see objects because light streams out of our eyes and touches them.

Despite being wrong, this theory will sound highly familiar to anyone who has read about [Backward Ray-tracing](https://en.wikipedia.org/wiki/Ray_tracing_(graphics)) algorithms.

## Relevant features

- **Cross-platform:** Emp is designed to work on Linux, macOS and Windows... although
it has been tested only on one macOS and one Windows PC.
- **Radiance-based:** Thanks to [Radiance](http://www.radiance-online.org), Emp_core is
built upon years of experience, research and testing.
- **Capable of reading CAD formats:** Emp_core was designed in order to work directly from
different CAD files. SKB format is the only one supported for now.
- **Embeddable:** Written in C++, Emp_core can be embedded in several other tools.
- **Do not repeat processes:** Based on Groundhog's simulation manager, Emp_core
was provided with a Task-manager, which is able to understand several broad tasks (i.e.
calculate Daylight Autonomy and Useful Daylight Illuminance) and eliminate redundant
tasks (i.e. perform an annual simulation).
- **Parallel computing:** Thanks to [Intel Threading Building Blocks](https://github.com/01org/tbb),
Emp's Task Manager can schedule tasks and use parallel computing to leverage
all the power in your machine and reduce calculation time.

## Important notice

So far, the SKP models that are read by Emp_core are meant to be created with Groundhog.
This is because a Groundhog model is a simple SKP model with metadata added to it,
enabling Emp to understand what surface is 'real' and what surface is, for examaple,
a workplane.


## What is in the box

- README.md : This file
- premake5.lua : The [premake](https://premake.github.io/) script for building the project
- make.lua : Some scripts used only for pre-release tasks, such as documenting and others.
It is used sort of like a make file
- main.h / main.cpp : The main header and source file
- main_test.h / main_test.cpp : The main and header files for running tests
- License.txt : The file with licensing information
- Doxyfile : The file used by Doxygen to generate the developer's documentation
- .gitmodules / .gitignore : Files used by Git version managing
- tests : Directory with automated tests (uses [Google Tests](https://github.com/google/googletest))
- standardscripts : Directory with standard scripts used as example and shipped with Emp for
performing common tasks
- src : The main directory with Emp's actual code
- prereleasescripts : lua scripts that are called by 'make.lua'
- premakescripts : lua scripts called before premake is called.
- guides : Directory with developer guidelines and specifications. They get documented in
the developer-doc
- googletest : Fork to [Google Tests](https://github.com/google/googletest), used for
compiling and running tests.
- Emp-doc : The user documentation (i.e. API reference, standard script reference, tutorials, etc.).
It is synced with the (Gitbook doc)[https://www.gitbook.com/book/groundhoglighting/Emp-doc/welcome]
- developer-doc : The developer documentation generated with Doxygen
- 3rdparty : Directory with external dependencies

## Building

Emp is intended to be cross-platform, thus we are using an automatic building tool. As Lua is the
languange chosen to accompany C++ within Emp, it was natural to use premake5 (which is based on Lua)
as such tool. However, Emp uses [Intel's Threading Building Blocks]()https://github.com/01org/tbb
(TBB) for allowing cross-platform parallel computing. Unfortunately, Intel TBB should be built using
make. So, for building you should:

### Build Intel TBB

Building IntelTBB is fairly easy in macOS, as the process has been automated in a premake command

```
# In macOS (and probably Linux... I have not tried)
premake5 intelTBB
```

On Windows, however, things have to be made "by hand"

1. Open the 3rdparty/IntelTBB/build/vs2013/makefile file in Visual Studio
2. Compile the solution in both DEBUG and RELEASE versions
3. This creates a directory with the corresponding architecture (x64 or Win32). Inside, you should see a Debug and Release directories
4. Move the Release directory to libs/RELEASE/tbb (i.e. changing the name), and the Debug to libs/DEBUG/tbb


### Create the project for your IDE

Emp is built using the simplest premake command... as mentioned in [Premake's documentation](https://github.com/premake/premake-core/wiki/Using-Premake):

> The simplest Premake command is:
>
>    premake5 [action]
>
> Premake defines the following list of actions out of the box; projects may also add their own custom actions.
>
> | Action      | Description                                       |
> |-------------|---------------------------------------------------|
> | vs2017      | Generate Visual Studio 2017 project files         |
> | vs2015      | Generate Visual Studio 2015 project files         |
> | vs2013      | Generate Visual Studio 2013 project files         |
> | vs2012      | Generate Visual Studio 2012 project files         |
> | vs2010      | Generate Visual Studio 2010 project files         |
> | vs2008      | Generate Visual Studio 2008 project files         |
> | vs2005      | Generate Visual Studio 2005 project files         |
> | gmake       | Generate GNU Makefiles (including [Cygwin][1] and [MinGW][2]) |
> | xcode4      | XCode projects (built-in [extension](https://github.com/premake/premake-core/tree/master/modules/xcode)) |
> | codelite    | CodeLite projects (built-in [extension](https://github.com/premake/premake-core/tree/master/modules/codelite)) |
>
> To generate Visual Studio 2017 project files, use the command:
>
>     premake5 vs2017
>
> You can see a complete list of the actions and other options supported by a project with the command:
>
>     premake5 --help
>

### Edit and compile

Finally, you should edit and compile with whatever IDE or toolset you use.

## License

Copyright (C) 2017 Germán Molina (germolinal@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

