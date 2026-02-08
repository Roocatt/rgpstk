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

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include <version.h>

#include "distance_delta.h"

enum action : char {
	UNKNOWN = '?',
	DISTANCE_DELTA = 'd',
	HELP = 'h',
	VERSION = 'v',
};

static void	usage(FILE *);
static bool	action_check(enum action *, enum action);

int
main(int argc, char **argv)
{
	enum action action = UNKNOWN;
	int res = 0, opt;
	bool bad_cfg = false;

	while ((opt = getopt(argc, argv, "dhv")) != -1) {
		switch (opt) {
		case 'd':
			bad_cfg = action_check(&action, DISTANCE_DELTA);
			break;
		case 'h':
			bad_cfg = action_check(&action, HELP);
			break;
		case 'v':
			bad_cfg = action_check(&action, VERSION);
			break;
		default:
			fprintf(stderr, "Unknown option %c.\n", opt);
			bad_cfg = true;
		}
	}

	if (bad_cfg) {
		fprintf(stderr, "\nInvalid parameters.\n\n");
		usage(stderr);
		res = -1;
		goto end;
	}

	switch (action) {
	case DISTANCE_DELTA:
		distance_delta_read_loop();
		break;
	case HELP:
		usage(stdout);
		break;
	case VERSION:
		rgpstk_print_version();
		break;
	default:
		fprintf(stderr, "\nNo run action found.\n\n");
		usage(stderr);
		res = -1;
	}

end:
	return (res);
}

static bool
action_check(enum action *cur_action, enum action new_action)
{
	bool res;

	if (*cur_action != UNKNOWN) {
		fprintf(stderr, "Option '%c' cannot be used with option '%c'", *cur_action, new_action);
		res = true;
	} else {
		*cur_action = new_action;
		res = false;
	}

	return (res);
}

static void
usage(FILE *dest)
{
	fprintf(dest, "Usage: nmea_parser [-dhv]\n");
}