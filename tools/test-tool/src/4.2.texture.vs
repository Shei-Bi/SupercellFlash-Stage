#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform vec4 myPMVMatrix;

out vec2 TexCoord;

void main()
{
	const vec4 constantList = vec4(-1.0, 1.0, 0.0, 0.0);
	gl_Position = myPMVMatrix * vec4(aPos, 0.0, 1.0) + constantList;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}