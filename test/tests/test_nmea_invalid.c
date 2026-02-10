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

#include "../lib/nmea.h"

int
test_nmea_invalid (void)
{
	rgpstk_nmea_message_t message = {0};
	int test_res, res = 0;
	rgpstk_geo_coordinate_t lat = {0}, lon = {0};
	char nmea_bad_checksum[] = "$GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D*A7\r\n";
	char nmea_no_checksum[] = "$GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D\r\n";
	char nmea_gll_too_mony_fields[] = "$GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D,,,,,*7A\r\n";
	char nmea_bad_doubles[] = "$GPGLL,39q3.880fz971,N,10506.7z3ag91v,W,034138.00,A,D\r\n";
	char nmea_gga_fix_quality_invalid[] = "$GPGGA,092750.000,5271.6902,N,00760.3382,W,0,8,1.03,61.7,M,55.2,M,,*79\r\n";
	char nmea_invalid_doubles[] = "$GPGLL,xyz,N,xyx,W,034138.00,A,D\r\n";

	/* expect `rgpstk_nmea_message_load` to return 0, but nmea_valid to be false */
	test_res = rgpstk_nmea_message_load(nmea_bad_checksum, sizeof(nmea_bad_checksum) - 1, &message);
	if (test_res) {
		printf("test_nmea_message_load() returned with a bad checksum %d\n", test_res);
		res = 1;
		goto end;
	}
	if (message.nmea_valid) {
		printf("test_nmea_message_load() did not catch a bad checksum\n");
		res = 2;
		rgpstk_nmea_message_free(&message);
		goto end;
	}
	if (!message.nmea_checksum) {
		printf("test_nmea_message_load() indicates no checksum with a bad checksum\n");
		res = 3;
		rgpstk_nmea_message_free(&message);
		goto end;
	}
	test_res = rgpstk_nmea_gga_lat_long(&message, &lat, &lon);
	if (test_res == 0) {
		printf("rgpstk_nmea_gps_get_lat_long_gga did not fail invalid message.\n");
		res = 4;
		rgpstk_nmea_message_free(&message);
		goto end;
	}
	rgpstk_nmea_message_free(&message);

	/* no checksum isn't invalid but test it here anyway */
	test_res = rgpstk_nmea_message_load(nmea_no_checksum, sizeof(nmea_no_checksum) - 1, &message);
	if (test_res) {
		printf("test_nmea_message_load() returned with no checksum %d\n", test_res);
		res = 5;
		goto end;
	}
	if (!message.nmea_valid) {
		printf("test_nmea_message_load() marked invalid with no checksum\n");
		res = 6;
		rgpstk_nmea_message_free(&message);
		goto end;
	}
	if (message.nmea_checksum) {
		printf("test_nmea_message_load() indicates checksum with a no checksum\n");
		res = 7;
		rgpstk_nmea_message_free(&message);
		goto end;
	}
	rgpstk_nmea_message_free(&message);

	test_res = rgpstk_nmea_message_load(nmea_gll_too_mony_fields, sizeof(nmea_gll_too_mony_fields) - 1, &message);
	if (test_res) {
		printf("test_nmea_message_load() returned with too many fields %d\n", test_res);
		res = 8;
		goto end;
	}
	test_res = rgpstk_nmea_gll_lat_long(&message, &lat, &lon);
	if (test_res == 0) {
		printf("rgpstk_nmea_gps_get_lat_long_gll did not fail invalid message.\n");
		res = 9;
		rgpstk_nmea_message_free(&message);
		goto end;
	}
	rgpstk_nmea_message_free(&message);

	test_res = rgpstk_nmea_message_load(nmea_bad_doubles, sizeof(nmea_bad_doubles) - 1, &message);
	if (test_res) {
		printf("test_nmea_message_load() returned with bad double values %d\n", test_res);
		res = 10;
		goto end;
	}
	test_res = rgpstk_nmea_gll_lat_long(&message, &lat, &lon);
	if (test_res == 0) {
		printf("rgpstk_nmea_gps_get_lat_long_gll did not fail invalid message.\n");
		res = 11;
		rgpstk_nmea_message_free(&message);
		goto end;
	}
	rgpstk_nmea_message_free(&message);

	test_res = rgpstk_nmea_message_load(nmea_gga_fix_quality_invalid, sizeof(nmea_gga_fix_quality_invalid) - 1, &message);
	if (test_res) {
		printf("test_nmea_message_load() returned with fix quality of invalid %d\n", test_res);
		res = 12;
		goto end;
	}
	test_res = rgpstk_nmea_gll_lat_long(&message, &lat, &lon);
	if (test_res == 0) {
		printf("rgpstk_nmea_gps_get_lat_long_gll did not fail invalid message.\n");
		res = 13;
	}
	rgpstk_nmea_message_free(&message);

	test_res = rgpstk_nmea_message_load(nmea_invalid_doubles, sizeof(nmea_invalid_doubles) - 1, &message);
	if (test_res) {
		printf("test_nmea_message_load() returned with invalid doubles %d\n", test_res);
		res = 14;
		goto end;
	}
	test_res = rgpstk_nmea_gll_lat_long(&message, &lat, &lon);
	if (test_res == 0) {
		printf("rgpstk_nmea_gps_get_lat_long_gll did not fail invalid message.\n");
		res = 15;
	}
	rgpstk_nmea_message_free(&message);

end:
	return (res);
}
