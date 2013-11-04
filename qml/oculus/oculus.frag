uniform sampler2D qt_Texture0;
varying vec2 qt_TexCoord0;
uniform vec2 lensOffsetFromCenter;
uniform vec4 distortion;
uniform float aspectRatio;
uniform float fillScale;

vec2 distortedOffsetFromOffsetCoord(vec2 offsetCoord) {
    vec2 offsetSquared = offsetCoord * offsetCoord;
    float radiusSquared = offsetSquared.x + offsetSquared.y;
    float distortionScale =
            distortion[0] +
            distortion[1] * radiusSquared +
            distortion[2] * radiusSquared * radiusSquared +
            distortion[3] * radiusSquared * radiusSquared * radiusSquared;
    return distortionScale * offsetCoord;
}

vec2 offsetCoordFromTexCoor(vec2 texCoord) {
    vec2 offsetCoord = texCoord;
    offsetCoord *= 2.0;
    offsetCoord -= 1.0;
    offsetCoord -= lensOffsetFromCenter;
    offsetCoord.y /= aspectRatio;
    return offsetCoord;
}

vec2 texCoordFromOffsetCoord(vec2 offsetCoord) {
    vec2 texCoord = offsetCoord;
    texCoord /= fillScale;
    texCoord.y *= aspectRatio;
    texCoord += lensOffsetFromCenter;
    texCoord += 1.0;
    texCoord /= 2.0;
    return texCoord;
}

void main(void) {
    vec2 offsetCoord = offsetCoordFromTexCoor(qt_TexCoord0.st);
    vec2 distortedOffset = distortedOffsetFromOffsetCoord(offsetCoord);
    vec2 distortedTexCoord = texCoordFromOffsetCoord(distortedOffset);
    vec2 clampedDistortedTexCoord = clamp(distortedTexCoord, vec2(0,0), vec2(1,1));
    if(distortedTexCoord.x == clampedDistortedTexCoord.x && distortedTexCoord.y == clampedDistortedTexCoord.y) {
        gl_FragColor = texture2D(qt_Texture0, clampedDistortedTexCoord.st);
    } else {
        gl_FragColor = vec4(0,0,0.2,1);
    }
}
