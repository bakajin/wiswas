/* OOMP wiswas
 * 
 * wiswas control based on: 
 * Ultrasonic Ranging Tank Robot
 * 
 * L = Left, R = Right , F = forward , B = backward
 *  U D L R S
 *  8 2 4 6
 */

/******************** 
psuedo 
  map surroundings in 45 degree increments
    movie to the middle of the area 
      (obstacle distance should be equal in both opposite directions)
   determine height of obstacles

   spiral the entire area until the end (and obstacle distance is 0)


**********************/
