//We create a struct to give properties to the planets/spheres
struct Planet {
	//Scale in three directions/dimensions
	float scaleX;
	float scaleY;
	float scaleZ;

	//Move in three scales/dimensions
	float translateX;
	float translateY;
	float translateZ;

	//Save a multiplier here to change the rotation based on the time in the computer
	float timeDiff;
};


//OPENGL_FRAMEWORK_APPLICATION_SYSTEM_H