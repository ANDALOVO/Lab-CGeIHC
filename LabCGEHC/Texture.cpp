#include "Texture.h"
#include "CommonValues.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = 0;
}
Texture::Texture(const char *FileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = FileLoc;
}

bool Texture::LoadTextureA()
{
	//para cambiar el origen a la esquina inferior izquierda como necesitamos
	stbi_set_flip_vertically_on_load(true);
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth,STBI_rgb_alpha); //el tipo unsigned char es para un array de bytes de la imagen, obtener datos de la imagen 
	if (!texData)
	{
		printf("No se encontró el archivo: %s", fileLocation);
		return false;
	}
	glGenTextures(1, &textureID); //parecido al VAO: crear una textura y asignarle un índice
	glBindTexture(GL_TEXTURE_2D, textureID);//se indica que la textura es de tipo 2D, para superficies planas es suficiente esta textura
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// eje S paralelo a X, repetir sobre el eje
/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);// eje S paralelo a X, repetir sobre el eje pero rotando con forme a un centro
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);// eje S paralelo a X, envolver toda la superficie
*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// eje T paralelo a Y, repetir sobre el eje
	/*GL_TEXTURE_MIN_FILTER: Para más cerca o textura se escala a menor tamaño. GL_TEXTURE_MAG_FILTER: Para más lejos o textura se escala a mayor tamaño. 
	GL_LINEAR  aplica sampling y blending de texels más cercanos. GL_NEAREST aplica sample de texel más cercano
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);//para hacer un unbind de la textura
	stbi_image_free(texData); //para liberar la información de la imagen
	return true;
}
bool Texture::LoadTexture()
{
	//para cambiar el origen a la esquina inferior izquierda como necesitamos
	stbi_set_flip_vertically_on_load(true);
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, STBI_rgb); //el tipo unsigned char es para un array de bytes de la imagen, obtener datos de la imagen 
	if (!texData)
	{
		printf("No se encontró el archivo: %s", fileLocation);
		return false;
	}
	glGenTextures(1, &textureID); //parecido al VAO: crear una textura y asignarle un índice
	glBindTexture(GL_TEXTURE_2D, textureID);//se indica que la textura es de tipo 2D, para superficies planas es suficiente esta textura

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// eje S paralelo a X, repetir sobre el eje
/*	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);// eje S paralelo a X, repetir sobre el eje pero rotando con forme a un centro
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);// eje S paralelo a X, envolver toda la superficie
*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// eje T paralelo a Y, repetir sobre el eje
	/*GL_TEXTURE_MIN_FILTER: Para más cerca o textura se escala a menor tamaño. GL_TEXTURE_MAG_FILTER: Para más lejos o textura se escala a mayor tamaño.
	GL_LINEAR  aplica sampling y blending de texels más cercanos. GL_NEAREST aplica sample de texel más cercano
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GLint alignment = 4;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);//para hacer un unbind de la textura
	stbi_image_free(texData); //para liberar la información de la imagen
	return true;
}
void Texture::ClearTexture()
{

	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}
void Texture::UseTexture()
{	//UnitTexture
	glActiveTexture(GL_TEXTURE0); //para crear un sampler que es lo que necesitan los shaders para poder acceder a la textura: 16 a 32 texturas pueden ser declaradas
	//si hay mas de 1 unittexture se tiene que crear una unifromvariable que haga cambio entre la unit texture a utilizar
	glBindTexture(GL_TEXTURE_2D, textureID);

}

Texture::~Texture()
{
	ClearTexture();
}

// Assimp entrega UV de glTF adaptadas a su convencion; se conserva el mismo
// volteo vertical usado por las texturas externas del proyecto.
bool Texture::LoadFromMemory(const unsigned char* data, int size)
{
    if (!data || size <= 0) return false;
    int w = 0, h = 0, channels = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load_from_memory(data, size, &w, &h, &channels, STBI_rgb_alpha);
    if (!pixels) {
        printf("No se pudo decodificar textura interna: %s\n", stbi_failure_reason());
        return false;
    }
    bool loaded = LoadRGBA(pixels, w, h);
    stbi_image_free(pixels);
    return loaded;
}

bool Texture::LoadRGBA(const unsigned char* data, int imageWidth, int imageHeight)
{
    if (!data || imageWidth <= 0 || imageHeight <= 0) return false;
    if (textureID) glDeleteTextures(1, &textureID);
    width = imageWidth; height = imageHeight; bitDepth = 4;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}
