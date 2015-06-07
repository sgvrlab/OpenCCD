/*******************************************************************************************/
* OpenCCD
/*******************************************************************************************/
* Version: 0.5
* Last update: Mar., 2009
* Maintained by: DukSu Kim ( bluekdct@gmail.com ), SungEui Yoon ( sungeui@gmail.com )
* SGlab, KAIST ( http://sglab.kaist.ac.kr )
* OpenCCD Homepage: http://sglab.kaist.ac.kr/OpenCCD
/*******************************************************************************************/

< Contents of Distribution >
  * Src: source code for OpenCCD library
  * Doc: documentation for the OpenCCD library
  * Demo1: One example demonstrating usage of the OpenCCD library 
  * include: Header files that should be included to an application

< System Requirements >
  * OS : Microsoft windows

< How to build OpenCCD >
  * Supported compiler: MS Visual Studio 2005 or later version
  * Visual studio project files for VS2005 and VS2008 are included in the distribution.
  * For other compilers, add all source files into a project and try to compile it.

<Building an application>
  Setting up the library for use in your application is simple.
	1. Build the OpenCCD library
	2. Make sure you link to OpenCCD.lib
	3. The include folder included with the distribution should be in your include path
	4. Once the above two things have been done, all you need to do is "#include <OpenCCD.h>" in your application

---------------------------------------------------------------------- Copyright 2009 SGlag, KAIST