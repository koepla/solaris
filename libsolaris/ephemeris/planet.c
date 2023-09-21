//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <libsolaris/ephemeris/planet.h>

/// Computes the orbital position of the planet
Elements planet_position_orbital(Planet* planet, DateTime* date) {
    f64 t = date_time_jc(date, false);

    Elements elements;
    elements.semi_major_axis = planet->state.semi_major_axis + planet->rate.semi_major_axis * t;
    elements.eccentricity = planet->state.eccentricity + planet->rate.eccentricity * t;
    elements.inclination = planet->state.inclination + planet->rate.inclination * t;
    elements.mean_longitude = planet->state.mean_longitude + planet->rate.mean_longitude * t;
    elements.lon_perihelion = planet->state.lon_perihelion + planet->rate.lon_perihelion * t;
    elements.lon_asc_node =
            planet->state.lon_asc_node + planet->rate.lon_asc_node * t;
    return elements;
}

/// Computes the eccentric anomaly using an iterative approach of kepler's equation
f64 eccentric_anomaly(f64 mean_anomaly, f64 eccentricity) {
    f64 eccentricity_degrees = math_degrees(eccentricity);
    f64 result = mean_anomaly + eccentricity_degrees * math_sine(mean_anomaly);
    for (usize iteration = 0; iteration < 10; ++iteration) {
        f64 delta_mean_anomaly = mean_anomaly - (result - eccentricity_degrees * math_sine(result));
        f64 delta_eccentric = delta_mean_anomaly / (1 - eccentricity * math_cosine(result));
        result += delta_eccentric;

        if (math_abs(delta_eccentric) < 1.0e-12) {
            break;
        }
    }
    return result;
}

/// Computes the heliocentric position of the earth at the given point in time
Vector3 position_of_earth(f64 julian_centuries) {
    // The EM-Barycenter kepler elements are hardcoded because they are needed for every computation
    Elements elements;
    elements.semi_major_axis = 1.00000261 + 0.00000562 * julian_centuries;
    elements.eccentricity = 0.01671022 - 0.00003804 * julian_centuries;
    elements.mean_longitude = 100.46457166 + 35999.37244981 * julian_centuries;
    elements.lon_perihelion = 102.93768193 + 0.32327364 * julian_centuries;

    f64 a = elements.semi_major_axis;
    f64 e = elements.eccentricity;
    f64 L = elements.mean_longitude;
    f64 w = elements.lon_perihelion;

    f64 mean_anomaly = math_modulo(L - w, 360.0);
    f64 ecc_anomaly = eccentric_anomaly(mean_anomaly, e);

    Vector3 in_orbit;
    in_orbit.x = a * (math_cosine(ecc_anomaly) - e);
    in_orbit.y = a * math_sqrt(1 - e * e) * math_sine(ecc_anomaly);
    in_orbit.z = 0;

    Matrix3x3 rotation = matrix3x3_rotation(ROTATION_AXIS_Z, w);
    return matrix3x3_mul_vector3(&rotation, &in_orbit);
}

/// Computes the equatorial position of the planet
Equatorial planet_position_equatorial(Planet* planet, DateTime* date) {
    Elements elements = planet_position_orbital(planet, date);
    f64 a = elements.semi_major_axis;
    f64 e = elements.eccentricity;
    f64 w = elements.lon_perihelion;
    f64 Om = elements.lon_asc_node;
    f64 L = elements.mean_longitude;
    f64 I = elements.inclination;

    // compute argument of perihelion and mean anomaly
    f64 perihelion = w - Om;
    f64 mean_anomaly = math_modulo(L - w, 360.0);

    // compute eccentric anomaly
    f64 ecc_anomaly = eccentric_anomaly(mean_anomaly, e);

    // true anomaly
    Vector3 in_orbit;
    in_orbit.x = a * (math_cosine(ecc_anomaly) - e);
    in_orbit.y = a * math_sqrt(1 - e * e) * math_sine(ecc_anomaly);
    in_orbit.z = 0;

    // clang-format off
    Matrix3x3 chain[] = {
        matrix3x3_rotation(ROTATION_AXIS_Z, Om),
        matrix3x3_rotation(ROTATION_AXIS_X, I),
        matrix3x3_rotation(ROTATION_AXIS_Z, perihelion)
    };
    // clang-format on
    Matrix3x3 helio_ecliptic_transform = matrix3x3_mul_chain(chain, ARRAY_SIZE(chain));
    Vector3 helio_ecliptic = matrix3x3_mul_vector3(&helio_ecliptic_transform, &in_orbit);

    f64 t = date_time_jc(date, false);
    Vector3 earth = position_of_earth(t);
    Vector3 geo_ecliptic = vector3_sub(&helio_ecliptic, &earth);

    Matrix3x3 reference_transition_transform =
            matrix3x3_reference_plane(REFERENCE_PLANE_ECLIPTIC, REFERENCE_PLANE_EQUATORIAL, 0);
    Vector3 geo_equatorial = matrix3x3_mul_vector3(&reference_transition_transform, &geo_ecliptic);

    Matrix3x3 precession_transform = matrix3x3_precession(REFERENCE_PLANE_EQUATORIAL, 0, t);
    Vector3 geo_equatorial_precessed = matrix3x3_mul_vector3(&precession_transform, &geo_equatorial);
    return equatorial_from_vector3(&geo_equatorial_precessed);
}

/// Retrieves the name of the planet in string representation
const char* planet_name_to_string(PlanetName name) {
    switch (name) {
        case PLANET_MERCURY:
            return "Mercury";
        case PLANET_VENUS:
            return "Venus";
        case PLANET_EARTH:
            return "Earth";
        case PLANET_MARS:
            return "Mars";
        case PLANET_JUPITER:
            return "Jupiter";
        case PLANET_SATURN:
            return "Saturn";
        case PLANET_URANUS:
            return "Uranus";
        case PLANET_NEPTUNE:
            return "Neptune";
        case PLANET_COUNT:
            return "Invalid";
    }
}
