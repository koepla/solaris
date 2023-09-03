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

#ifndef SOLARIS_EPHEMERIS_FIXED_BODY_H
#define SOLARIS_EPHEMERIS_FIXED_BODY_H

#include <libsolaris/date-time.h>
#include <libsolaris/ephemeris/coordinates.h>
#include <libsolaris/utility/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Classification of the fixed object
/// @see https://cdsarc.cds.unistra.fr/ftp/VII/118/ReadMe
typedef enum Classification {
    CLASSIFICATION_GALAXY,
    CLASSIFICATION_OPEN_STAR_CLUSTER,
    CLASSIFICATION_GLOBULAR_STAR_CLUSTER,
    CLASSIFICATION_REFLECTION_NEBULA,
    CLASSIFICATION_PLANETARY_NEBULA,
    CLASSIFICATION_CLUSTER,
    CLASSIFICATION_ASTERISM,
    CLASSIFICATION_KNOT,
    CLASSIFICATION_TRIPLE_STAR,
    CLASSIFICATION_DOUBLE_STAR,
    CLASSIFICATION_SINGLE_STAR,
    CLASSIFICATION_UNCERTAIN,
    CLASSIFICATION_UNIDENTIFIED,
    CLASSIFICATION_PHOTOGRAPHIC_PLATE_DEFECT,
    CLASSIFICATION_NONEXISTENT,
    CLASSIFICATION_PLANET,
    CLASSIFICATION_COUNT
} Classification;

/// Catalog of the fixed object
/// @note Because solaris makes heave use of ngc.dat
///       files, the main catalogs are NGC and IC.
/// @see https://cdsarc.cds.unistra.fr/ftp/VII/118/ReadMe
typedef enum CatalogName { CATALOG_NGC, CATALOG_IC, CATALOG_MESSIER } CatalogName;

/// Designation of the fixed object
/// @note Designation consists of catalog and the index in
///       the catalog. An example might be M107, whose
///       catalog would be the Messier catalog with an index
///       of 107.
/// @see https://cdsarc.cds.unistra.fr/ftp/VII/118/ReadMe
typedef struct Designation {
    CatalogName catalog;
    usize index;
} Designation;

/// Possible constellations
/// @note Constellations are generated from ngc.dat files,
///       there is no guarantee that this list is complete.
/// @see https://cdsarc.cds.unistra.fr/ftp/VII/118/ReadMe
typedef enum Constellation {
    CONSTELLATION_ANDROMEDA,
    CONSTELLATION_CASSIOPEIA,
    CONSTELLATION_PISCES,
    CONSTELLATION_PEGASUS,
    CONSTELLATION_TUCANA,
    CONSTELLATION_SCULPTOR,
    CONSTELLATION_CETUS,
    CONSTELLATION_CEPHEUS,
    CONSTELLATION_PHOENIX,
    CONSTELLATION_HYDRUS,
    CONSTELLATION_TRIANGULUM,
    CONSTELLATION_OCTANS,
    CONSTELLATION_PERSEUS,
    CONSTELLATION_ARIES,
    CONSTELLATION_FORNAX,
    CONSTELLATION_ERIDANUS,
    CONSTELLATION_HOROLOGIUM,
    CONSTELLATION_RETICULUM,
    CONSTELLATION_TAURUS,
    CONSTELLATION_CAMELOPARDALIS,
    CONSTELLATION_MENSA,
    CONSTELLATION_DORADO,
    CONSTELLATION_CAELUM,
    CONSTELLATION_ORION,
    CONSTELLATION_PICTOR,
    CONSTELLATION_AURIGA,
    CONSTELLATION_LEPUS,
    CONSTELLATION_LUPUS,
    CONSTELLATION_COLUMBA,
    CONSTELLATION_GEMINI,
    CONSTELLATION_MONOCEROS,
    CONSTELLATION_CARINA,
    CONSTELLATION_PUPPIS,
    CONSTELLATION_CANIS_MAJOR,
    CONSTELLATION_LYNX,
    CONSTELLATION_VOLANS,
    CONSTELLATION_CANIS_MINOR,
    CONSTELLATION_CANCER,
    CONSTELLATION_VELA,
    CONSTELLATION_HYDRA,
    CONSTELLATION_PYXIS,
    CONSTELLATION_URSA_MAJOR,
    CONSTELLATION_LEO,
    CONSTELLATION_LEO_MINOR,
    CONSTELLATION_CHAMAELEON,
    CONSTELLATION_ANTLIA,
    CONSTELLATION_DRACO,
    CONSTELLATION_SEXTANS,
    CONSTELLATION_CRATER,
    CONSTELLATION_CENTAURUS,
    CONSTELLATION_VIRGO,
    CONSTELLATION_URSA_MINOR,
    CONSTELLATION_MUSCA,
    CONSTELLATION_CORVUS,
    CONSTELLATION_COMA_BERENICES,
    CONSTELLATION_CRUX,
    CONSTELLATION_CANES_VENATICI,
    CONSTELLATION_BOOTES,
    CONSTELLATION_CIRCINUS,
    CONSTELLATION_APUS,
    CONSTELLATION_LIBRA,
    CONSTELLATION_TRIANGULUM_AUSTRALE,
    CONSTELLATION_SERPENS,
    CONSTELLATION_CORONA_BOREALIS,
    CONSTELLATION_NORMA,
    CONSTELLATION_SCORPIUS,
    CONSTELLATION_HERCULES,
    CONSTELLATION_OPHIUCHUS,
    CONSTELLATION_ARA,
    CONSTELLATION_PAVO,
    CONSTELLATION_SAGITTARIUS,
    CONSTELLATION_CORONA_AUSTRALIS,
    CONSTELLATION_TELESCOPIUM,
    CONSTELLATION_LYRA,
    CONSTELLATION_SCUTUM,
    CONSTELLATION_AQUILA,
    CONSTELLATION_VULPECULA,
    CONSTELLATION_CYGNUS,
    CONSTELLATION_SAGITTA,
    CONSTELLATION_CAPRICORNUS,
    CONSTELLATION_DELPHINUS,
    CONSTELLATION_MICROSCOPIUM,
    CONSTELLATION_INDUS,
    CONSTELLATION_AQUARIUS,
    CONSTELLATION_EQUULEUS,
    CONSTELLATION_GRUS,
    CONSTELLATION_PISCIS_AUSTRINUS,
    CONSTELLATION_LACERTA,
    CONSTELLATION_COUNT
} Constellation;

typedef struct FixedObject {
    Designation designation;
    Constellation constellation;
    Classification classification;
    Equatorial position;
    f64 dimension;
    f64 magnitude;
} FixedObject;

/// Computes the precessed equatorial position of the fixed object with the equinox of date
/// @param date_time Date for computation
/// @return precessed position
SOLARIS_API Equatorial fixed_object_position(FixedObject* body, DateTime* date_time);

/// Retrieves a string representation of the provided classification
/// @param classification The classification
/// @return String representation of the classification
SOLARIS_API const char* classification_string(Classification classification);

/// Retrieves a string representation of the provided catalog
/// @param catalog The catalog
/// @return String representation of the catalog
SOLARIS_API const char* catalog_string(CatalogName catalog);

/// Retrieves a string representation of the provided constellation
/// @param constellation The constellation
/// @return String representation of the constellation
SOLARIS_API const char* constellation_string(Constellation constellation);

#ifdef __cplusplus
}
#endif

#endif// SOLARIS_EPHEMERIS_FIXED_BODY_H
