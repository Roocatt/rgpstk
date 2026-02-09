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

#include <math.h>

#include "geo.h"

#define RAD_FROM_DEG(deg) (((deg) * M_PI) / 180.0)
#define DEG_FROM_RAD(rad) ((rad) * 180.0 / M_PI)

double
rgpstk_geo_calculate_distance(const rgpstk_geo_coordinate_t *a_lat_coordinate,
    const rgpstk_geo_coordinate_t *a_lon_coordinate, const rgpstk_geo_coordinate_t *b_lat_coordinate,
    const rgpstk_geo_coordinate_t *b_lon_coordinate)
{
	double distance, a_lat, a_lon, b_lat, b_lon, delta_lon, cos2_sigma_m, lambda, lambda_prev, sin_lambda,
	    cos_lambda, sin_sigma, cos_sigma, sigma, sin_alpha, cos2_alpha, reduced_latitude_a, reduced_latitude_b,
	    tmp_val1, tmp_val2, coef_c, delta_sigma;
	uint16_t i;

	a_lon = a_lon_coordinate->degrees;
	if (a_lon_coordinate->direction == RGPSTK_GEO_DIR_WEST)
		a_lon *= -1.0;
	b_lon = b_lon_coordinate->degrees;
	if (b_lon_coordinate->direction == RGPSTK_GEO_DIR_WEST)
		b_lon *= -1.0;
	b_lon = RAD_FROM_DEG(b_lon);
	a_lon = RAD_FROM_DEG(a_lon);

	a_lat = a_lat_coordinate->degrees;
	if (a_lat_coordinate->direction == RGPSTK_GEO_DIR_SOUTH)
		a_lat *= -1.0;
	b_lat = b_lat_coordinate->degrees;
	if (b_lat_coordinate->direction == RGPSTK_GEO_DIR_SOUTH)
		b_lat *= -1.0;
	a_lat = RAD_FROM_DEG(a_lat);
	b_lat = RAD_FROM_DEG(b_lat);

	delta_lon = b_lon - a_lon;
	if (delta_lon > M_PI)
		delta_lon -= 2 * M_PI;
	if (delta_lon < M_PI * -1.0)
		delta_lon += 2 * M_PI;

	reduced_latitude_a = atan((1.0 - RGPSTK_GEO_WGS84_FLATTENING) * tan(a_lat));
	reduced_latitude_b = atan((1.0 - RGPSTK_GEO_WGS84_FLATTENING) * tan(b_lat));

	i = 0;
	lambda = delta_lon;

	do {
		sin_lambda = sin(lambda);
		cos_lambda = cos(lambda);

		tmp_val1 = cos(reduced_latitude_b) * sin_lambda;
		tmp_val2 = cos(reduced_latitude_a) * sin(reduced_latitude_b) - sin(reduced_latitude_a)
		    * cos(reduced_latitude_b) * cos_lambda;

		sin_sigma = sqrt(tmp_val1 * tmp_val1 + tmp_val2 * tmp_val2);
		if (sin_sigma == 0.0) {
			distance = 0.0;
			goto end;
		}

		cos_sigma = sin(reduced_latitude_a) * sin(reduced_latitude_b) + cos(reduced_latitude_a)
		    * cos(reduced_latitude_b) * cos_lambda;
		sigma = atan2(sin_sigma, cos_sigma);

		sin_alpha = cos(reduced_latitude_a) * cos(reduced_latitude_b) * sin_lambda / sin_sigma;
		cos2_alpha = 1.0 - sin_alpha * sin_alpha;

		if (cos2_alpha != 0.0)
			cos2_sigma_m = cos_sigma - 2.0 * sin(reduced_latitude_a) * sin(reduced_latitude_b) / cos2_alpha;
		else
			cos2_sigma_m = 0.0;

		coef_c = RGPSTK_GEO_WGS84_FLATTENING / 16.0 * cos2_alpha * (4.0 + RGPSTK_GEO_WGS84_FLATTENING * (4.0 - 3.0 * cos2_alpha));

		lambda_prev = lambda;
		lambda = delta_lon + (1.0 - coef_c) * RGPSTK_GEO_WGS84_FLATTENING * sin_alpha *
			 (sigma + coef_c * sin_sigma *
			  (cos2_sigma_m + coef_c * cos_sigma * (-1.0 + 2.0 * cos2_sigma_m * cos2_sigma_m)));

	} while (fabs(lambda - lambda_prev) > RGPSTK_GEO_L_CONVERGE_THRESHOLD && ++i < 1000);

	/* If we cannot converge, fallback to haversine. */
	if (i == 1000) {
		distance = rgpstk_geo_calculate_distance_haversine(a_lat_coordinate, a_lon_coordinate, b_lat_coordinate,
		    b_lon_coordinate);
		goto end;
	}

	tmp_val1 = cos2_alpha * (RGPSTK_GEO_WGS84_SEMI_MAJOR_AXIS * RGPSTK_GEO_WGS84_SEMI_MAJOR_AXIS
	    - RGPSTK_GEO_WGS84_SEMI_MINOR_AXIS * RGPSTK_GEO_WGS84_SEMI_MINOR_AXIS) / (RGPSTK_GEO_WGS84_SEMI_MINOR_AXIS
	    * RGPSTK_GEO_WGS84_SEMI_MINOR_AXIS);
	tmp_val2 = tmp_val1 / 1024.0 * (256 + tmp_val1 * (-128 + tmp_val1 * (74 - 47 * tmp_val1)));
	delta_sigma = tmp_val2 * sin_sigma * (cos2_sigma_m + tmp_val2 / 4.0 * (
	    cos_sigma * (-1 + 2 * cos2_sigma_m * cos2_sigma_m) -
	    tmp_val2 / 6.0 * cos2_sigma_m * (-3 + 4 * sin_sigma * sin_sigma)
	    * (-3 + 4 * cos2_sigma_m * cos2_sigma_m)));

	distance = RGPSTK_GEO_WGS84_SEMI_MINOR_AXIS * (1.0 + tmp_val1 / 16384.0 * (4096.0 + tmp_val1
	    * (-768.0 + tmp_val1 * (320.0 - 175.0 * tmp_val1)))) * (sigma - delta_sigma);

end:
	return (distance);
}

double
rgpstk_geo_calculate_distance_haversine(const rgpstk_geo_coordinate_t *a_lat_coordinate,
    const rgpstk_geo_coordinate_t *a_lon_coordinate, const rgpstk_geo_coordinate_t *b_lat_coordinate,
    const rgpstk_geo_coordinate_t *b_lon_coordinate)
{
	double a_lat, a_lon, b_lat, b_lon, delta_lat, delta_lon, sin_dlat, sin_dlon, distance;

	a_lon = a_lon_coordinate->degrees;
	if (a_lon_coordinate->direction == RGPSTK_GEO_DIR_WEST)
		a_lon *= -1.0; b_lon = b_lon_coordinate->degrees;
	if (b_lon_coordinate->direction == RGPSTK_GEO_DIR_WEST)
		b_lon *= -1.0; b_lon = RAD_FROM_DEG(b_lon);
	a_lon = RAD_FROM_DEG(a_lon);
	a_lat = a_lat_coordinate->degrees;

	if (a_lat_coordinate->direction == RGPSTK_GEO_DIR_SOUTH)
		a_lat *= -1.0;
	b_lat = b_lat_coordinate->degrees;
	if (b_lat_coordinate->direction == RGPSTK_GEO_DIR_SOUTH)
		b_lat *= -1.0;
	a_lat = RAD_FROM_DEG(a_lat);
	b_lat = RAD_FROM_DEG(b_lat);

	delta_lat = b_lat - a_lat;
	delta_lon = b_lon - a_lon;

	if (delta_lon > M_PI)
		delta_lon -= 2 * M_PI;
	if (delta_lon < M_PI * -1.0)
		delta_lon += 2 * M_PI;

	sin_dlat = sin(delta_lat / 2);
	sin_dlon = sin(delta_lon / 2);

	distance = sqrt(sin_dlat * sin_dlat + cos(a_lat) * cos(b_lat) * sin_dlon * sin_dlon);

	/* clamp before asin() to ensure there are no issues from floating point rounding. */
	if (distance > 1.0)
		distance = 1.0;
	else if (distance < 0.0)
		distance = 0.0;

	distance = 2 * asin(distance) * RGPSTK_GEO_EARTH_RADIUS_M;

	return (distance);
}

double
rgpstk_geo_calculate_distance_spherical_law_of_cosines(const rgpstk_geo_coordinate_t *a_lat_coordinate,
    const rgpstk_geo_coordinate_t *a_lon_coordinate, const rgpstk_geo_coordinate_t *b_lat_coordinate,
    const rgpstk_geo_coordinate_t *b_lon_coordinate)
{
	double a_lat, a_lon, b_lat, b_lon, theta, distance;

	a_lon = a_lon_coordinate->degrees;
	if (a_lon_coordinate->direction == RGPSTK_GEO_DIR_WEST)
		a_lon *= -1.0f; b_lon = b_lon_coordinate->degrees;
	if (b_lon_coordinate->direction == RGPSTK_GEO_DIR_WEST)
		b_lon *= -1.0f; theta = RAD_FROM_DEG(a_lon - b_lon);
	b_lon = RAD_FROM_DEG(b_lon);
	a_lon = RAD_FROM_DEG(a_lon);
	a_lat = a_lat_coordinate->degrees;

	if (a_lat_coordinate->direction == RGPSTK_GEO_DIR_SOUTH)
		a_lat *= -1.0f; b_lat = b_lat_coordinate->degrees;
	if (b_lat_coordinate->direction == RGPSTK_GEO_DIR_SOUTH)
		b_lat *= -1.0f;

	a_lat = RAD_FROM_DEG(a_lat);
	b_lat = RAD_FROM_DEG(b_lat);

	distance = sin(a_lat) * sin(b_lat) + cos(a_lat) * cos(b_lat) * cos(theta);
	if (distance > 1.0)
		distance = 1.0;
	if (distance < -1.0)
		distance = -1.0;

	distance = DEG_FROM_RAD(acos(distance)) * 11118.957696;

	return (distance);
}

bool
rgpstk_geo_is_direction(const uint8_t dir)
{
	bool res;

	switch (dir) {
	case RGPSTK_GEO_DIR_NORTH:
	case RGPSTK_GEO_DIR_SOUTH:
	case RGPSTK_GEO_DIR_EAST:
	case RGPSTK_GEO_DIR_WEST:
		res = true;
		break;
	default:
		res = false;
	}

	return (res);
}

bool
rgpstk_geo_direction_is_lat(const rgpstk_geo_direction_t direction)
{
	return (direction == RGPSTK_GEO_DIR_NORTH || direction == RGPSTK_GEO_DIR_SOUTH);
}

bool
rgpstk_geo_direction_is_long(const rgpstk_geo_direction_t direction)
{
	return (direction == RGPSTK_GEO_DIR_EAST || direction == RGPSTK_GEO_DIR_WEST);
}
