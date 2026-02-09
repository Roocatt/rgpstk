# Roos's GPS Tool Kit (RGPSTK)

[![Build and Test](https://github.com/Roocatt/rgpstk/actions/workflows/workflow.yml/badge.svg)](https://github.com/Roocatt/rgpstk/actions/workflows/workflow.yml)
[![codecov](https://codecov.io/github/Roocatt/rgpstk/graph/badge.svg?token=1LYWL3ZMVB)](https://codecov.io/github/Roocatt/rgpstk)
![GitHub Licence](https://img.shields.io/github/license/Roocatt/piMCP2515?label=licence)

A helpful library and tool collection for handling GPS related tasks.
More tools and functionality will be added over time.

## Notable Features and Functionality

 - NMEA 0183 handling
 - Geographic Distance Calculation
   - Vincenty's Formulae
   - Haversine Formula
   - Spherical Law of Cosines

## Documentation

Documentation is available on the
[GitHub wiki](https://github.com/Roocatt/rgpstk/wiki). The wiki can also
be cloned using git from `https://github.com/Roocatt/rgpstk.wiki.git`.

# Building

```sh
# Generate build files with CMake
cmake .

# Build
make

# Optional: run tests
make test
```

# Licence

This software is available under the ISC licence.