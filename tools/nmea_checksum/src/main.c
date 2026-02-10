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

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <nmea.h>

int
main(int argc, char *argv[])
{
	int res = 0, arg_len, validation_sum;
	uint8_t checksum;
	char validation_buff[2];

	if (argc != 3) {
		fprintf(stderr, "Usage: rgpstk_nmea_checksum <calc|valid> [message]\n");
		goto end;
	}
	arg_len = strlen(argv[2]);

	if (strcmp("calc", argv[1]) == 0) {
		res = rgpstk_nmea_checksum_calculate(argv[2], arg_len, &checksum);
		if (res == 0)
			printf("%02x\n", checksum);
		else
			fprintf(stderr, "Could not calculate checksum. Ensure that '%s' is correct\n", argv[2]);
	} else if (strcmp("valid", argv[1]) == 0) {
		res = rgpstk_nmea_checksum_calculate(argv[2], arg_len, &checksum);
		if (res != 0) {
			fprintf(stderr, "Could not calculate checksum to complete validation.\n");
			goto end;
		}
		validation_buff[0] = argv[2][arg_len - 2];
		validation_buff[1] = argv[2][arg_len - 1];
		errno = 0;
		validation_sum = strtol(validation_buff, NULL, 16);
		if (errno == 0) {
			if ((uint8_t)validation_sum == checksum)
				printf("OK\n");
			else
				printf("FAIL\n");
		} else
			fprintf(stderr, "Could not read checksum field to validate.\n");
	} else {
		fprintf(stderr, "'%s' is not a valid argument. Use 'calc' or 'valid'.\n", argv[1]);
	}

end:
	return (res);
}
