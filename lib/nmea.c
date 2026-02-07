/*
 * Created by Roos Catling-Tate.
 * 
 * Copyright 2026
 */

#include "nmea.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char nmea_talker[][2] = {
	"GP",
	"GA",
	"GB",
	"BD",
	"GL",
	"GN",
	"GQ"
};

static char nmea_sentence[][3] = {
	"GGA",
	"GLL",
	"GSA",
	"GSV",
	"RMC",
	"VTG"
};

static rgpstk_nmea_talker_t	nmea_talker_index2enum(uint8_t);

static rgpstk_nmea_talker_t
nmea_talker_index2enum(uint8_t index)
{
	rgpstk_nmea_talker_t talker;

	switch(index) {
	case 0:
		talker = RGPSTK_NMEA_TALKER_GPS;
		break;
	case 1:
		talker = RGPSTK_NMEA_TALKER_GALILEO;
		break;
	case 2:
	case 3:
		talker = RGPSTK_NMEA_TALKER_BEIDOU;
		break;
	case 4:
		talker = RGPSTK_NMEA_TALKER_GLONASS;
		break;
	case 5:
		talker = RGPSTK_NMEA_TALKER_GENERIC_GNSS;
		break;
	case 6:
		talker = RGPSTK_NMEA_TALKER_QZSS;
		break;
	default:
		talker = RGPSTK_NMEA_TALKER_UNKNOWN;
	}
	return talker;
}

static rgpstk_nmea_sentence_t	nmea_sentence_index2enum(uint8_t);

static rgpstk_nmea_sentence_t
nmea_sentence_index2enum(uint8_t index)
{
	rgpstk_nmea_sentence_t sentence;

	switch(index) {
	case 0:
		sentence = RGPSTK_NMEA_SENTENCE_GPS_FD;
		break;
	case 1:
		sentence = RGPSTK_NMEA_SENTENCE_GEO_LAT_LONG;
		break;
	case 2:
		sentence = RGPSTK_NMEA_SENTENCE_GNSS_DOP;
		break;
	case 3:
		sentence = RGPSTK_NMEA_SENTENCE_GNSS_SAT_IN_VIEW;
		break;
	case 4:
		sentence = RGPSTK_NMEA_SENTENCE_REC_MIN;
		break;
	case 5:
		sentence = RGPSTK_NMEA_SENTENCE_GROUND_VEL_COURSE;
		break;
	default:
		sentence = RGPSTK_NMEA_SENTENCE_UNKNOWN;
	}

	return (sentence);
}

bool
rgpstk_nmea_message_has_lat_long(rgpstk_nmea_message_t *msg)
{
	return (msg->nmea_sentence == RGPSTK_NMEA_SENTENCE_GEO_LAT_LONG);
}

int
rgpstk_nmea_gps_get_lat_long(rgpstk_nmea_message_t *msg)
{
	int res = 0;

	if (!rgpstk_nmea_message_has_lat_long(msg)) {
		res = -1;
		goto err;
	}


err:
	return (res);
}

int
rgpstk_nmea_message_load(const char *buffer, uint8_t len, rgpstk_nmea_message_t *msg)
{
	rgpstk_nmea_talker_t talker = RGPSTK_NMEA_TALKER_UNKNOWN;
	rgpstk_nmea_sentence_t sentence = RGPSTK_NMEA_SENTENCE_UNKNOWN;
	int res = 0;
	uint8_t i, j, num_fields = 0, *field_lens, cur_field_len = 0, cur_field_index = 0;
	bool check_sum = false;

	memset(msg, 0, sizeof(rgpstk_nmea_message_t));

	if (!(buffer[0] == RGPSTK_NMEA_CHAR_START || buffer[0] == RGPSTK_NMEA_CHAR_ENCAPSULATION_START)) {
		res = -1;
		goto err;
	}

	/* Handling of extended messages should be handled, but they aren't standardized...
	 * Eventually deal with this better, but this will do for now.
	 */
	if (buffer[1] == RGPSTK_NMEA_EXTENDED) {
		msg->nmea_talker = RGPSTK_NMEA_TALKER_UNKNOWN;
		msg->nmea_sentence = RGPSTK_NMEA_SENTENCE_UNKNOWN;
		goto err;
	}

	if (buffer[len - 1] != RGPSTK_NMEA_CHAR_END) {
		res = -1;
		goto err;
	}
	if (buffer[len - 2] != '\r') {
		res = -1;
		goto err;
	}
	/* NOTE: LENGTH CHECK */

	for (i = 0; i < (uint8_t)(sizeof(nmea_talker) / sizeof(nmea_talker[0])); i++)
		if (!strncmp(nmea_talker[i], buffer + 1, 2))
			talker = nmea_talker_index2enum(i);

	for (i = 1; i < (uint8_t)(sizeof(nmea_sentence) / sizeof(nmea_sentence[0])); i++)
		if (!strncmp(nmea_sentence[i], buffer + 3, 3))
			sentence = nmea_sentence_index2enum(i);

	if (buffer[6] != RGPSTK_NMEA_CHAR_FIELD_DELIMITER) {
		res = -1;
		goto err;
	}

	for (i = 7; i < len; i++) {
		if (buffer[i] == RGPSTK_NMEA_CHAR_FIELD_DELIMITER) {
			if (check_sum) { /* checksum must be last... */
				res = -1;
				goto err;
			}
			num_fields++;
		}
		if (buffer[i] == RGPSTK_NMEA_CHAR_CHECKSUM_DELIMITER) {
			if (check_sum) {
				res = -1;
				goto err;
			} else {
				check_sum = true;
				num_fields++;
			}
		}
	}

	field_lens = calloc(num_fields, sizeof(uint8_t));
	if (field_lens == NULL) {
		res = -1;
		goto err;
	}

	msg->nmea_fields = calloc(num_fields, sizeof(rgpstk_nmea_message_field_t));
	if (msg->nmea_fields == NULL) {
		free(field_lens);
		res = -1;
		goto err;
	}

	msg->nmea_talker = talker;
	msg->nmea_sentence = sentence;
	msg->nmea_checksum = check_sum;
	msg->nmea_fields_count = num_fields;

	for (i = 7; i < len - 3; i++) {
		if (buffer[i] == RGPSTK_NMEA_CHAR_FIELD_DELIMITER
		    || buffer[i] == RGPSTK_NMEA_CHAR_CHECKSUM_DELIMITER) {
			field_lens[cur_field_index] = cur_field_len;
			cur_field_index++;
			cur_field_len = 0;
		} else
			cur_field_len++;
	}

	for (i = 0; i < num_fields; i++) {
		msg->nmea_fields[i].len = field_lens[i];
		msg->nmea_fields[i].value = calloc(1, field_lens[i] + 1); /* +1 for '\0' char */
		if (msg->nmea_fields[i].value == NULL) {
			for (j = 0; j < i; j++)
				free(msg->nmea_fields[j].value);
			free(field_lens);
			free(msg->nmea_fields);
			res = -1;
			goto err;
		}
	}
	free(field_lens);

	cur_field_index = 0;
	cur_field_len = 0;
	for (i = 7; i < len - 3; i++) {
		if (buffer[i] == RGPSTK_NMEA_CHAR_FIELD_DELIMITER
		    || buffer[i] == RGPSTK_NMEA_CHAR_CHECKSUM_DELIMITER) {
			cur_field_index++;
			cur_field_len = 0;
		} else
			msg->nmea_fields[i].value[cur_field_len] = buffer[i];
	}

err:
	return (res);
}

void
rgpstk_nmea_message_start_end_index(const char *msg_buffer, int8_t len, int8_t *start_index, int8_t *end_index)
{
	int8_t i;

	*start_index = -1;
	*end_index = -1;

	for (i = 0; i < len; ++i) {
		if (msg_buffer[i] == RGPSTK_NMEA_CHAR_START || msg_buffer[i] == RGPSTK_NMEA_CHAR_ENCAPSULATION_START)
			*start_index = i;
		if (msg_buffer[i] == RGPSTK_NMEA_CHAR_END)
			*end_index = i;
	}
}

void
rgpstk_nmea_message_free(rgpstk_nmea_message_t *msg)
{
	uint8_t i;

	for (i = 0; i < msg->nmea_fields_count; i++)
		free(msg->nmea_fields[i].value);
	free(msg->nmea_fields);

	memset(msg, 0, sizeof(rgpstk_nmea_message_t));
}