# Emp: a scriptable lighting calculation engine

After years of development, [Groundhog](http://www.groundhoglighting.com) has 
become stable and intuitive enough to be used by industry, students and 
academics. However, the more intuitive and stable a tool is, the bigger
the projects it is used for. Indeed, what was once a tool used only for solving 
models with small rooms and few windows has become an excelent tool for
teaching to students who use it for much larger models with really complex
requirements.

Solving larger models force a tool to be much more time efficient because no 
one wants to wait 35 minutes for the results to come back. Even though the 
[Radiance](http://www.radiance-online.org) processes are as fast as they can
(which is something this project will not focus on), an important fraction 
of the time required to process a model was spent in reading and parsing files,
analizing numerical data and other processes that could clearly be optimized.

This project intends to become the calculation engine behind [Groundhog](http://www.groundhoglighting.com)
and a tool for helping researchers, students and practitioners make their
work in a more efficient way.

## Origin of the name

Emp is not an acronym, but the short version of 'Empedocles'; who, according to Wikipedia:

>> ... is credited with the first comprehensive theory of light and vision. He put forward the idea that we see objects because light streams out of our eyes and touches them. 

Despite being wrong, this theory will sound highly familiar to anyone who has read about [Backward Ray-tracing](https://en.wikipedia.org/wiki/Ray_tracing_(graphics)) algorithms.

## Relevant features

- **Cross-platform:** Emp is designed to work on Linux, macOS and Windows... although 
it has only been developed and tested on Windows so far.
- **Radiance-based:** Thanks to [Radiance](http://www.radiance-online.org), Emp is
built upon years of experience, research and testing.
- **Capable of reading CAD formats:** Emp was designed in order to work directly from
different CAD files. SKB format is the only one supported for now.
- **Scriptable:** Thanks to [Lua](https://www.lua.org/), Emp can be scripted and thus 
used for research and practice in a very efficient way. This also helps non-experts
programmers help develop new features and functions in the shape of scripts.
- **Do not repeat processes:** Based on Groundhog's simulation manager, Emp
was provided with a Task-manager, which is able to understand several broad tasks (i.e. 
calculate Daylight Autonomy and Useful Daylight Illuminance) and eliminate redundant 
tasks (i.e. perform an annual simulation).
- **Parallel computing:** Thanks to [Intel Threading Building Blocks](https://github.com/01org/tbb),
Emp's Task Manager can schedule tasks and use parallel computing to leverage 
all the power in your machine and reduce calculation time.
- **Embeddable:** Emp was designed to be shipped with Groundhog and/or other tools.

## Important notice

So far, the SKP models that are read by Emp are meant to be created with Groundhog.
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