// triangle.cpp

#include <vgl.h>
#include <stdio.h>
#include <iostream>



GLuint program;
GLuint vao;
GLuint vbo;

void checkShader(GLuint shader)
{
      GLint compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) 
        {
#ifdef _DEBUG
            GLsizei len;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( shader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
#endif /* DEBUG */
        }

}
void init()
{
	static const char* vs_source[] = {
	"#version 430	\n"
	" void main() \n"
	" { 	\n"
	"		const vec4 vertices[] = vec4[]( \n"
	"		vec4(0.25,-0.25,0.5,1.0), \n"
	"		vec4(-0.25,-0.25,0.5,1.0), \n"
	"		vec4( 0.25, 0.25, 0.5, 1.0)); \n"
	" gl_Position = vertices[gl_VertexID]; \n" 
	"} \n"
};
	
	static const char* tcs_source[] = {
		"#version 430 core \n"
		"layout (vertices = 3) out; \n"
		" void main() \n"
		" { \n"
		"	if(gl_InvocationID == 0) \n" 
		" 	{ \n"
		" 		gl_TessLevelInner[0] = 5.0; \n"
		"		gl_TessLevelInner[1] = 5.0;	\n"
		" 		gl_TessLevelOuter[0] = 5.0;	\n"
		" 		gl_TessLevelOuter[1] = 5.0; \n"
		"		gl_TessLevelOuter[2] = 5.0; \n"
		"} \n"
		" gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; \n" 
		" } \n" 
	};
	

	static const char* tes_source[] = {
		"#version 430 core \n"
		" layout (triangles,equal_spacing,cw) in;\n"  
		// layout (triangles, equal_spacing, cw,pint_mode) in;
		" void main() \n" 
		" { \n"
		" 	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +  \n"
		" 				  ( gl_TessCoord.y * gl_in[1].gl_Position) +  \n"
		"				  ( gl_TessCoord.z * gl_in[2].gl_Position);   \n"
		" } \n"
	};

	
	static const char* fs_source[] = {
		"#version 430 core \n"
		" out vec4 color; \n" 
		" void main() \n"
	" { \n" 
	" color = vec4(0.0,0.8,0.0,1.0); \n"
	" } \n"
	};
	
	
	program = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs,1,vs_source,NULL);
	glCompileShader(vs);
    checkShader(vs);
	
	// tessation control shader 
	GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tcs,1,tcs_source,NULL);
	glCompileShader(tcs);
	 checkShader(tcs);

	// tessation evaluation shader
	GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tes,1,tes_source,NULL);
	glCompileShader(tes);
	 checkShader(tes);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs,1,fs_source,NULL);
	glCompileShader(fs);
	checkShader(fs);
	
    glAttachShader(program, vs);
	glAttachShader(program, tcs);
	glAttachShader(program, tes);
	glAttachShader(program, fs);
	
	glLinkProgram(program);
	
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	 
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
}

void display()
{
	static const GLfloat green[] = { 0.0f, 0.05f, 0.20f, 1.0f};
	glClearBufferfv(GL_COLOR,0,green);
	
	glUseProgram(program);
	
	glDrawArrays(GL_PATCHES,0,3);
	
	glutSwapBuffers();
	
}
	 
void shutdown()
{
	glDeleteVertexArrays(1,&vao);
	glDeleteProgram(program);
}

void reshape(int width, int height)
{
	glViewport(0,0, width, height);
	
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'q':
		case 'Q':
		case 033:
		{
			exit(0);
			break;
		}
		case 'm':
		{
			static GLenum mode = GL_FILL;
			mode = (mode == GL_FILL ? GL_LINE : GL_FILL);
			glPolygonMode(GL_FRONT_AND_BACK, mode);
			break;

		}
    }
	glutPostRedisplay();
}
	
int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("triangle");
	
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		fprintf(stderr,"err, init fail, code:%d\n",glewGetErrorString(err));
		exit(0);
	}
	
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	
	glutMainLoop();
	shutdown();
	
	return 0;
}
	