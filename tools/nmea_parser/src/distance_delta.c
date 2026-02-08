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
#include <string.h>

#include <geo.h>
#include <nmea.h>

#include "distance_delta.h"


static rgpstk_geo_coordinate_t last_lat = {0}, last_lon = {0};

#define BUFFER_SIZE 128

void
distance_delta_read_loop(void)
{
	rgpstk_nmea_message_t message;
	double distance;
	int res;
	uint8_t i = 0;
	char buffer[BUFFER_SIZE], c;
	bool start = false;

	for (;;) {
		c = (char)fgetc(stdin);
		if (c == EOF)
			break;
		if (c == RGPSTK_NMEA_CHAR_START || c == RGPSTK_NMEA_CHAR_ENCAPSULATION_START) {
			i = 0;
			memset(buffer, 0, BUFFER_SIZE);
			start = true;
		}
		if (start)
			buffer[i] = c;
		i++;
		if (c == RGPSTK_NMEA_CHAR_END) {
			res = rgpstk_nmea_message_load(buffer, i, &message);
			if (res)
				continue;
			if (distance_delta(&message, &distance))
				printf("distance delta: %f\n", distance);
			rgpstk_nmea_message_free(&message);
		}
	}
}

/**
 * Read message for a new position.
 *
 * @param message The new NMEA message.
 * @param distance pointer to write the calculated distance value to.
 * @return true if there is a new distance update, otherwise false.
 */
bool
distance_delta(const rgpstk_nmea_message_t *message, double *distance)
{
	rgpstk_geo_coordinate_t lat = {0}, lon = {0};
	bool res = true;

	if (!rgpstk_nmea_message_has_lat_long(message)) {
		res = false;
		goto end;
	}

	rgpstk_nmea_gps_get_lat_long(message, &lat, &lon);

	if (last_lat.direction == RGPSTK_GEO_DIR_UNKNOWN) {
		res = false;
	}

	*distance = rgpstk_geo_calculate_distance(&last_lat, &last_lon, &lat, &lon);

	memcpy(&last_lat, &lat, sizeof(lat));
	memcpy(&last_lon, &lon, sizeof(lon));

end:
	return (res);
}
