/*******************
 *
 * Copyright 1998-2010 IAR Systems AB.  
 *
 * This is a template implementation of the "__write" function used by
 * the standard library.  Replace it with a system-specific
 * implementation.
 *
 * The "__write" function should output "size" number of bytes from
 * "buffer" in some application-specific way.  It should return the
 * number of characters written, or _LLIO_ERROR on failure.
 *
 * If "buffer" is zero then __write should perform flushing of
 * internal buffers, if any.  In this case "handle" can be -1 to
 * indicate that all handles should be flushed.
 *
 * The template implementation below assumes that the application
 * provides the function "MyLowLevelPutchar".  It should return the
 * character written, or -1 on failure.
 *
 ********************/

#include <yfuns.h>

int MyLowLevelGetchar();       // Function prototype
int MyLowLevelPutchar(int ch); // Function prototype


//#pragma module_name = "?__write" // Use this if you are adding a locally modified write.c to your project.

// Alternatively, defining the function with __ATTRIBUTES means that the linker will give higher priority 
// for this function than the __write it finds in write.c of the standard library. All the other functions 
// of the module will not be affected.
// See https://www.iar.com/support/resources/articles/overriding-and-redirecting-library-modules/
__ATTRIBUTES size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  /* Remove the #if #endif pair to enable the implementation */
#if 1    

  size_t nChars = 0;

  if (buffer == 0)
  {
    /*
     * This means that we should flush internal buffers.  Since we
     * don't we just return.  (Remember, "handle" == -1 means that all
     * handles should be flushed.)
     */
    return 0;
  }

  /* This template only writes to "standard out" and "standard err",
   * for all other file handles it returns failure. */
  if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size != 0; --size)
  {
    if (MyLowLevelPutchar(*buffer++) < 0)
    {
      return _LLIO_ERROR;
    }

    ++nChars;
  }

  return nChars;

#else

  /* Always return error code when implementation is disabled. */
  return _LLIO_ERROR;

#endif
}




/*******************
 *
 * Copyright 1998-2010 IAR Systems AB.  
 *
 * This is a template implementation of the "__read" function used by
 * the standard library.  Replace it with a system-specific
 * implementation.
 *
 * The "__read" function reads a number of bytes, at most "size" into
 * the memory area pointed to by "buffer".  It returns the number of
 * bytes read, 0 at the end of the file, or _LLIO_ERROR if failure
 * occurs.
 *
 * The template implementation below assumes that the application
 * provides the function "MyLowLevelGetchar".  It should return a
 * character value, or -1 on failure.
 *
 ********************/


//#pragma module_name = "?__read" // Use this if you are adding a locally modified read.c to your project.


// Alternatively, defining the function with __ATTRIBUTES means that the linker will give higher priority 
// for this function than the __read it finds in read.c of the standard library. All the other functions 
// of the module will not be affected.
// See https://www.iar.com/support/resources/articles/overriding-and-redirecting-library-modules/
__ATTRIBUTES size_t __read(int handle, unsigned char * buffer, size_t size)
{
  /* Remove the #if #endif pair to enable the implementation */
#if 1    

  int nChars = 0;

  /* This template only reads from "standard in", for all other file
   * handles it returns failure. */
  if (handle != _LLIO_STDIN)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size > 0; --size)
  {
    int c = MyLowLevelPutchar(MyLowLevelGetchar()); // Echo each character first
    //int c = MyLowLevelGetchar();                  // Don't echo each character first
    if (c < 0)
      break;

    *buffer++ = c;
    ++nChars;
  }

  return nChars;

#else

  /* Always return error code when implementation is disabled. */
  return _LLIO_ERROR;

#endif
}

