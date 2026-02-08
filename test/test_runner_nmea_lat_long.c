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

#include <geo.h>
#include <stdio.h>

#include "../lib/nmea.h"

int
main(void)
{
	rgpstk_nmea_message_t message;
	rgpstk_geo_coordinate_t lat = {0}, lon = {0};
	int res;
	char nmea_gll[] = "$GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D*7A\r\n";
	char nmea_gga[] = "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n";

	res = rgpstk_nmea_message_load(nmea_gll, sizeof(nmea_gll) - 1, &message);
	if (res) {
		printf("error loading GLL message\n");
		goto end;
	}

	if (!rgpstk_nmea_message_has_lat_long(&message)) {
		printf("rgpstk_nmea_message_has_lat_long returned false for GLL message\n");
		res = 1;
		rgpstk_nmea_message_free(&message);
		goto end;
	}

	res = rgpstk_nmea_gps_get_lat_long(&message, &lat, &lon);
	if (res) {
		printf("error reading lat/long for GGA message\n");
		rgpstk_nmea_message_free(&message);
		goto end;
	}

	res = rgpstk_nmea_message_load(nmea_gga, sizeof(nmea_gga) - 1, &message);
	if (res) {
		printf("error loading GGA message\n");
		goto end;
	}

	if (!rgpstk_nmea_message_has_lat_long(&message)) {
		printf("rgpstk_nmea_message_has_lat_long returned false for GGA message\n");
		res = 1;
		rgpstk_nmea_message_free(&message);
		goto end;
	}
	res = rgpstk_nmea_gps_get_lat_long(&message, &lat, &lon);
	if (res) {
		printf("error reading lat/long for GGA message\n");
		rgpstk_nmea_message_free(&message);
	}

end:
	return (res);
}