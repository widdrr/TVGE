#version 460 core
in vec4 FragmentPosition;

uniform vec3 glShadowCasterPosition;
uniform float glShadowFarPlane;

void main()
{
    float lightDistance = length(FragmentPosition.xyz - glShadowCasterPosition);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / glShadowFarPlane;
    
    gl_FragDepth = lightDistance;
}  