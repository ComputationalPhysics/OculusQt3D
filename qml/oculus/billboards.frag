varying highp vec4 texCoord;
uniform sampler2D qt_Texture0;
varying highp float atomType;

void main(void) {
    mediump vec4 textureColor = texture2D(qt_Texture0, texCoord.st);
    gl_FragColor = textureColor*gl_Color;
}
