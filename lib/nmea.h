/*
 * Created by Roos Catling-Tate.
 * 
 * Copyright 2026
 */

#ifndef RGPSTK_NMEA_H
#define RGPSTK_NMEA_H

#include <stdbool.h>
#include <stdint.h>

#define RGPSTK_NMEA_MAX_LEN 82

#define RGPSTK_NMEA_CHAR_START '$'
#define RGPSTK_NMEA_CHAR_ENCAPSULATION_START '!'
#define RGPSTK_NMEA_CHAR_END '\n'
#define RGPSTK_NMEA_CHAR_FIELD_DELIMITER ','
#define RGPSTK_NMEA_CHAR_CHECKSUM_DELIMITER '$'
#define RGPSTK_NMEA_CHAR_TAG_BLOCK_DELIMITER '\\'
#define RGPSTK_NMEA_CHAR_HEX_DELIMITER '^'
#define RGPSTK_NMEA_CHAR_RESERVED '~'

#define RGPSTK_NMEA_CHAR_RANGE_START 0x20 /* ' ' */
#define RGPSTK_NMEA_CHAR_RANGE_END 0x7e /* '~' */

#define RGPSTK_NMEA_EXTENDED 'P'

typedef enum {
	RGPSTK_NMEA_TALKER_UNKNOWN,
	RGPSTK_NMEA_TALKER_GPS,
	RGPSTK_NMEA_TALKER_GALILEO,
	RGPSTK_NMEA_TALKER_BEIDOU,
	RGPSTK_NMEA_TALKER_GLONASS,
	RGPSTK_NMEA_TALKER_GENERIC_GNSS,
	RGPSTK_NMEA_TALKER_QZSS,
} rgpstk_nmea_talker_t;

typedef enum {
	RGPSTK_NMEA_SENTENCE_UNKNOWN,
	RGPSTK_NMEA_SENTENCE_GPS_FD,
	RGPSTK_NMEA_SENTENCE_GEO_LAT_LONG,
	RGPSTK_NMEA_SENTENCE_GNSS_DOP,
	RGPSTK_NMEA_SENTENCE_GNSS_SAT_IN_VIEW,
	RGPSTK_NMEA_SENTENCE_REC_MIN,
	RGPSTK_NMEA_SENTENCE_GROUND_VEL_COURSE,
} rgpstk_nmea_sentence_t;

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
} rgpstk_nmea_message_t;

bool	rgpstk_nmea_message_has_lat_long(rgpstk_nmea_message_t *);
int	rgpstk_nmea_gps_get_lat_long(rgpstk_nmea_message_t *);
int	rgpstk_nmea_message_load(const char *, uint8_t, rgpstk_nmea_message_t *);
void	rgpstk_nmea_message_start_end_index(const char *, int8_t, int8_t *, int8_t *);
void	rgpstk_nmea_message_free(rgpstk_nmea_message_t *);

#endif /* RGPSTK_NMEA_H */