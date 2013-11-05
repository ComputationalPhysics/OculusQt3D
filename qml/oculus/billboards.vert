attribute highp vec4 qt_Vertex;
uniform highp mat4 qt_ModelViewProjectionMatrix;
attribute highp vec4 qt_MultiTexCoord0;
attribute highp vec4 qt_Color;
varying highp vec4 texCoord;

void main(void)
{
    texCoord = qt_MultiTexCoord0;
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
    gl_FrontColor = qt_Color;
}
