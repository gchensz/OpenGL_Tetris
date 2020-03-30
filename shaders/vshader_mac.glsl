attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform int xsize;
uniform int ysize;

void main() 
{
	mat4 scale = mat4(2.0/float(xsize), 0.0, 0.0, 0.0,
					  0.0, 2.0/float(ysize), 0.0, 0.0,
					  0.0, 0.0, 1.0, 0.0,
					  0.0, 0.0, 0.0, 1.0 );

	mat4 transl = mat4( 1.0, 0.0, 0.0, -200.0,
						0.0, 1.0, 0.0, -360.0,
						0.0, 0.0, 1.0, 0.0,
						0.0, 0.0, 0.0, 1.0);

    // First, center the image by translating each vertex by half of
	// the original window size (since the vertices originally have x
	// component in (0, 560) and y component in (0, 724).
	//
	// Then, multiply by the scale matrix to maintain size after the
	// window is reshaped
    
	gl_Position = (vPosition * transl * scale); 
    color = vColor;
}