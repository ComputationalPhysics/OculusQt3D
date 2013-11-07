varying highp vec4 texCoord;
uniform sampler2D qt_Texture0;
varying highp float atomType;

void main(void) {
    mediump vec4 textureColor = texture2D(qt_Texture0, texCoord.st);
    vec4 color = textureColor*gl_Color;
    if(color.a < 0.9) discard;
    else gl_FragColor = color;
}
