Description of input file --> input.info
args:
	- R 					- radius of disk													- 75.0
	- n 					- accuracy number (number of points on contours)					- 4000
	- r 					- initial coordinate of disk center									- 77.0
	- contain_percent 		- percantage of containment (balooned contour outside rough one)	- 0.7
	- max_delta_distance 	- cycle distance													- 0.2
	- cycle_count 			- number of swings inside each cycle								- 2
	- rough_filename 		- filename of rough stone in OBJ format								- rough.obj
	- diamond_filename 		- filename of target stone in OBJ format							- diamond.obj
	- output_filename 		- output filename in boost property tree format						- trace.info