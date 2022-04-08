// ************************   GET_COMPASS_DATA   ************************//

void get_compass_data(float target_lat, float target_lon)
{
  sensors_event_t event;
  mag.getEvent(&event);

  compass_heading = atan2(event.magnetic.y, event.magnetic.x) * 180.0 / M_PI - compass_offset;
  gps_heading     = atan2(target_lon - gps.location.lng(), target_lat - gps.location.lat()) * 180.0 / M_PI;
  dist_to_target  = gps.distanceBetween(gps.location.lat(), gps.location.lng(), target_lat, target_lon);

  // Once you have your compass_heading, you must then add your 'Declination Angle', which
  // is the 'Error' of the magnetic field in your location in radians.
  // Find yours here: http://www.magnetic-declination.com/
  // Salt Lake is 11°25', or 11.416°

  float declinationAngle = 11.416;
  compass_heading = compass_heading + declinationAngle;

  // Check for wrap due to addition of declination or subtraction of offset.
  if (abs(compass_heading) > 180)
  {
    if (compass_heading > 180) compass_heading -= 360;
    if (compass_heading < -180) compass_heading += 360;
  }

  heading_error = compass_heading - gps_heading;
  // wrap heading_error so it is between -180 and 180
  if (heading_error < -180) heading_error += 360;
  if (heading_error > 180) heading_error += -360;
}
//  ================   End GET_COMPASS_DATA   ============================

