#version 330
in vec3 TexCoords;
out vec4 color;
uniform sampler2D panorama;

void main()
{
    // El vertex shader existente invierte pos para los cubemaps antiguos.
    vec3 direction = normalize(-TexCoords);
    const float PI = 3.14159265359;
    vec2 uv = vec2(atan(direction.z, direction.x) / (2.0 * PI) + 0.5,
                   0.5 - asin(clamp(direction.y, -1.0, 1.0)) / PI);
    // Elevar el paisaje para que las montanas sobresalgan del piso.
    // El ajuste disminuye hasta cero en los polos, sin recortar cielo ni suelo.
    const float elevacionPaisaje = 0.12;
    uv.y += elevacionPaisaje * 4.0 * uv.y * (1.0 - uv.y);
    // Imagen cargada sin volteo: fila superior = cielo.
    color = texture(panorama, uv);
}
