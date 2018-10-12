
#version 400 core
// 应用程序传递过来的是vec2 类型变量，会自动转换成vec4，因为 绑定到slot 0位置的是vec2  着色器一次获取输入也只能获取一个vec2，将vec2 作为输入，
// 转递给vec4，则会进行类型转换
layout( location = 0 ) in vec4 vPosition;

void
main()
{
    gl_Position = vPosition;
}
