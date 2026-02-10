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

/* Reference messages. */
/*
$GPGSV,8,1,25,21,44,141,47,15,14,049,44,6,31,255,46,3,25,280,44*75
$GPGSV,8,2,25,18,61,057,48,22,68,320,52,27,34,268,47,24,32,076,45*76
$GPGSV,8,3,25,14,51,214,49,19,23,308,46*7E
$GPGSV,8,4,25,51,44,183,49,46,41,169,43,48,36,220,45*47
$GLGSV,8,5,25,82,49,219,52,76,22,051,41,83,37,316,51,67,57,010,51*6C
$GLGSV,8,6,25,77,24,108,44,81,10,181,46,78,1,152,34,66,18,060,45*50
$GLGSV,8,7,25,68,37,284,50*5C
$GBGSV,8,8,25,111,35,221,47,112,4,179,39,114,48,290,48*11
*/

#include <stdio.h>

#include "../lib/nmea.h"

int
test_nmea_gsv(void)
{
	rgpstk_nmea_message_t msg;
	rgpstk_nmea_gsv_sv_t sv;
	rgpstk_nmea_gsv_t gsv;
	int res = 0;
	char nmea_gsv1[] = "$GPGSV,8,1,25,21,44,141,47,15,14,049,44,6,31,255,46,3,25,280,44*75\r\n";
	char nmea_gga[] = "$GPGGA,092750.000,5321.6802,N,00630.3372,W,1,8,1.03,61.7,M,55.2,M,,*76\r\n";


	res = rgpstk_nmea_message_load(nmea_gsv1, sizeof(nmea_gsv1) - 1, &msg);
	if (res != 0) {
		printf("nmea_gsv1 load error %d\n", res);
		res = 1;
		goto end;
	}

	res = rgpstk_nmea_gsv(&msg, &gsv);
	if (res != 0 || gsv.gsv_msg_sv_count != 4) {
		printf("nmea_gsv load error %d\n", res);
		res = 2;
		rgpstk_nmea_message_free(&msg);
		goto end;
	}
	res = rgpstk_nmea_gsv_sv(&msg, &gsv, 0, &sv);
	if (res != 0) {
		printf("nmea_gsv sv error %d\n", res);
		res = 3;
		goto end;
	}

	rgpstk_nmea_message_free(&msg);

	res = rgpstk_nmea_message_load(nmea_gga, sizeof(nmea_gga) - 1, &msg);
	if (res != 0) {
		printf("nmea_gga load error %d\n", res);
		res = 4;
	}
	res = rgpstk_nmea_gsv(&msg, &gsv);
	if (res == 0) {
		printf("rgpstk_nmea_gsv no error when expected %d\n", res);
		res = 5;
	}
	rgpstk_nmea_message_free(&msg);
	res = 0;

end:
	return (res);
}
