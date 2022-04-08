

// Arduino Code for GPS Car
// Slick Science SSAV (Small Scale Autonomous Vehicle)

/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                          BEGIN STUDENT-ADJUSTABLE SECTION                           //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

// Student-avaliable variables...
char team_name[] = "     ENGR 2900";
char sub_name[] =  "   Let's Do This!";
int min_dist_to_tgt = 2;    // minimum distance (in meters) to target before moving on to next target - More accurate closer, but harder
int delay_at_target = 5;    // delay for x seconds (and wag servo) once at gps target before going to next target - makes it more obvious when you get there
int servo_wag_speed = 250;  // milliseconds waiting between left/right - make smaller for faster wag, larger for slower



// Slick Science 1st Day Demo Course
//char course_name[] = "   2900 1st Demo";
//const int num_gps_tgts = 5;
//float target_lats[num_gps_tgts]  = {  40.671899,   40.672300,   40.672079,   40.672077,   40.671885,   40.671885};
//float target_longs[num_gps_tgts] = {-111.945020, -111.944961, -111.945022, -111.946248, -111.946256};

//char course_name[] = "1st Day Demo";
//const int num_gps_tgts = 6;
//float target_lats[num_gps_tgts]  = {  40.673230,   40.673518,   40.673230,   40.673410,   40.673394,   40.673230};
//float target_longs[num_gps_tgts] = {-111.939819, -111.939632, -111.939529, -111.939966, -111.939513, -111.939819};

//char course_name[] = "   2900 1st Demo";
//const int num_gps_tgts = 6;
//float target_lats[num_gps_tgts]  = { 40.671532,   40.671618,   40.671411,   40.671438,   40.671532  };
//float target_longs[num_gps_tgts] = {-111.946164, -111.946216, -111.946231, -111.946004, -111.946164 };

char course_name[] = "What the...";
const int num_gps_tgts = 6;
float target_lats[num_gps_tgts]  = {  40.671670,   40.671664,   40.671596,   40.671511,   40.671670};
float target_longs[num_gps_tgts] = {-111.946269, -111.946053, -111.946167, -111.946264, -111.946269 };


/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                           END STUDENT-ADJUSTABLE SECTION                            //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////
