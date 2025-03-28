#shader vertex

#version 460 core

layout(location = 0) in vec2 position; // Vec2 car tes coordonnées sont 2D (x, y)
layout(location = 1) in vec3 inColor;  // Ajout de l'attribut couleur

out vec3 fragColor; // Envoyer la couleur au fragment shader

uniform float u_Time;
void main()
{
    float offest = sin(u_Time) * 0.5f;
    gl_Position = vec4(position.x + offest, position.y, 0.0, 1.0); // On met Z=0 et W=1 pour la projection
    fragColor = inColor; // Transférer la couleur au fragment shader
}
#shader fragment
#version 460 core

in vec3 fragColor;  // Récupérer la couleur envoyée par le vertex shader
out vec4 color;     // Sortie de la couleur finale

void main()
{
    color = vec4(fragColor, 1.0); // Utiliser la couleur interpolée
}