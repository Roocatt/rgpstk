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
test_nmea_checksum(void)
{
	int res = 0;
	char nmea_message[] = "$GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D*"; /* 7A */
	char nmea_message_truncated[] = "$GPGLL,3953.88008971,N,10506.75";
	uint8_t nmea_checksum = 0;

	res = rgpstk_nmea_checksum_calculate(nmea_message, sizeof(nmea_message), &nmea_checksum);
	if (res != 0 || nmea_checksum != 0x7A) {
		printf("Failed checksum calculation for valid message\n");
		res = 1;
		goto end;
	}

	/* intentionally use larger size */
	res = rgpstk_nmea_checksum_calculate(nmea_message_truncated, sizeof(nmea_message) - 1, &nmea_checksum);
	if (res == 0) {
		printf("Passed checksum calculation for truncated message with wrong length\n");
		res = 2;
		goto end;
	}

	res = rgpstk_nmea_checksum_calculate(nmea_message_truncated, sizeof(nmea_message_truncated) - 1, &nmea_checksum);
	if (res == 0) {
		printf("Passed checksum calculation for truncated message\n");
		res = 3;
	} else
		res = 0;

end:
	return (res);
}
