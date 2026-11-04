#version 330 core
in vec2 uv;
out vec4 color;
uniform sampler2D smokeTexture;
uniform float age;
uniform float seed;
void main() {
    // Animacion de la imagen: desplazamiento y ondulacion continua de las UV.
    vec2 moving=uv;
    moving.x+=0.025*sin(uv.y*13.0+age*3.5+seed*6.28);
    moving.y+=0.035*sin(age*1.7+seed*6.28);
    vec4 texel=texture(smokeTexture,clamp(moving,vec2(0.001),vec2(0.999)));
    float edge=smoothstep(0.0,0.12,uv.x)*smoothstep(0.0,0.12,1.0-uv.x)
              *smoothstep(0.0,0.12,uv.y)*smoothstep(0.0,0.12,1.0-uv.y);
    float fade=smoothstep(0.0,0.08,age)*(1.0-smoothstep(0.15,2.0,age));
    float alpha=texel.a*edge*fade*0.38;
    if(alpha<0.003) discard;
    color=vec4(mix(vec3(0.7),texel.rgb,0.65),alpha);
}
