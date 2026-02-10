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
#include "../lib/nmea.h"

int
test_geo_calc(void)
{
	rgpstk_nmea_message_t message1 = {0}, message2 = {0};
	rgpstk_geo_coordinate_t lat_a = {0}, lon_a = {0}, lat_b = {0}, lon_b = {0};
	int res = 0;

	char nmea_gga1[] = "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n";
	char nmea_gga2[] = "$GPGGA,092750.000,5271.6902,N,00760.3382,W,1,8,1.03,61.7,M,55.2,M,,*78\r\n";

	res = rgpstk_nmea_message_load(nmea_gga1, sizeof(nmea_gga1) - 1, &message1);
	if (res) {
		printf("rgpstk_nmea_message_load for GGA #1 returned %d\n", res);
		goto end;
	}
	res = rgpstk_nmea_message_load(nmea_gga2, sizeof(nmea_gga2) - 1, &message2);
	if (res) {
		printf("rgpstk_nmea_message_load for GGA #2 returned %d\n", res);
		rgpstk_nmea_message_free(&message1);
		goto end;
	}

	res = rgpstk_nmea_message_lat_long(&message1, &lat_a, &lon_a);
	if (res) {
		printf("rgpstk_nmea_gps_get_lat_long for GGA #1 returned %d\n", res);
		goto err;
	}
	res = rgpstk_nmea_message_lat_long(&message2, &lat_b, &lon_b);
	if (res) {
		printf("rgpstk_nmea_gps_get_lat_long for GGA #2 returned %d\n", res);
		goto err;
	}

	printf("rgpstk_geo_calculate_distance %f\n", rgpstk_geo_calculate_distance(&lat_a, &lon_a, &lat_b, &lon_b));
	printf("rgpstk_geo_calculate_distance_haversine %f\n",
	    rgpstk_geo_calculate_distance_haversine(&lat_a, &lon_a, &lat_b, &lon_b));
	printf("rgpstk_geo_calculate_distance_spherical_law_of_cosines %f\n",
	    rgpstk_geo_calculate_distance_spherical_law_of_cosines(&lat_a, &lon_a, &lat_b, &lon_b));

	/* load the same values in to each */
	rgpstk_nmea_message_free(&message2);
	res = rgpstk_nmea_message_load(nmea_gga1, sizeof(nmea_gga1) - 1, &message2);
	if (res) {
		printf("rgpstk_nmea_message_load for GGA #2 returned %d\n", res);
		rgpstk_nmea_message_free(&message1);
		goto end;
	}

	res = rgpstk_nmea_message_lat_long(&message2, &lat_b, &lon_b);
	if (res) {
		printf("rgpstk_nmea_gps_get_lat_long for GGA #1 into message #2 returned %d\n", res);
		goto err;
	}

	printf("rgpstk_geo_calculate_distance %f\n", rgpstk_geo_calculate_distance(&lat_a, &lon_a, &lat_b, &lon_b));
	printf("rgpstk_geo_calculate_distance_haversine %f\n",
	    rgpstk_geo_calculate_distance_haversine(&lat_a, &lon_a, &lat_b, &lon_b));
	printf("rgpstk_geo_calculate_distance_spherical_law_of_cosines %f\n",
	    rgpstk_geo_calculate_distance_spherical_law_of_cosines(&lat_a, &lon_a, &lat_b, &lon_b));

err:
	rgpstk_nmea_message_free(&message1);
	rgpstk_nmea_message_free(&message2);
end:
	return (res);
}
