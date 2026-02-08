/* Copyright 2026 Roos Catling-Tate
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with or
 * without fee is hereby granted, provided that the above copyright notice and this permission
 * notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "version.h"

/* This is to quiet IDE complaints. These should be defined by args passed to the compiler by way of CMake. */
#ifndef RGPSTK_VERSION_MAJOR
#define RGPSTK_VERSION_MAJOR 0
#endif
#ifndef RGPSTK_VERSION_MINOR
#define RGPSTK_VERSION_MINOR 0
#endif
#ifndef RGPSTK_VERSION_PATCH
#define RGPSTK_VERSION_PATCH 0
#endif

#ifdef __linux__
extern char *program_invocation_short_name;
#endif

inline int
rgpstk_version_major(void)
{
	return (RGPSTK_VERSION_MAJOR);
}

inline int
rgpstk_version_minor(void)
{
	return (RGPSTK_VERSION_MINOR);
}

inline int
rgpstk_version_patch(void)
{
	return (RGPSTK_VERSION_PATCH);
}

void
rgpstk_print_version(void)
{
	const char *program_name;

#if defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__DragonFly__) || defined(__APPLE__)
	program_name = getprogname();
	if (program_name == NULL || strlen(program_name) < 6 || strncmp(program_name, "rgpstk", 6) != 0)
		program_name = "rgpstk lib";
#elif defined(__linux__)
	program_name = program_invocation_short_name;
#else
	program_name = "RGPSTK";
#endif

	printf("Roos's GPS Tool Kit - %s\nv%d.%d.%d\n", program_name, RGPSTK_VERSION_MAJOR, RGPSTK_VERSION_MINOR,
	    RGPSTK_VERSION_PATCH);
}