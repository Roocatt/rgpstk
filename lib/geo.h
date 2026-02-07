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

#ifndef RGPSTK_GEO_H
#define RGPSTK_GEO_H

#include <stdint.h>
#include <stdbool.h>

#include "geo_defs.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define RGPSTK_GEO_EARTH_RADIUS_M 637100.0
#define RGPSTK_GEO_WGS84_SEMI_MAJOR_AXIS 6378137.0
#define RGPSTK_GEO_WGS84_FLATTENING (1.0 / 298.257223563)
#define RGPSTK_GEO_WGS84_SEMI_MINOR_AXIS (RGPSTK_GEO_WGS84_SEMI_MAJOR_AXIS * (1.0 - RGPSTK_GEO_WGS84_FLATTENING))

#define RGPSTK_GEO_L_CONVERGE_THRESHOLD 1e-12

double	rgpstk_geo_calculate_distance(const rgpstk_geo_coordinate_t *, const rgpstk_geo_coordinate_t *,
   const rgpstk_geo_coordinate_t *, const rgpstk_geo_coordinate_t *);
double	rgpstk_geo_calculate_distance_haversine(const rgpstk_geo_coordinate_t *, const rgpstk_geo_coordinate_t *,
   const rgpstk_geo_coordinate_t *, const rgpstk_geo_coordinate_t *);
double	rgpstk_geo_calculate_distance_spherical_law_of_cosines(const rgpstk_geo_coordinate_t *,
    const rgpstk_geo_coordinate_t *, const rgpstk_geo_coordinate_t *, const rgpstk_geo_coordinate_t *);

bool	rgpstk_geo_is_direction(uint8_t);
bool	rgpstk_geo_direction_is_lat(rgpstk_geo_direction_t);
bool	rgpstk_geo_direction_is_long(rgpstk_geo_direction_t);

#if defined(__cplusplus)
}
#endif

#endif /* RGPSTK_GEO_H */