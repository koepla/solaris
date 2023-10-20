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

#include <solaris/object.h>

/// Computes the precessed equatorial position of the fixed object with the equinox of date
Equatorial object_position(Object* body, Time* date_time) {
    f64 epoch = time_jc(date_time, false);
    Matrix3x3 precession = matrix3x3_precession(REFERENCE_PLANE_EQUATORIAL, -0.000012775, epoch);
    Vector3 position = vector3_from_equatorial(&body->position);
    Vector3 precessed = matrix3x3_mul_vector3(&precession, &position);
    return equatorial_from_vector3(&precessed);
}

/// Retrieves a string representation of the provided classification
const char* classification_string(Classification classification) {
    switch (classification) {
        case CLASSIFICATION_GALAXY:
            return "Galaxy";
        case CLASSIFICATION_UNIDENTIFIED:
            return "Unidentified";
        case CLASSIFICATION_NONEXISTENT:
            return "Nonexistent";
        case CLASSIFICATION_REFLECTION_NEBULA:
            return "Reflection Nebula";
        case CLASSIFICATION_SINGLE_STAR:
            return "Single Star";
        case CLASSIFICATION_DOUBLE_STAR:
            return "Double Star";
        case CLASSIFICATION_ASTERISM:
            return "Asterism";
        case CLASSIFICATION_PLANETARY_NEBULA:
            return "Planetary Nebula";
        case CLASSIFICATION_UNCERTAIN:
            return "Uncertain";
        case CLASSIFICATION_GLOBULAR_STAR_CLUSTER:
            return "Globular Star Cluster";
        case CLASSIFICATION_OPEN_STAR_CLUSTER:
            return "Open Star Cluster";
        case CLASSIFICATION_CLUSTER:
            return "Cluster";
        case CLASSIFICATION_TRIPLE_STAR:
            return "Triple Star";
        case CLASSIFICATION_KNOT:
            return "Knot";
        case CLASSIFICATION_PHOTOGRAPHIC_PLATE_DEFECT:
            return "Photographic Plate Defect";
        case CLASSIFICATION_PLANET:
            return "Planet";
        default:
            return "unknown";
    }
}

/// Retrieves a string representation of the provided catalog
const char* catalog_string(CatalogName catalog) {
    switch (catalog) {
        case CATALOG_NGC:
            return "NGC";
        case CATALOG_IC:
            return "IC";
        case CATALOG_MESSIER:
            return "M";
        default:
            return "?";
    }
}

/// Retrieves a string representation of the provided constellation
const char* constellation_string(Constellation constellation) {
    switch (constellation) {
        case CONSTELLATION_ANDROMEDA:
            return "Andromeda";
        case CONSTELLATION_CASSIOPEIA:
            return "Cassiopeia";
        case CONSTELLATION_PISCES:
            return "Pisces";
        case CONSTELLATION_PEGASUS:
            return "Pegasus";
        case CONSTELLATION_TUCANA:
            return "Tucana";
        case CONSTELLATION_SCULPTOR:
            return "Sculptor";
        case CONSTELLATION_CETUS:
            return "Cetus";
        case CONSTELLATION_CEPHEUS:
            return "Cepheus";
        case CONSTELLATION_PHOENIX:
            return "Phoenix";
        case CONSTELLATION_HYDRUS:
            return "Hydrus";
        case CONSTELLATION_TRIANGULUM:
            return "Triangulum";
        case CONSTELLATION_OCTANS:
            return "Octans";
        case CONSTELLATION_PERSEUS:
            return "Perseus";
        case CONSTELLATION_ARIES:
            return "Aries";
        case CONSTELLATION_FORNAX:
            return "Fornax";
        case CONSTELLATION_ERIDANUS:
            return "Eridanus";
        case CONSTELLATION_HOROLOGIUM:
            return "Horologium";
        case CONSTELLATION_RETICULUM:
            return "Reticulum";
        case CONSTELLATION_TAURUS:
            return "Taurus";
        case CONSTELLATION_CAMELOPARDALIS:
            return "Camelopardalis";
        case CONSTELLATION_MENSA:
            return "Mensa";
        case CONSTELLATION_DORADO:
            return "Dorado";
        case CONSTELLATION_CAELUM:
            return "Caelum";
        case CONSTELLATION_ORION:
            return "Orion";
        case CONSTELLATION_PICTOR:
            return "Pictor";
        case CONSTELLATION_AURIGA:
            return "Auriga";
        case CONSTELLATION_LEPUS:
            return "Lepus";
        case CONSTELLATION_LUPUS:
            return "Lupus";
        case CONSTELLATION_COLUMBA:
            return "Columba";
        case CONSTELLATION_GEMINI:
            return "Gemini";
        case CONSTELLATION_MONOCEROS:
            return "Monoceros";
        case CONSTELLATION_CARINA:
            return "Carina";
        case CONSTELLATION_PUPPIS:
            return "Puppis";
        case CONSTELLATION_CANIS_MAJOR:
            return "Canis Major";
        case CONSTELLATION_LYNX:
            return "Lynx";
        case CONSTELLATION_VOLANS:
            return "Volans";
        case CONSTELLATION_CANIS_MINOR:
            return "Canis Minor";
        case CONSTELLATION_CANCER:
            return "Cancer";
        case CONSTELLATION_VELA:
            return "Vela";
        case CONSTELLATION_HYDRA:
            return "Hydra";
        case CONSTELLATION_PYXIS:
            return "Pyxis";
        case CONSTELLATION_URSA_MAJOR:
            return "Ursa Major";
        case CONSTELLATION_LEO:
            return "Leo";
        case CONSTELLATION_LEO_MINOR:
            return "Leo Minor";
        case CONSTELLATION_CHAMAELEON:
            return "Chamaeleon";
        case CONSTELLATION_ANTLIA:
            return "Antlia";
        case CONSTELLATION_DRACO:
            return "Draco";
        case CONSTELLATION_SEXTANS:
            return "Sextans";
        case CONSTELLATION_CRATER:
            return "Crater";
        case CONSTELLATION_CENTAURUS:
            return "Centaurus";
        case CONSTELLATION_VIRGO:
            return "Virgo";
        case CONSTELLATION_URSA_MINOR:
            return "Ursa Minor";
        case CONSTELLATION_MUSCA:
            return "Musca";
        case CONSTELLATION_CORVUS:
            return "Corvus";
        case CONSTELLATION_COMA_BERENICES:
            return "Coma Berenices";
        case CONSTELLATION_CRUX:
            return "Crux";
        case CONSTELLATION_CANES_VENATICI:
            return "Canes Venatici";
        case CONSTELLATION_BOOTES:
            return "Bootes";
        case CONSTELLATION_CIRCINUS:
            return "Circinus";
        case CONSTELLATION_APUS:
            return "Apus";
        case CONSTELLATION_LIBRA:
            return "Libra";
        case CONSTELLATION_TRIANGULUM_AUSTRALE:
            return "Triangulum Australe";
        case CONSTELLATION_SERPENS:
            return "Serpens";
        case CONSTELLATION_CORONA_BOREALIS:
            return "Corona Borealis";
        case CONSTELLATION_NORMA:
            return "Norma";
        case CONSTELLATION_SCORPIUS:
            return "Scorpius";
        case CONSTELLATION_HERCULES:
            return "Hercules";
        case CONSTELLATION_OPHIUCHUS:
            return "Ophiuchus";
        case CONSTELLATION_ARA:
            return "Ara";
        case CONSTELLATION_PAVO:
            return "Pavo";
        case CONSTELLATION_SAGITTARIUS:
            return "Sagittarius";
        case CONSTELLATION_CORONA_AUSTRALIS:
            return "Corona Australis";
        case CONSTELLATION_TELESCOPIUM:
            return "Telescopium";
        case CONSTELLATION_LYRA:
            return "Lyra";
        case CONSTELLATION_SCUTUM:
            return "Scutum";
        case CONSTELLATION_AQUILA:
            return "Aquila";
        case CONSTELLATION_VULPECULA:
            return "Vulpecula";
        case CONSTELLATION_CYGNUS:
            return "Cygnus";
        case CONSTELLATION_SAGITTA:
            return "Sagitta";
        case CONSTELLATION_CAPRICORNUS:
            return "Capricornus";
        case CONSTELLATION_DELPHINUS:
            return "Delphinus";
        case CONSTELLATION_MICROSCOPIUM:
            return "Microscopium";
        case CONSTELLATION_INDUS:
            return "Indus";
        case CONSTELLATION_AQUARIUS:
            return "Aquarius";
        case CONSTELLATION_EQUULEUS:
            return "Equuleus";
        case CONSTELLATION_GRUS:
            return "Grus";
        case CONSTELLATION_PISCIS_AUSTRINUS:
            return "Piscis Austrinus";
        case CONSTELLATION_LACERTA:
            return "Lacerta";
        default:
            return "unknown";
    }
}
