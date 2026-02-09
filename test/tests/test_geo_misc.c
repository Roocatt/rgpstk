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

#include <stdio.h>

#include "../lib/geo.h"

int
test_geo_misc(void)
{
	int res = 0;

	if (rgpstk_geo_direction_is_long(RGPSTK_GEO_DIR_NORTH)) {
		printf("RGPSTK_GEO_DIR_NORTH misclasified\n");
		res = 1;
		goto end;
	}
	if (!rgpstk_geo_direction_is_long(RGPSTK_GEO_DIR_EAST)) {
		printf("RGPSTK_GEO_DIR_EAST misclasified\n");
		res = 1;
		goto end;
	}
	if (!rgpstk_geo_direction_is_lat(RGPSTK_GEO_DIR_NORTH)) {
		printf("RGPSTK_GEO_DIR_NORTH misclasified\n");
		res = 1;
		goto end;
	}
	if (rgpstk_geo_direction_is_lat(RGPSTK_GEO_DIR_EAST)) {
		printf("RGPSTK_GEO_DIR_EAST misclasified\n");
		res = 1;
		goto end;
	}
	if (rgpstk_geo_is_direction('K')) {
		printf("'K' should not be a direction.\n");
		res = 1;
		goto end;
	}
	if (!rgpstk_geo_is_direction('N')) {
		printf("'N' should be a direction.\n");
		res = 1;
	}

end:
	return (res);
}
