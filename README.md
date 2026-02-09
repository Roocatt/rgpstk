# Roos's GPS Tool Kit (RGPSTK)

[![Build and Test](https://github.com/Roocatt/rgpstk/actions/workflows/workflow.yml/badge.svg)](https://github.com/Roocatt/rgpstk/actions/workflows/workflow.yml)
![Test Coverage](https://img.shields.io/badge/dynamic/regex?url=https%3A%2F%2Fraw.githubusercontent.com%2FRoocatt%2Frgpstk%2Fmain%2Ftest_coverage.json&search=%7B%22coverage%22%3A%20%22(.*)%22%7D&replace=%241&label=test%20coverage)
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