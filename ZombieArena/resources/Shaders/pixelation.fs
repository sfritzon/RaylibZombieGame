#version 330

in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform float pixelSize;

out vec4 finalColor;


void main()
{
    ivec2 texSize = textureSize(texture0, 0);
    vec2 pixelBlockSize = vec2(pixelSize) / vec2(texSize);
    vec2 pixelatedUV = floor(fragTexCoord / pixelBlockSize) * pixelBlockSize;
    finalColor = texture(texture0, pixelatedUV);
}