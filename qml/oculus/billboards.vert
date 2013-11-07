attribute highp vec4 qt_Vertex;
uniform highp mat4 qt_ModelViewProjectionMatrix;
attribute highp vec4 qt_MultiTexCoord0;
attribute highp vec4 qt_Color;
varying highp vec4 texCoord;
attribute highp vec3 qt_Normal;
uniform highp mat4 qt_ModelViewMatrix;
uniform highp mat3 qt_NormalMatrix;

struct qt_MaterialParameters {
     mediump vec4 emission;
     mediump vec4 ambient;
     mediump vec4 diffuse;
     mediump vec4 specular;
     mediump float shininess;
 };
 uniform qt_MaterialParameters qt_Material;

 struct qt_SingleLightParameters {
     mediump vec4 position;
     mediump vec3 spotDirection;
     mediump float spotExponent;
     mediump float spotCutoff;
     mediump float spotCosCutoff;
     mediump float constantAttenuation;
     mediump float linearAttenuation;
     mediump float quadraticAttenuation;
 };
 uniform qt_SingleLightParameters qt_Light;

 varying mediump vec4 litColor;
 varying mediump vec4 litSecondaryColor;

void main(void)
{
    // Calculate the vertex and normal to use for lighting calculations.
    highp vec4 vertex = qt_ModelViewMatrix * qt_Vertex;
    texCoord = qt_MultiTexCoord0;
    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;

    // Viewer is at infinity.
    vec3 toEye = vec3(0, 0, 1);
    vec3 delta = toEye - vertex.xyz;

    float one_over_color_cutoff = 1.0/10000.0;
    float dr2 = dot(delta,delta);
    float color_factor = max( (1.0-dr2*one_over_color_cutoff),0.3);
    // distance = min(distance, 20.0);

    gl_FrontColor = qt_Color*color_factor;
    gl_FrontColor.w = qt_Color.w;
}
