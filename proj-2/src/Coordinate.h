//
// Created by aqueiros22 on 18-12-2023.
//

#ifndef PROJETO_AED_2_COORDINATE_H
#define PROJETO_AED_2_COORDINATE_H

/**
 * @brief Class that represents  the coordinates, latitude and longitude
 */
class Coordinate {
    double latitude;
    double longitude;
public:

    Coordinate(double latitude, double longitude);

    Coordinate();

    double getLatitude() const;
    double getLongitude() const;
    double toRadians(double degrees) const;
    double dist_coordinates(Coordinate cord2) const;
};


#endif //PROJETO_AED_2_COORDINATE_H
