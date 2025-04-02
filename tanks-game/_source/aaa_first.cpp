#include "aaa_first.h"

#include <rpmalloc.h>
#include <rpnew.h> //only include once!
//malloc.c is included by rpmalloc.c with ENABLE_OVERRIDE=1 (not enabled), would only be used by stb_image
//to get it to compile, need to make _msize_base noexcept because that's what the current/latest Windows SDK wants

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

FirstLoadedObject::FirstLoadedObject() {
	//needs to be initialized before main(), because anything using the heap (like std::string or std::vector) will fail otherwise
	//also, this is *not* a HACK, because this is literally the only way to solve this problem in C/C++ (as far as I'm aware)
	rpmalloc_initialize();
}

FirstLoadedObject first_loaded;
