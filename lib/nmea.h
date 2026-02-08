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

#ifndef RGPSTK_NMEA_H
#define RGPSTK_NMEA_H

#include <stdbool.h>
#include <stdint.h>
#include <geo.h>

#define RGPSTK_NMEA_MAX_LEN 82

#define RGPSTK_NMEA_CHAR_START '$'
#define RGPSTK_NMEA_CHAR_ENCAPSULATION_START '!'
#define RGPSTK_NMEA_CHAR_END '\n'
#define RGPSTK_NMEA_CHAR_FIELD_DELIMITER ','
#define RGPSTK_NMEA_CHAR_CHECKSUM_DELIMITER '*'
#define RGPSTK_NMEA_CHAR_TAG_BLOCK_DELIMITER '\\'
#define RGPSTK_NMEA_CHAR_HEX_DELIMITER '^'
#define RGPSTK_NMEA_CHAR_RESERVED '~'

#define RGPSTK_NMEA_CHAR_RANGE_START 0x20 /* ' ' */
#define RGPSTK_NMEA_CHAR_RANGE_END 0x7e /* '~' */

#define RGPSTK_NMEA_EXTENDED 'P'

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum {
	RGPSTK_NMEA_TALKER_UNKNOWN,
	RGPSTK_NMEA_TALKER_GPS,
	RGPSTK_NMEA_TALKER_GALILEO,
	RGPSTK_NMEA_TALKER_BEIDOU,
	RGPSTK_NMEA_TALKER_GLONASS,
	RGPSTK_NMEA_TALKER_GENERIC_GNSS,
	RGPSTK_NMEA_TALKER_QZSS,
} rgpstk_nmea_talker_t;

/* Aliases to use rgpstk_nmea_talker_t with the char sequence instead of a description as the name. */
#define RGPSTK_NMEA_TALKER_GP GGPSTK_NMEA_TALKER_GPS
#define RGPSTK_NMEA_TALKER_GA RGPSTK_NMEA_TALKER_GALILEO
#define RGPSTK_NMEA_TALKER_GB RGPSTK_NMEA_TALKER_BEIDOU
#define RGPSTK_NMEA_TALKER_BD RGPSTK_NMEA_TALKER_BEIDOU
#define RGPSTK_NMEA_TALKER_GL RGPSTK_NMEA_TALKER_GLONASS
#define RGPSTK_NMEA_TALKER_GN RGPSTK_NMEA_TALKER_GENERIC_GNSS
#define RGPSTK_NMEA_TALKER_GQ RGPSTK_NMEA_TALKER_QZSS

typedef enum {
	RGPSTK_NMEA_SENTENCE_UNKNOWN,
	RGPSTK_NMEA_SENTENCE_GPS_FIX_DATA,
	RGPSTK_NMEA_SENTENCE_GEO_LAT_LONG,
	RGPSTK_NMEA_SENTENCE_GNSS_DOP,
	RGPSTK_NMEA_SENTENCE_GNSS_SAT_IN_VIEW,
	RGPSTK_NMEA_SENTENCE_REC_MIN,
	RGPSTK_NMEA_SENTENCE_GROUND_VEL_COURSE,
} rgpstk_nmea_sentence_t;

/* Aliases to use rgpstk_nmea_sentence_t with the char sequence in the name instead of a description as the name. */
#define RGPSTK_NMEA_SENTENCE_GGA RGPSTK_NMEA_SENTENCE_GPS_FIX_DATA
#define RGPSTK_NMEA_SENTENCE_GLL RGPSTK_NMEA_SENTENCE_GEO_LAT_LONG
#define RGPSTK_NMEA_SENTENCE_GSA RGPSTK_NMEA_SENTENCE_GNSS_DOP
#define RGPSTK_NMEA_SENTENCE_GSV RGPSTK_NMEA_SENTENCE_GNSS_SAT_IN_VIEW
#define RGPSTK_NMEA_SENTENCE_RMC RGPSTK_NMEA_SENTENCE_REC_MIN
#define RGPSTK_NMEA_SENTENCE_VTG RGPSTK_NMEA_SENTENCE_GROUND_VEL_COURSE

typedef struct {
	uint8_t len;
	char *value;
} rgpstk_nmea_message_field_t;

typedef struct {
	rgpstk_nmea_talker_t nmea_talker;
	rgpstk_nmea_sentence_t nmea_sentence;
	uint8_t nmea_fields_count;
	rgpstk_nmea_message_field_t *nmea_fields;
	bool nmea_checksum; /* if true, last field holds checksum */
	bool nmea_valid; /* if valid checksum. Always true from `rgpstk_nmea_message_load` if no checksum is present */
} rgpstk_nmea_message_t;

double	rgpstk_nmea_dm2d(double);
int	rgpstk_checksum_calculate(const char *, uint8_t, uint8_t *);

bool	rgpstk_nmea_message_has_lat_long(const rgpstk_nmea_message_t *);
int	rgpstk_nmea_gps_get_lat_long_gga(const rgpstk_nmea_message_t *, rgpstk_geo_coordinate_t *, rgpstk_geo_coordinate_t *);
int	rgpstk_nmea_gps_get_lat_long_gll(const rgpstk_nmea_message_t *, rgpstk_geo_coordinate_t *, rgpstk_geo_coordinate_t *);
int	rgpstk_nmea_gps_get_lat_long(const rgpstk_nmea_message_t *, rgpstk_geo_coordinate_t *, rgpstk_geo_coordinate_t *);

int	rgpstk_nmea_message_load(const char *, uint8_t, rgpstk_nmea_message_t *);
void	rgpstk_nmea_message_free(rgpstk_nmea_message_t *);

void	rgpstk_nmea_message_start_end_index(const char *, int8_t, int8_t *, int8_t *);

#if defined(__cplusplus)
}
#endif

#endif /* RGPSTK_NMEA_H */